#include <iostream>

#include "song.hpp"
#include <chrono>

static uint8_t* audio_pos;
static uint8_t* audio_start;
static unsigned int audio_len;
static std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameCallback;
static void audio_callback(void *userdata, uint8_t *stream, int len) {
    lastFrameCallback = std::chrono::high_resolution_clock::now();
    if(audio_len == 0) {
        audio_pos = audio_start;
    } else {
        len = ( len > audio_len ? audio_len : len );
        SDL_memcpy (stream, audio_pos, len);

        audio_pos += len;
        audio_len -= len;
    }
}

static int isSysBigEndian() {
    int i = 0x00FFFFFF;
    return *(char*)(&i) == 0; 
}

static int convertLittleToSysEndian(int in) {
    int out = 0;
    char* outPtr = (char*)&out;
    char* inPtr = (char*)&in;
    if(isSysBigEndian()) {
        //Big Endian
        for(int i = sizeof(int) - 1; i >= 0; i--) {
            outPtr[sizeof(int) - 1 - i] = inPtr[i];
        }
    } else {
        //Little Endian
        out = in;
    }
    return out;
}

static int convertBigToSysEndian(int in) {
    int out = 0;
    char* outPtr = (char*)&out;
    char* inPtr = (char*)&in;
    if(isSysBigEndian()) {
        //Big Endian
        out = in;
    } else {
        //Little Endian
        for(int i = sizeof(int) - 1; i >= 0; i--) {
            outPtr[sizeof(int) - 1 - i] = inPtr[i];
        }
    }
    return out;
}

Song::Song(std::string path) {
    loadWav(path.c_str());
    playing = false;
}

Song::~Song() {
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
}

int16_t* Song::getCurrentFrame(int frame_length) {
    if(frame_length == -1) {
        frame_length = sample_frame_length;
    }
    unsigned int bytesIntoSong = audio_pos - wav_buffer;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    double nsPerFrame = (1000000000.0 / frames_per_sec);
    int nsSince = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastFrameCallback).count();
    int samplesPlayed = sample_frame_length * ((double)nsSince / nsPerFrame);
    // One sample of a frame is 16 bits

    int sampleFrameOffset = samplesPlayed % sample_frame_length - frame_length / 2;
    return samples + (bytesIntoSong / 2) + sampleFrameOffset;
}

bool Song::loadWav(const char* path) {

    /* Load the WAV */
    if (SDL_LoadWAV(path, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
        return false;
    } 
    
    audio_pos = wav_buffer;
    audio_start = wav_buffer;
    audio_len = wav_length;

    wav_spec.callback = (SDL_AudioCallback)&audio_callback;
    wav_spec.userdata = NULL;

    samples = new int16_t[wav_length / 2];
    if(!samples) {
        std::cerr << "ERROR: Could not initialize samples buffer" << std::endl;
        return false;
    }

    for(int i = 0; i < wav_length; i += 2) {
        samples[i / 2] = convertLittleToSysEndian(*reinterpret_cast<int16_t*>(&wav_buffer[i]));
    }

    frames_per_sec = wav_spec.freq;
    channels = wav_spec.channels;
    sample_frame_length = wav_spec.samples;

    return true;
}

void Song::play() {
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);

    silence_value = wav_spec.silence;

    /* Open the audio device */
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
          fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
          exit(-1);
    }
    
    lastFrameCallback = std::chrono::high_resolution_clock::now();
    SDL_PauseAudio(0);
    playing = true;
}

void Song::pause() {
    SDL_PauseAudio(1);
    playing = false;
}

bool Song::isPlaying() {
    return playing;
}

bool Song::isDone() {
    return audio_len == 0;
}

uint32_t Song::getSampleFrameLength() {
    return sample_frame_length;
}

uint8_t Song::getChannels() {
    return channels;
}

uint8_t Song::getSilenceValue() {
    return silence_value;
}

int Song::getFramesPerSec() {
    return frames_per_sec;
}
