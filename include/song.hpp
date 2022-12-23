#pragma once
#include <cstdint>
#include <SDL2/SDL_audio.h>
#include <string>

class Song {
        SDL_AudioSpec wav_spec;
        uint32_t wav_length;
        uint8_t* wav_buffer;

        int16_t* samples;
        int frames_per_sec;
        uint32_t sample_frame_length;
        uint8_t channels; 
        uint8_t silence_value;

        bool playing;

        bool loadWav(const char* path);

    public:

        void play();
        void pause();
        bool isPlaying();
        bool isDone();
        uint32_t getSampleFrameLength();
        uint8_t getChannels();
        uint8_t getSilenceValue();
        int getFramesPerSec();

        int16_t* getCurrentFrame(int frame_length = -1);

        Song(std::string path);
        ~Song();
};
