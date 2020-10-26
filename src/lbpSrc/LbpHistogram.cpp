
#include "lbpSrc/LbpHistogram.h"
#include "lbpSrc/LbpViewLogger.h"

#include "Simd/SimdBase.h"

LbpHistogram::LbpHistogram(const View &lbpSrc) {
    lbpSrc_ = lbpSrc;
}

LbpHistogram::~LbpHistogram() {
    //DO Nothing
}

HistogramPack LbpHistogram::getHistogramPack(size_t x, size_t y, size_t width, size_t height) {
    HistogramPack histogramPack(x, y, width, height);

    View temp(width, height, View::Gray8);

    // 复制 lbpSrc_ 在(x,y,w,h) 区域内数据至temp
    const uint8_t *src = lbpSrc_.data;
    src += y*lbpSrc_.stride + x;
    uint8_t *dst = temp.data;

    memset(dst, 0, width); //首行数据置零
    src += lbpSrc_.stride;
    dst += temp.stride;
    for (size_t row = 1; row < height - 1; ++row) {
        dst[0] = 0; //第一列数据置零
        for (size_t col = 1; col < width - 1; ++col) {
            dst[col] = src[col];
        }
        dst[width - 1] = 0; //最后一列数据置零

        src += lbpSrc_.stride;
        dst += temp.stride;
    }
    memset(dst, 0, width); //最后一行数据置零

    Simd::Base::Histogram(temp.data, temp.width, temp.height, temp.stride, histogramPack.data());

    return histogramPack;
}

std::vector<HistogramPack> LbpHistogram::getHistogramPacks(size_t width, size_t height, size_t stride_x, size_t stride_y) {
    std::vector<HistogramPack> histogramPacks;

    for (size_t vertical = 0; vertical + height <= lbpSrc_.height; vertical += stride_y) {
        for (size_t horizontal = 0; horizontal + width <= lbpSrc_.width; horizontal += stride_x) {
            HistogramPack histogramPack = getHistogramPack(horizontal, vertical, width, height);
            histogramPacks.push_back(histogramPack);

            if (horizontal + stride_x + width > lbpSrc_.width) {
                //TODO 单独取剩下的区域?
            }
        }

        if (vertical + stride_y + height > lbpSrc_.height) {
            //TODO 单独取剩下的区域?
        }
    }

    return histogramPacks;
}
