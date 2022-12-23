#include "visualizer.hpp"

Visualizer::Visualizer(VisualizerCallback* callbacks, int n, unsigned int* numBuckets) {
    this->n = n;
    //Intialize input buffer
    in = new double[n];
    if(!in) {
        std::cerr << "ERROR: in buffer couldn't intialize" << std::endl;
    }

    //Intialize output buffer
    freqBuckets = n / 2 + 1;
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * freqBuckets);
    if(!out) {
        std::cerr << "ERROR: out buffer couldn't initialize" << std::endl;
    }

    this->outBuckets = numBuckets;

    //Create plan
    plan = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);

    windowCache = new double[n];
    if(!windowCache) {
        std::cerr << "ERROR: window cache buffer couldn't initialize" << std::endl;
    }

    //Populate window cache to save computation time
    for(int i = 0; i < n; i++) {
        windowCache[i] = hann(i, n);
    }

    this->callbacks = callbacks;
}

Visualizer::~Visualizer() {
    delete[] in;
    fftw_free(out);
    fftw_destroy_plan(plan);
}

double* Visualizer::computeFrame(int16_t* sampleFrame, int outBuckets) { 
    double maxValDb = DBL_MIN;
    for(int i = 0; i < n; i++) {
        in[i] = windowCache[i] * sampleFrame[i];
        if(in[i] > maxValDb) {
            maxValDb = in[i];
        }
    }
    maxValDb = 20 * log10(maxValDb);
    fftw_execute(plan);

    double* fft_results = new double[freqBuckets];
    if(!fft_results) {
        std::cerr << "ERROR: Visualizer couldn't initialize results buffer" << std::endl;
    }

    for(int i = 0; i < freqBuckets; i++) {
        //Normalize and take magnitude
        out[i][0] *= (1 / sqrt(n));
        out[i][1] *= (1 / sqrt(n));
        fft_results[i] = magnitude(out[i]);

        if(fft_results[i] != 0) {
            double db = 20 * log10(fft_results[i]);
            fft_results[i] = std::max(0.0, db);
        }
    }
    
    double* results = new double[outBuckets];
    int perBucket = freqBuckets / outBuckets;
    for(int i = 0; i < outBuckets; i++) {
        double sum = getSum(&fft_results[i * perBucket], perBucket);
        results[i] = (sum / perBucket) / maxValDb;
        //if(results[i] > max) {
            //max = results[i];
        //}
    }

    //for(int i = 0; i < outBuckets; i++) {
        //results[i] /= max;
    //}
    return results;
}

void Visualizer::setSong(Song* song) {
    this->song = song;
}

void Visualizer::setCallback(VisualizerCallback* callbacks) {
    this->callbacks = callbacks;
}

void Visualizer::setOutBuckets(unsigned int* numBuckets) {
    outBuckets = numBuckets;
}

// Call callback
bool Visualizer::update() {
    if(song == nullptr) {
        std::cerr << "ERROR: Song must be set to update visualizer" << std::endl;
        return false;
    }

    if(song->isPlaying()) {
        for(int i = 0; callbacks[i] != nullptr; i++) {
            double* bucketVals = computeFrame(song->getCurrentFrame(song->getFramesPerSec()), outBuckets[i]);
            callbacks[i](bucketVals, outBuckets[i]);
        }
    }

    return true;
}
        //double sum = 0.0;
        //for(int j = i * (outLength / NUM_BARS); j < (i + 1) * (outLength / NUM_BARS); j++) {
            ////sum += (magnitude(out[j]) != 0 ? log10(magnitude(out[j])) : 0);
            //sum += (fft_data[j] != 0 ? 20 * std::max(0.0, log10(fft_data[j])) : 0);
            ////std::cout << (magnitude(out[j]) != 0 ? log10(magnitude(out[j])) : 0);
        //}
        //results[i] = (sum / (outLength / NUM_BARS));
        //std::cout << sum / (outLength / NUM_BARS) << " ";
    //}
    //std::cout << std::endl;

    //return results;
//}
