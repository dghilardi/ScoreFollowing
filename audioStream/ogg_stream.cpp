#include "ogg_stream.h"


Ogg_Stream::Ogg_Stream(): samples(0), prevRet(-1){

}

void Ogg_Stream::open(string path)
{
    int result;

    if(!(oggFile = fopen(path.c_str(), "rb")))
        throw string("Could not open Ogg file.");

    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fclose(oggFile);

        throw string("Could not open Ogg stream. ") + errorString(result);
    }

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);

    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;


    alGenBuffers(2, buffers);
    check();
    alGenSources(1, &source);
    check();

    alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
}




void Ogg_Stream::release()
{
    alSourceStop(source);
    empty();
    alDeleteSources(1, &source);
    check();
    alDeleteBuffers(1, buffers);
    check();

    ov_clear(&oggStream);
}




void Ogg_Stream::display()
{
    cout
        << "version         " << vorbisInfo->version         << "\n"
        << "channels        " << vorbisInfo->channels        << "\n"
        << "rate (hz)       " << vorbisInfo->rate            << "\n"
        << "bitrate upper   " << vorbisInfo->bitrate_upper   << "\n"
        << "bitrate nominal " << vorbisInfo->bitrate_nominal << "\n"
        << "bitrate lower   " << vorbisInfo->bitrate_lower   << "\n"
        << "bitrate window  " << vorbisInfo->bitrate_window  << "\n"
        << "\n"
        << "vendor " << vorbisComment->vendor << "\n";

    for(int i = 0; i < vorbisComment->comments; i++)
        cout << "   " << vorbisComment->user_comments[i] << "\n";

    cout << endl;
}




bool Ogg_Stream::playback()
{
    if(playing())
        return true;

    if(!stream(buffers[0]))
        return false;

    if(!stream(buffers[1]))
        return false;

    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);

    return true;
}




bool Ogg_Stream::playing()
{
    ALenum state;

    alGetSourcei(source, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}




bool Ogg_Stream::update()
{
    int processed;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;

        alSourceUnqueueBuffers(source, 1, &buffer);
        check();

        active = stream(buffer);

        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }

    return active;
}




bool Ogg_Stream::stream(ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        result = ov_read(&oggStream, pcm + size, 128, 0, 2, 1, &section);
        result = ov_read(&oggStream, pcm + size, 128, 0, 2, 1, &section);
        assert(result==128);
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }

    if(size == 0)
        return false;

    alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
    check();

    return true;
}




void Ogg_Stream::empty()
{
    int queued;

    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);

    while(queued--)
    {
        ALuint buffer;

        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}




void Ogg_Stream::check()
{
    int error = alGetError();

    if(error != AL_NO_ERROR)
        throw string("OpenAL error was raised.");
}



string Ogg_Stream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return string("Read from media.");
        case OV_ENOTVORBIS:
            return string("Not Vorbis data.");
        case OV_EVERSION:
            return string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string("Invalid Vorbis header.");
        case OV_EFAULT:
            return string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string("Unknown Ogg error.");
    }
}
/*
bool Ogg_Stream::readSample(fvec_t *sample){
    assert(sample->length==BUFFER_SIZE);
    assert(sample->channels==vorbisInfo->channels);

    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        for(int i=0; i<sample->channels; ++i){
            //result = ov_read(&oggStream, sample->data[i] + size, 8, 0, 2, 1, &section);
            assert(result==8);
        }
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }

    if(size == 0)
        return false;

    //alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
    check();

    return true;
}
*/
bool Ogg_Stream::readHeader(ogg_packet &packet, fvec_t *dest){
    int ret = vorbis_synthesis_headerin(vorbisInfo, vorbisComment, &packet);

    if (prevRet == 1 && ret == OV_ENOTVORBIS) {
        // First data packet
        ret = vorbis_synthesis_init(&mDsp, vorbisInfo);
        assert(ret == 0);
        ret = vorbis_block_init(&mDsp, &mBlock);
        assert(ret == 0);
        getFrame(packet, dest);
    }
    else if (ret == 0) {
        prevRet = 1;
    }
}

bool Ogg_Stream::getFrame(ogg_packet &packet, fvec_t *dest){
    int ret = 0;

    if (vorbis_synthesis(&mBlock, &packet) == 0) {
        ret = vorbis_synthesis_blockin(&mDsp, &mBlock);
        assert(ret == 0);
    }

    if((samples = vorbis_synthesis_pcmout(&mDsp, &(dest->data))) > 0){
        ret = vorbis_synthesis_read(&mDsp, samples);
        assert(ret == 0);
        return true;
    }
    cout << samples << endl;
    return false;
}
