//处理流程几乎完全参考dcraw。
//不会最后编程dcraw吧。。。。。。。。。。。。。。。。。。
#include<opencv.hpp>
#include<iostream>
#include<cstdio>
using namespace cv;
//c++ 计时opencv的api的 计时函数。用了api，会比实际时间多1.几毫秒
class Mytime {
public:
    Mytime() {
        time = static_cast<double>(cv::getTickCount());
    }
    Mytime(const std::string& iMes) :mes(iMes) {
        time = static_cast<double>(cv::getTickCount());
    }
    ~Mytime() {
        time = (static_cast<double>(cv::getTickCount()) - time) / cv::getTickFrequency() * 1000;
        std::cout << mes << "耗时：" << time << "ms" << std::endl;
    }
    double time;
    std::string mes;
};






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
void doDemosaic(int quality, cv::Mat& src, cv::Mat& dst);
const char* inputRawPath;
void (*load_raw)();//函数指针。
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
    cv::Mat rgb;
    int quality = 0;
    if (src.data != NULL) {
        Mytime time;
        doDemosaic(quality, src, rgb);
    }
    cv::imwrite("rgb.bmp",rgb);
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
void doDemosaic(int quality, cv::Mat& src, cv::Mat& dst) {
    cv::Mat rgb;
    switch (quality)
    {
    case 0:
        cv::cvtColor(src, rgb, COLOR_BayerGB2BGR);
        break;
    case 1:
        cv::cvtColor(src, rgb, COLOR_BayerGB2BGR_EA);
        break;
    default:

        break;
    }
    dst = rgb;
    if (src.type() == CV_16UC1) {
        rgb.convertTo(dst, CV_32FC3);
        dst = dst / 4096 * 255;//线性量化到0-255；
        dst.convertTo(dst, CV_8UC3);
    }

}