#include <iostream>

#include "lbpSrc/LbpHistogram.h"
#include "lbpSrc/LbpViewLogger.h"

int lbpHistogramTest(int argc, char * argv[]) {
    View image;
    image.Load("picture/lena.pgm"); //width = 32, height = 32, stride = 32, format = 1

    View dst(image.width, image.height, image.format/*View::Gray8*/);

    LbpEstimate(image, dst); //低位从左上角开始, 沿顺时针旋转

    lbpViewLogger logger("temp/View.h5", "UcharArray", dst.width, dst.height); //Test***
    logger.write(dst.data, dst.stride, dst.width, dst.height);

    LbpHistogram lbpHistogram(dst);
    std::vector<HistogramPack> packs = lbpHistogram.getHistogramPacks(64, 64, 64, 64);

    std::cout << "packs size = " << packs.size() << std::endl;

    return 0;
}