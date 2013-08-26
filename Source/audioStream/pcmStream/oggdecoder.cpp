#include "oggdecoder.h"

void VorbisDecode::initForData(OggStream* stream) {
  int ret = vorbis_synthesis_init(&stream->mVorbis.mDsp, &stream->mVorbis.mInfo);
  assert(ret == 0);
  ret = vorbis_block_init(&stream->mVorbis.mDsp, &stream->mVorbis.mBlock);
  assert(ret == 0);
}

bool OggDecoder::read_page(istream& stream, ogg_sync_state* state, ogg_page* page) {
  int ret = 0;

  // If we've hit end of file we still need to continue processing
  // any remaining pages that we've got buffered.
  if (!stream.good())
    return ogg_sync_pageout(state, page) == 1;

  while((ret = ogg_sync_pageout(state, page)) != 1) {
    // Returns a buffer that can be written too
    // with the given size. This buffer is stored
    // in the ogg synchronisation structure.
    char* buffer = ogg_sync_buffer(state, 4096);
    assert(buffer);

    // Read from the file into the buffer
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

bool OggDecoder::readSingleFrame(float ***data, uint *samples){
    *samples = 0;
    int ret;
    ogg_packet packet;
    if(read_packet(is, &state, audio, &packet)){
        if(vorbis_synthesis(&audio->mVorbis.mBlock, &packet)==0){
            ret=vorbis_synthesis_blockin(&audio->mVorbis.mDsp, &audio->mVorbis.mBlock);
            assert(ret==0);
        }

        *samples=vorbis_synthesis_pcmout(&audio->mVorbis.mDsp, data);

        ret = vorbis_synthesis_read(&audio->mVorbis.mDsp, *samples);
        assert(ret == 0);
        return true;
    }
    return false;
}

int OggDecoder::getChannelNumber(){
    return audio->mVorbis.mInfo.channels;
}

void OggDecoder::play(istream& is) {
  ogg_sync_state state;

  int ret = ogg_sync_init(&state);
  assert(ret == 0);

  // Read headers for all streams
  read_headers(is, &state);

  // Find and initialize the first theora and vorbis
  // streams. According to the Theora spec these can be considered the
  // 'primary' streams for playback.
  OggStream* video = 0;
  OggStream* audio = 0;
  for(StreamMap::iterator it = mStreams.begin(); it != mStreams.end(); ++it) {
    OggStream* stream = (*it).second;
    if (!video && stream->mType == TYPE_THEORA) {/*
      video = stream;
      video->mTheora.initForData(video);*/
    }
    else if (!audio && stream->mType == TYPE_VORBIS) {
      audio = stream;
      audio->mVorbis.initForData(audio);
    }
    else
      stream->mActive = false;
  }

  assert(audio);

  if (video) {/*
    cout << "Video stream is "
     << video->mSerial << " "
     << video->mTheora.mInfo.frame_width << "x" << video->mTheora.mInfo.frame_height
     << endl;*/
  }

  cout << "Audio stream is "
       << audio->mSerial << " "
       << audio->mVorbis.mInfo.channels << " channels "
       << audio->mVorbis.mInfo.rate << "KHz"
       << endl;

  /*ret = sa_stream_create_pcm(&mAudio,
                 NULL,
                 SA_MODE_WRONLY,
                 SA_PCM_FORMAT_S16_NE,
                 audio->mVorbis.mInfo.rate,
                 audio->mVorbis.mInfo.channels);
  assert(ret == SA_SUCCESS);

  ret = sa_stream_open(mAudio);
  assert(ret == SA_SUCCESS);
    */
  // Read audio packets, sending audio data to the sound hardware.
  // When it's time to display a frame, decode the frame and display it.
  ogg_packet packet;
  while (read_packet(is, &state, audio, &packet)) {
    if (vorbis_synthesis(&audio->mVorbis.mBlock, &packet) == 0) {
      ret = vorbis_synthesis_blockin(&audio->mVorbis.mDsp, &audio->mVorbis.mBlock);
      assert(ret == 0);
    }

    float** pcm = 0;
    int samples = 0;
    while ((samples = vorbis_synthesis_pcmout(&audio->mVorbis.mDsp, &pcm)) > 0) {
      if (true) {

    if (samples > 0) {

      /*
          size_t size = samples * audio->mVorbis.mInfo.channels;
          short* buffer = new short[size];
      short* p = buffer;
      for (int i=0;i < samples; ++i) {
        for(int j=0; j < audio->mVorbis.mInfo.channels; ++j) {
          int v = static_cast<int>(floorf(0.5 + pcm[j][i]*32767.0));
          if (v > 32767) v = 32767;
          if (v <-32768) v = -32768;
          *p++ = v;
        }
      }

          //ret = sa_stream_write(mAudio, buffer, sizeof(*buffer)*size);
      //assert(ret == SA_SUCCESS);
          delete[] buffer;
      */
    }

    ret = vorbis_synthesis_read(&audio->mVorbis.mDsp, samples);
    assert(ret == 0);
      }

      // At this point we've written some audio data to the sound
      // system. Now we check to see if it's time to display a video
      // frame.
      //
      // The granule position of a video frame represents the time
      // that that frame should be displayed up to. So we get the
      // current time, compare it to the last granule position read.
      // If the time is greater than that it's time to display a new
      // video frame.
      //
      // The time is obtained from the audio system - this represents
      // the time of the audio data that the user is currently
      // listening to. In this way the video frame should be synced up
      // to the audio the user is hearing.
      //
      /*if (video) {
    ogg_int64_t position = 0;
        sa_position_t positionType = SA_POSITION_WRITE_SOFTWARE;
#if defined(WIN32)
        positionType = SA_POSITION_WRITE_HARDWARE;
#endif
        int ret = sa_stream_get_position(mAudio, positionType, &position);
    assert(ret == SA_SUCCESS);
    float audio_time =
      float(position) /
      float(audio->mVorbis.mInfo.rate) /
      float(audio->mVorbis.mInfo.channels) /
      sizeof(short);

    float video_time = th_granule_time(video->mTheora.mCtx, mGranulepos);
    if (audio_time > video_time) {
      // Decode one frame and display it. If no frame is available we
      // don't do anything.
      ogg_packet packet;
      if (read_packet(is, &state, video, &packet)) {
        handle_theora_data(video, &packet);
        video_time = th_granule_time(video->mTheora.mCtx, mGranulepos);
      }
    }
      }*/
    }
        /*
    // Check for SDL events to exit
    SDL_Event event;
    if (SDL_PollEvent(&event) == 1) {
      if (event.type == SDL_KEYDOWN &&
      event.key.keysym.sym == SDLK_ESCAPE)
    break;
      if (event.type == SDL_KEYDOWN &&
      event.key.keysym.sym == SDLK_SPACE)
    SDL_WM_ToggleFullScreen(mSurface);
    } */
  }

  // Cleanup
  ret = ogg_sync_clear(&state);
  assert(ret == 0);
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
