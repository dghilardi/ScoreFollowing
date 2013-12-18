#include "oggdecoder.h"

void VorbisDecode::initForData(OggStream* stream) {
  int ret = vorbis_synthesis_init(&stream->mVorbis.mDsp, &stream->mVorbis.mInfo);
  assert(ret == 0);
  ret = vorbis_block_init(&stream->mVorbis.mDsp, &stream->mVorbis.mBlock);
  assert(ret == 0);
}

bool OggDecoder::read_page(istream& stream, ogg_sync_state* state, ogg_page* page) {
  int ret = 0;
  if (!stream.good())
    return ogg_sync_pageout(state, page) == 1;

  while((ret = ogg_sync_pageout(state, page)) != 1) {
    char* buffer = ogg_sync_buffer(state, 4096);
    assert(buffer);

    stream.read(buffer, 4096);
    int bytes = stream.gcount();
    if (bytes == 0) {
      // End of file.
      continue;
    }

    // Update the synchronisation layer with the number
    // of bytes written to the buffer
    ret = ogg_sync_wrote(state, bytes);
    assert(ret == 0);
  }
  return true;
}

bool OggDecoder::read_packet(istream& is, ogg_sync_state* state, OggStream* stream, ogg_packet* packet) {
  int ret = 0;

  while ((ret = ogg_stream_packetout(&stream->mState, packet)) != 1) {
    ogg_page page;
    if (!read_page(is, state, &page))
      return false;

    int serial = ogg_page_serialno(&page);
    assert(mStreams.find(serial) != mStreams.end());
    OggStream* pageStream = mStreams[serial];

    // Drop data for streams we're not interested in.
    if (stream->mActive) {
      ret = ogg_stream_pagein(&pageStream->mState, &page);
      assert(ret == 0);
    }
  }
  return true;
}

void OggDecoder::read_headers(istream& stream, ogg_sync_state* state) {
  ogg_page page;

  bool headersDone = false;
  while (!headersDone && read_page(stream, state, &page)) {
    int ret = 0;
    int serial = ogg_page_serialno(&page);
    OggStream* stream = 0;

    if(ogg_page_bos(&page)) {
      // At the beginning of the stream, read headers
      // Initialize the stream, giving it the serial
      // number of the stream for this page.
      stream = new OggStream(serial);
      ret = ogg_stream_init(&stream->mState, serial);
      assert(ret == 0);
      mStreams[serial] = stream;
    }

    assert(mStreams.find(serial) != mStreams.end());
    stream = mStreams[serial];

    // Add a complete page to the bitstream
    ret = ogg_stream_pagein(&stream->mState, &page);
    assert(ret == 0);

    // Process all available header packets in the stream. When we hit
    // the first data stream we don't decode it, instead we
    // return. The caller can then choose to process whatever data
    // streams it wants to deal with.
    ogg_packet packet;
    while (!headersDone &&
       (ret = ogg_stream_packetpeek(&stream->mState, &packet)) != 0) {
      assert(ret == 1);

      // A packet is available. If it is not a header packet we exit.
      // If it is a header packet, process it as normal.
      //headersDone = headersDone || handle_theora_header(stream, &packet);
      headersDone = headersDone || handle_vorbis_header(stream, &packet);
      if (!headersDone) {
    // Consume the packet
    ret = ogg_stream_packetout(&stream->mState, &packet);
    assert(ret == 1);
      }
    }
  }
}

OggDecoder::OggDecoder(string filename): mGranulepos(0), is(filename.c_str(), ios::in | ios::binary), audio(0){
    int ret = ogg_sync_init(&state);
    assert(ret==0);
    alreadyReaden.num = 0;

    read_headers(is, &state);

    for(StreamMap::iterator it=mStreams.begin(); it!=mStreams.end(); ++it){
        OggStream *stream = (*it).second;
        if(stream->mType == TYPE_VORBIS){
            audio = stream;
            audio->mVorbis.initForData(audio);
        }else{
            stream->mActive = false;
        }
    }
    assert(audio);
}

int OggDecoder::getSampleRate(){
    return audio->mVorbis.mInfo.rate;
}

OggDecoder::~OggDecoder(){
    for(StreamMap::iterator it = mStreams.begin(); it != mStreams.end(); ++it) {
        OggStream* stream = (*it).second;
        delete stream;
    }
    int ret = ogg_sync_clear(&state);
    assert(ret == 0);
    is.close();
}

/**
 * @brief OggDecoder::readNextFrame Read the next frame (usually 1024 frames but can be less)
 * @param samples number of readen samples
 * @param data samples
 * @return false if the end of the stream is reached
 */
bool OggDecoder::readNextFrame(uint *samples, float ***data)
{
    *samples = 0;
    int ret;
    ogg_packet packet;
    bool readStatus = read_packet(is, &state, audio, &packet);
    if(readStatus){
        if(vorbis_synthesis(&audio->mVorbis.mBlock, &packet)==0){
            ret=vorbis_synthesis_blockin(&audio->mVorbis.mDsp, &audio->mVorbis.mBlock);
            assert(ret==0);
        }

        *samples=vorbis_synthesis_pcmout(&audio->mVorbis.mDsp, data);

        ret = vorbis_synthesis_read(&audio->mVorbis.mDsp, *samples);
        assert(ret == 0);
    }

    return readStatus;
}

bool OggDecoder::readFixedFrame(float **data, uint *samples, uint frameSize){
    float **tmpdata=NULL;
    while(alreadyReaden.num<frameSize){
        bool readOk = readNextFrame(samples, &tmpdata);
        if(!readOk) return false;
        if(*samples>0){
            memcpy(&alreadyReaden.data[0][alreadyReaden.num], tmpdata[0], sizeof(float)*(*samples));
            memcpy(&alreadyReaden.data[1][alreadyReaden.num], tmpdata[1], sizeof(float)*(*samples));
            alreadyReaden.num += *samples;
        }
    }
    *samples = frameSize;
    memcpy(data[0], alreadyReaden.data[0], sizeof(float)*frameSize);
    memcpy(data[1], alreadyReaden.data[1], sizeof(float)*frameSize);
    alreadyReaden.num -= frameSize;
    memcpy(alreadyReaden.data[0], alreadyReaden.data[0]+frameSize, sizeof(float)*alreadyReaden.num);
    memcpy(alreadyReaden.data[1], alreadyReaden.data[1]+frameSize, sizeof(float)*alreadyReaden.num);
    return true;
}

bool OggDecoder::readSingleFrame(float ***data, uint *samples){
    //bool readStatus = readNextFrame(samples, data);

    //*data = new float*[2];
    const int frameSize = FRAME_SIZE;
    //(*data)[0] = new float[frameSize];
    //(*data)[1] = new float[frameSize];
    bool readStatus = readFixedFrame(*data, samples, frameSize);
    return readStatus;
}

int OggDecoder::getChannelNumber(){
    return audio->mVorbis.mInfo.channels;
}

bool OggDecoder::handle_vorbis_header(OggStream* stream, ogg_packet* packet) {
  int ret = vorbis_synthesis_headerin(&stream->mVorbis.mInfo,
                      &stream->mVorbis.mComment,
                      packet);
  // Unlike libtheora, libvorbis does not provide a return value to
  // indicate that we've finished loading the headers and got the
  // first data packet. To detect this I check if I already know the
  // stream type and if the vorbis_synthesis_headerin call failed.
  if (stream->mType == TYPE_VORBIS && ret == OV_ENOTVORBIS) {
    // First data packet
    return true;
  }
  else if (ret == 0) {
    stream->mType = TYPE_VORBIS;
  }
  return false;
}
/*
void usage() {
  cout << "Usage: plogg <filename>" << endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    usage();
    return 0;
  }

  ifstream file(argv[1], ios::in | ios::binary);
  if (file) {
    OggDecoder decoder;
    decoder.play(file);
    file.close();
    for(StreamMap::iterator it = decoder.mStreams.begin();
    it != decoder.mStreams.end();
    ++it) {
      OggStream* stream = (*it).second;
      delete stream;
    }
  }
  SDL_Quit();
  return 0;
}
*/
