/*
 * g++ -o MyTest -Wall -std=c++11 MyTest.cpp `pkg-config --cflags --libs opencv` -I../ libSimd.a -lpthread
*/

#include <iostream>
#include <stdio.h>

#include "opencv2/opencv.hpp"
#include "Simd/SimdLib.hpp"

#include "lbpSrc/StrLogger.h"
#include "lbpSrc/LbpViewLogger.h"

typedef Simd::View<Simd::Allocator> View;

int lbpHistogramTest(int argc, char * argv[]);

int main(int argc, char * argv[]) {

    lbpHistogramTest(0, nullptr);

    // View image;
    // image.Load("lena.pgm"); //width = 32, height = 32, stride = 32, format = 1
    // View dst(image.width, image.height, image.format/*View::Gray8*/);

    // std::cout << "width = " << image.width << ", height = " << image.height << 
    //             ", stride = " << image.stride << ", format = " << image.format << std::endl;

    // std::cout << "width = " << dst.width << ", height = " << dst.height << 
    //             ", stride = " << dst.stride << ", format = " << dst.format << std::endl;

    // for (size_t row = 0; row < image.height; ++row) {
    //     for (size_t col = 0; col < image.width; ++col) {
    //         printf("%02X, ", image.data[image.width * row + col]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // LbpEstimate(image, dst); //低位从左上角开始, 沿顺时针旋转

    // std::string message;
    // for (size_t row = 0; row < dst.height; ++row) {
    //     for (size_t col = 0; col < dst.width; ++col) {
    //         char buf[8];
    //         sprintf(buf, "%02X, ", dst.data[dst.width * row + col]);
    //         message.append(buf);
    //     }
    //     message.append("\n");
    // }

    // std::cout << message << std::endl;

    // dst.Save("lena-lbp.pgm");

    // // StrLogger log0;
    // // log0.setLogFile("logfile.txt");
    // // log0.writeMsg(message);

    // lbpViewLogger log("SDS2.h5", "UcharArray", dst.width, dst.height);
    // // if (dst.width == (size_t)dst.stride) {
    // //     log.write(dst.data);
    // // } else {
    // //     //TODO
    // // }

    return 0;
}
