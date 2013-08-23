#ifndef OGGDECODER_H
#define OGGDECODER_H

#include "debugHeader.h"
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <ogg/ogg.h>
#include <theora/theora.h>
#include <theora/theoradec.h>
#include <vorbis/codec.h>

//#include <SDL/SDL.h>
/*
extern "C" {
#include <sydney_audio.h>
}
*/
using namespace std;

enum StreamType {
  TYPE_VORBIS,
  TYPE_THEORA,
  TYPE_UNKNOWN
};

class OggStream;
/*
class TheoraDecode {
public:
  th_info mInfo;
  th_comment mComment;
  th_setup_info *mSetup;
  th_dec_ctx* mCtx;

public:
  TheoraDecode() :
    mSetup(0),
    mCtx(0)
  {
    th_info_init(&mInfo);
    th_comment_init(&mComment);
  }

  void initForData(OggStream* stream);

  ~TheoraDecode() {
    th_setup_free(mSetup);
    th_decode_free(mCtx);
  }
};
*/
class VorbisDecode {
public:
  vorbis_info mInfo;
  vorbis_comment mComment;
  vorbis_dsp_state mDsp;
  vorbis_block mBlock;

public:
  VorbisDecode()
  {
    vorbis_info_init(&mInfo);
    vorbis_comment_init(&mComment);
  }

  void initForData(OggStream* stream);
};

class OggStream
{
public:
  int mSerial;
  ogg_stream_state mState;
  StreamType mType;
  bool mActive;
  //TheoraDecode mTheora;
  VorbisDecode mVorbis;

public:
  OggStream(int serial = -1) :
    mSerial(serial),
    mType(TYPE_UNKNOWN),
    mActive(true)
  {
  }

  ~OggStream() {
    int ret = ogg_stream_clear(&mState);
    assert(ret == 0);
  }
};

typedef map<int, OggStream*> StreamMap;

class OggDecoder
{
public:
  StreamMap mStreams;
  //SDL_Surface* mSurface;
  //SDL_Overlay* mOverlay;
  //sa_stream_t* mAudio;
  ogg_int64_t  mGranulepos;
  ifstream is;
  ogg_sync_state state;
  OggStream *audio;
private:
  bool handle_theora_header(OggStream* stream, ogg_packet* packet);
  bool handle_vorbis_header(OggStream* stream, ogg_packet* packet);
  void read_headers(istream& stream, ogg_sync_state* state);

  bool read_page(istream& stream, ogg_sync_state* state, ogg_page* page);
  bool read_packet(istream& is, ogg_sync_state* state, OggStream* stream, ogg_packet* packet);
  void handle_theora_data(OggStream* stream, ogg_packet* packet);

public:
  OggDecoder(string filename, unsigned int *channels);
  ~OggDecoder();

  void play(istream& stream);
  bool readSingleFrame(float ***data, uint *samples);
  int getSampleRate();
};

#endif // OGGDECODER_H
