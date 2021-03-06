#include "WavFile.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Wrong parametres!" << endl;
        return 0;
    }
    double resize = atof(argv[3]);
    if (resize == 0) {
        cout << "Wrong koef!" << endl;
        return 0;
    }
    WavFile wavFile(argv[1]);
    wavFile.interpolation(resize);
    wavFile.writeWav(argv[2]);
    return 0;
}