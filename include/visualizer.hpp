#pragma once
#include <cmath>
#include <cstdlib>
#include <string>
#include <fftw3.h>
#include <iostream>
#include <bitset>

#include <vector>

#include "song.hpp"

// Notes pure sinusoid will be all imaginary
// Pure cosine will be all real
// Amplitude can be gotten by dividing by input size and multiplying by 2
// If input vector is size N -> output vector is size n/2 + 1
//
// 1) Slice out a chunk of the signal
// 2) 'window' the chunk*
// 3) Compute the DFT/FFT of the chunk
// 4) Store the DFT/FFT output somewhere
// 5) Slice the next chunk out of the signal**
// 6) Keep repeating until you hit the end of the signal
//

typedef void (*VisualizerCallback)(double* bucketVals, unsigned int numBuckets);

class Visualizer {
        double* windowCache;
        
        double *in;
        int n;

        fftw_complex *out;
        int freqBuckets;
        unsigned int* outBuckets;

        fftw_plan plan;
        
        VisualizerCallback* callbacks; 

        Song* song;

        inline double hann(int i, int N) {
            return 0.5*(1 - cos(2*M_PI*i/(N-1)));
        }

        inline double magnitude(fftw_complex n) {
           return sqrt(n[0] * n[0] + n[1] * n[1]);
        }

        inline double getSum(double* data, int n) {
            double ret = 0.0;
            for(int i = 0; i < n; i++) {
                ret += data[i];
            }

            return ret;
        }


    public:
        Visualizer(VisualizerCallback* callbacks, int n, unsigned int* numBuckets);
        ~Visualizer();

        double* computeFrame(int16_t* sampleFrame, int outBuckets);
        void setSong(Song* song);
        void setCallback(VisualizerCallback* callbacks);
        void setOutBuckets(unsigned int* numBuckets);
        bool update();
};
