/*
 * g++ -o picToLbpTest -Wall -std=c++11 picToLbpTest.cpp ../lbpSrc/LbpViewLogger.cpp `pkg-config --cflags --libs opencv hdf5` -I../ ../../prj/cmake/libSimd.a -lpthread
*/
#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

#ifndef SIMD_OPENCV_ENABLE
#define SIMD_OPENCV_ENABLE
#endif

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include "lbpSrc/LbpViewLogger.h"

typedef Simd::View<Simd::Allocator> View;

int main(int argc, char * argv[]) {
    cv::Mat frame = cv::imread(std::string(argv[1]));
    View image = frame;

    View gray;
    if (image.format != View::Gray8)
    {
        gray.Recreate(image.Size(), View::Gray8);
        Convert(image, gray);
        image = gray;
    }

    View dst(image.width, image.height, image.format/*View::Gray8*/);

    LbpEstimate(image, dst); //低位从左上角开始, 沿顺时针旋转
    dst.Save("picture/hello.pgm");

    lbpViewLogger logger("temp/View1.h5", "UcharArray", dst.width, dst.height);
    logger.write(dst.data, dst.stride, dst.width, dst.height);
}

