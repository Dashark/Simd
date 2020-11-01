#include <cstring>
#include <ctime>
#include <glib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "utility.h"
#include "opencv2/opencv.hpp"

#ifndef SIMD_OPENCV_ENABLE
#define SIMD_OPENCV_ENABLE
#endif

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"
#include "lbpSrc/LbpViewLogger.h"
#include "lbpSrc/LbpHistogram.h"
#include "lbpSrc/StrLogger.h"

typedef Simd::View<Simd::Allocator> View;

int createTrainingSamplesFromInfo(const char* dstdirname, const char* infoname, int winwidth, int winheight) {
    char fullname[4096];
    char* filename;
    
    FILE* info;
    int line = 0, total = 0;
    int x, y, width, height;

    info = fopen( infoname, "r" );
    if (info == NULL ) {
        fprintf( stderr, "Unable to open file: %s\n", infoname );
        return total;
    }

    gchar *h5infofile = g_strdup_printf("%s/lbp-list-pos.info", dstdirname);
    StrLogger h5infologger(h5infofile);

    for (;;) {
        cv::Mat src;
        int count, rc;
        rc = fscanf(info, "%s %d", fullname, &count);
        if (rc != EOF && rc == 2) {
            line++;

            filename = strrchr( fullname, '/' );
            if( filename == NULL ) {
                filename = fullname;
            } else {
                filename++;
            }
            
            gchar **filename_splt = g_strsplit(filename, ".", -1);

            src = cv::imread( fullname, cv::IMREAD_GRAYSCALE );
            if(src.empty()) {
                fprintf( stderr, "Unable to open image: %s\n", fullname );
            }

            for (int i = 0; i < count; i++) {
                int error = ( fscanf( info, "%d %d %d %d", &x, &y, &width, &height ) != 4 );
                if( error ) {
                    fprintf( stderr, "读取图片: %s 的第 %d 个标注信息失败.\n", fullname, i);
                    break;
                }

                cv::Mat slice, sample;
                slice = src(cv::Rect(x, y, width, height));
                resize( slice, sample, cv::Size(winwidth, winheight), 0, 0,
                    width >= winwidth && height >= winheight ? (int)cv::INTER_AREA : CV_INTER_LINEAR );

                View image = sample;
                View dst(image.width, image.height, image.format /*View::Gray8*/);
                LbpEstimate(image, dst); //低位从左上角开始, 沿顺时针旋转

                gchar *h5flie_name = g_strdup_printf("%s/lbp_pos/%s-%d.h5", dstdirname, filename_splt[0], i+1);
                gchar *area_info = g_strdup_printf("x:%d, y:%d, w:%d, h:%d", x, y, width, height);
                h5infologger.writeMsg(std::string(h5flie_name).append("\n"));

                lbpViewLogger h5logger(std::string(h5flie_name), std::string(area_info), dst.width, dst.height);
                h5logger.write(dst.data, dst.stride, dst.width, dst.height);
                g_free(h5flie_name);

                // 读取已缩放图片直方图
                LbpHistogram lbpHistogram(dst);
                HistogramPack hisPack = lbpHistogram.getHistogramPack(0, 0, dst.width, dst.height);
                std::string hisData{std::to_string(hisPack[0])};
                for (size_t hi = 1, size = hisPack.size(); hi < size; hi++) {
                    hisData.append(" ");
                    hisData.append(std::to_string(hisPack[hi]));
                }
                // printf("直方图数据:[%s]\n", hisData.c_str());
                gchar *hisdatafile = g_strdup_printf("%s/lbp_pos/%s-%d.txt", dstdirname, filename_splt[0], i+1);
                StrLogger hisdatalogger(hisdatafile);
                hisdatalogger.writeMsg(hisData);
                
                total++;
            }
            g_strfreev(filename_splt);

        } else if(rc != EOF) {
            fprintf( stderr, "读取图片名及包含标注个数失败.\n");
            break;
        } else {
            // printf("已读取到文件尾部.\n");
            break;
        }

    } //end_for()

    g_free(h5infofile);
    fclose( info );

    return total;
}


int createTrainingSamplesFromBg(const char* dstdirname, const char* bgfilename, int winwidth, int winheight) {
    char *fullname = NULL;
    size_t len = 0;
    ssize_t read;
    char* filename = NULL;
    
    FILE* info;
    int line = 0, total = 0;

    info = fopen( bgfilename, "r" );
    if (info == NULL ) {
        fprintf( stderr, "Unable to open file: %s\n", bgfilename );
        return total;
    }

    gchar *h5infofile = g_strdup_printf("%s/lbp-list-neg.info", dstdirname);
    StrLogger h5infologger(h5infofile);

    for (;;) {
        cv::Mat src;
        read = getline(&fullname, &len, info);
        if (read != -1) {
            g_strchomp(fullname);
            line++;

            filename = strrchr( fullname, '/' );
            if( filename == NULL ) {
                filename = fullname;
            } else {
                filename++;
            }

            gchar **filename_splt = g_strsplit(filename, ".", -1);

            src = cv::imread( fullname, cv::IMREAD_GRAYSCALE );
            if(src.empty()) {
                fprintf( stderr, "Unable to open image: %s\n", fullname );
            }

            cv::Mat sample;
            resize( src, sample, cv::Size(winwidth, winheight), 0, 0,
                src.cols >= winwidth && src.rows >= winheight ? (int)cv::INTER_AREA : CV_INTER_LINEAR );

            View image = sample;
            View dst(image.width, image.height, image.format /*View::Gray8*/);
            LbpEstimate(image, dst); //低位从左上角开始, 沿顺时针旋转

            // LbpHistogram lbpHistogram(dst);

            gchar *h5flie_name = g_strdup_printf("%s/lbp_neg/%s.h5", dstdirname, filename_splt[0]);
            gchar *area_info = g_strdup_printf("x:%d, y:%d, w:%d, h:%d", 0, 0, src.cols, src.rows);
            h5infologger.writeMsg(std::string(h5flie_name).append("\n"));

            lbpViewLogger h5logger(std::string(h5flie_name), std::string(area_info), dst.width, dst.height);
            h5logger.write(dst.data, dst.stride, dst.width, dst.height);
            g_free(h5flie_name);

            // 读取已缩放图片直方图
            LbpHistogram lbpHistogram(dst);
            HistogramPack hisPack = lbpHistogram.getHistogramPack(0, 0, dst.width, dst.height);
            std::string hisData{std::to_string(hisPack[0])};
            for (size_t hi = 1, size = hisPack.size(); hi < size; hi++) {
                hisData.append(" ");
                hisData.append(std::to_string(hisPack[hi]));
            }
            // printf("直方图数据:[%s]\n", hisData.c_str());
            gchar *hisdatafile = g_strdup_printf("%s/lbp_neg/%s.txt", dstdirname, filename_splt[0]);
            StrLogger hisdatalogger(hisdatafile);
            hisdatalogger.writeMsg(hisData);

            g_strfreev(filename_splt);

        } else {
            // printf("已读取到文件尾部.\n");
            break;
        }
    } //end_for()

    if (fullname) {
        free(fullname);
    }
    g_free(h5infofile);
    fclose( info );

    return total;
}


void createTrainingSamples(const char* dstdirname, const char* infoname, 
                            const char* bgfilename,  int winwidth, int winheight) {
    if (infoname ) {
        printf( "读取正样本描述文件 %s\n", infoname );
        createTrainingSamplesFromInfo(dstdirname, infoname, winwidth, winheight);
    }

    if (bgfilename ) {
        printf( "读取负样本描述文件 %s\n", bgfilename );
        createTrainingSamplesFromBg(dstdirname, bgfilename, winwidth, winheight);
    }

}