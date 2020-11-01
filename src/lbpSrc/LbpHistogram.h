#ifndef _LBP_HISTOGRAM_H_
#define _LBP_HISTOGRAM_H_

#include <vector>

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdLib.hpp"
#include "Simd/SimdConst.h"

typedef Simd::View<Simd::Allocator> View;

/*
 * 功能: 直方图数组存储
*/
struct HistogramPack {
    uint32_t histogram[Simd::HISTOGRAM_SIZE]; //直方图数组
    uint32_t x_, y_, width_, height_; //矩形起点(x,y), 宽-width, 高-height

    HistogramPack() = default;
    HistogramPack(size_t x, size_t y, size_t width, size_t height) : x_(x), y_(y), width_(width), height_(height) {

    }

    uint32_t operator[](size_t index) {
        assert(index < Simd::HISTOGRAM_SIZE);
        return histogram[index];
    }

    //获取直方图数组
    uint32_t* data() {
        return histogram;
    }

    //获取直方图数组大小
    size_t size() {
        return Simd::HISTOGRAM_SIZE;
    }
};

/*
 * 功能: 将lbpView且分小格后做直方图转换
*/
class LbpHistogram {
private:
    /* data */
    View lbpSrc_;
public:
    LbpHistogram(const View &lbpSrc);
    ~LbpHistogram();

    // 获取起点坐标(x,y), 宽-width, 高-height 区域的直方图
    HistogramPack getHistogramPack(size_t x, size_t y, size_t width, size_t height);

    // 从坐标(0,0)点开始, 沿Z字形路线按指定步长获取 宽-width, 高-height 的区域直方图集合
    std::vector<HistogramPack> getHistogramPacks(size_t width, size_t height, size_t stride_x, size_t stride_y);
};



#endif /* _LBP_HISTOGRAM_H_ */