//�������̼�����ȫ�ο�dcraw��
//���������dcraw�ɡ�����������������������������������
#include<opencv.hpp>
#include<iostream>
#include<cstdio>
using namespace cv;
static void help(int argc);

enum imageType
{
    RAW_8bit,
    RAW_12bit_2Bytes
};

struct Rawimage
{
    int w;
    int h;
    imageType type;
    void* data;
    int8_t bytePerPixel;
};

Rawimage rawImg;
FILE* ifp;
void identify();//identify raw image message.
void unpacked_load_raw();
const char* inputRawPath;
void (*load_raw)();//����ָ�롣
int main(int argc, char* argv[]) {
    help(argc);
    inputRawPath = argv[1];
    ifp = fopen(inputRawPath, "rb");
    //void*test=malloc(2);
    //std::cout<< fread(test,1,2,ifp)<<"\n";

    if (!ifp) {
        std::cout << "\ncan not open the raw file\n" << std::endl;
    }
    identify();
    rawImg.data = calloc(rawImg.w * rawImg.h, rawImg.bytePerPixel);
    load_raw();
    cv::Mat src;
    if (rawImg.bytePerPixel == 1) {
        src = cv::Mat(rawImg.h, rawImg.w, CV_8UC1, rawImg.data);
    }
    else if (rawImg.bytePerPixel == 2) {
        src = cv::Mat(rawImg.h, rawImg.w, CV_16UC1, rawImg.data);
    }


    fclose(ifp);
    free(rawImg.data);
    return 0;
}

void help(int argc) {
    if (argc == 1) {
        std::cout << "\nneed raw path\n";
        exit;
    }
}
void identify() {
    int fsize = 0;
    static const struct {
        unsigned size;
        ushort rw, rh;
        ushort offset;
    } rawMesTable[] = { {5038848,2592,1944,0},
    {10077696,2592,1944,0} };

    if (ifp) {
        fseek(ifp, 0, SEEK_SET);
        fseek(ifp, 0, SEEK_END);
        fsize = ftell(ifp);
    }
    if (fsize) {
        for (int ti = 0; ti < sizeof rawMesTable / sizeof * rawMesTable; ti++) {
            if (fsize == rawMesTable[ti].size) {
                rawImg.w = rawMesTable[ti].rw;
                rawImg.h = rawMesTable[ti].rh;
                rawImg.bytePerPixel = rawMesTable[ti].size / rawImg.w / rawImg.h;
                load_raw = unpacked_load_raw;
            }
        }
    }
}
void unpacked_load_raw() {
    fseek(ifp, 0, 0);
    int size = rawImg.h * rawImg.w;
    if (fread(rawImg.data, rawImg.bytePerPixel, size, ifp) < size) {
        std::cout << "read raw data error\n" << std::endl;
    };
}