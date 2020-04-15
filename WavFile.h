#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

class WavFile {
public:
    WavFile(char fileName[]);
    ~WavFile();
    void writeWav(char fileName[]);
    void interpolation(double koef);
private:
    // RIFF Header
    int32_t chunkId;    // "RIFF" in ASCII
    int32_t chunkSize;    // file size excluding chankId and chunkSize size(8 bytes)
    int32_t format; // "WAVE" in ASCII

    // "fmt " subchunk
    int32_t subchunk1Id;    // "fmt " in ASCII
    int32_t subchunk1Size;  // "fmt " subchunk size excluding subchunk1Id and subchunk1Size(8), always 16 for PCM
    int16_t audioFormat;    // 1 for PCM, if another - compression
    int16_t numChannels;    // 1 - Mono, 2- Stereo, 4 - Cuadro ...
    int32_t sampleRate; // 44100 hertz
    int32_t byteRate;   // bytes per second (sampleRate * numChannels * bitsPerSample/8)
    int16_t blockAlign; // bytes for one sample including channels(bitsPerSample/8 * numChannels)
    int16_t bitsPerSample;  // 8 or 16 or ... (depth of sound)

    // "data" subchunk
    int32_t subchunk2Id;    // "data" in ASCII
    int32_t subchunk2Size;  // numSamples * blockAlign(bitsPerSample/8 * numChannels) or all samples size
    int16_t* data;   // samples

    int16_t* newData;    // edited samples
    int32_t newSize;
};