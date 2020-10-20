/*
 * g++ -o lbpFaceTest -Wall -std=c++11 lbpFaceTest.cpp `pkg-config --cflags --libs opencv` -I../ libSimd.a -lpthread
*/

#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

#ifndef SIMD_OPENCV_ENABLE
#define SIMD_OPENCV_ENABLE
#endif

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"

int main(int argc, char * argv[]) {

    typedef Simd::Detection<Simd::Allocator> Detection;
    Detection detection;
    detection.Load("../../data/cascade/lbp_face.xml"); //可添加多个xml
    detection.Load("../../data/cascade/haar_face_0.xml");
    bool inited = false;

    const char * WINDOW_NAME = "FaceDetection";
    cv::namedWindow(WINDOW_NAME, 1);

    cv::Mat frame = cv::imread(std::string(argv[1]));;

    Detection::View image = frame;

    if (!inited)
    {
        detection.Init(image.Size(), 1.2, image.Size() / 20);
        inited = true;
    }

    Detection::Objects objects;
    detection.Detect(image, objects);

    for (size_t i = 0; i < objects.size(); ++i)
        Simd::DrawRectangle(image, objects[i].rect, Simd::Pixel::Bgr24(0, 255, 255));

    cv::imshow(WINDOW_NAME, frame);


    cv::waitKey(0);//暂停显示一下,ESC退出

    return 0;
}