#ifndef GLOBAL_H
#define GLOBAL_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

const int SWIRWIDTH = 1280;
const int SWIRHEIGHT = 1024;
const int FRAME_PMNUM =	16;
const int FRAME_PXCNT = SWIRWIDTH * SWIRHEIGHT;
const int FRAMEBUFSIZE = FRAME_PXCNT * 2 + 64;

typedef struct _framebuffer {
        uint16_t image[FRAME_PXCNT];
        uint32_t param[FRAME_PMNUM];
} PACKAGEBUFFER;
typedef PACKAGEBUFFER* LPPACKAGEBUFFER;

typedef struct
{
    QString path;
    QString ip;
    int port;
    int nGain;
    bool bHighgain;
    bool bHistogram;
    bool bSmooth;
    bool bIntegral;
    bool bCorrection;
    bool bMirror;
    bool bImageMode;    //局部放大图像模式/文本模式
} SWIRSETTINGS;

void msleep(unsigned int msec);
QImage mat2qimage(const cv::Mat &mat);
cv::Mat qimage2mat(const QImage &qimage);

#endif // GLOBAL_H
