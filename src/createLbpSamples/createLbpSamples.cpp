
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
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

/*
 * ./createLbpSamples -data <.../> -info <.../metre.info> -bg <.../bg.txt> -w <40> -h <40>
 * eg ./createLbpSamples -data /home/axis/pictures_/hello 
 *      -info /home/axis/pictures_/profile/metre.info 
 *      -bg /home/axis/pictures_/profile/bg.txt -w 40 -h 40
*/
static int icvMkDir( const char* filename );

int main(int argc, char* argv[]) {
    char* nullname   = (char*)"(NULL)";
    char* dstDirName = NULL; //目标文件存放位置，在此路径下有 pos/ 和 neg/ 文件夹
    char* infoName   = NULL; //正样本描述文件全路径
    char* bgfileName = NULL; //负样本描述文件全路径
    int width  = 24;
    int height = 24;

    if( argc == 1 || !strcmp(argv[1], "--help") ) {
        printf( "Usage: %s\n  [-data <转换后文件存放目录>]\n"
                "  [-info <正样本描述文件>]\n"
                "  [-bg <负样本描述文件>]\n"
                "  [-w <压缩图宽，默认 %d>]\n  [-h <压缩图高，默认 %d>]\n",
                argv[0], width, height);

        return 0;
    }

    for(int i = 1; i < argc; ++i) {
        if( !strcmp( argv[i], "-data" ) ) {
            dstDirName = argv[++i];
        } else if (!strcmp(argv[i], "-info") ) {
            infoName = argv[++i];
        } else if (!strcmp(argv[i], "-bg") ) {
            bgfileName = argv[++i];
        } else if (!strcmp(argv[i], "-w")) {
            width = atoi(argv[++i]);
        } else if(!strcmp(argv[i], "-h") ) {
            height = atoi(argv[++i]);
        }
    }

    if (!dstDirName || !infoName || !bgfileName) {
        printf("输入 %s --help 获取更多信息\n", argv[0]);
        return 0;
    }

    printf("dest file name: %s\n", ((dstDirName == NULL) ? nullname : dstDirName ) );
    printf("Info file name: %s\n", ((infoName == NULL) ? nullname : infoName ) );
    printf("BG   file name: %s\n", ((bgfileName == NULL) ? nullname : bgfileName ) );
    printf("Width: %d\n", width );
    printf("Height: %d\n", height );

    gchar *dstDirName_, *dstDirName_p, *dstDirName_n;
    if (dstDirName[strlen(dstDirName)-1] == '/') {
        dstDirName_ = g_strdup_printf("%s", dstDirName);
    } else {
        dstDirName_ = g_strdup_printf("%s/", dstDirName);
    }
    dstDirName_[strlen(dstDirName_)-1] = 0; //清除尾部 '/'

    dstDirName_p = g_strdup_printf("%s/lbp_pos/", dstDirName_);
    dstDirName_n = g_strdup_printf("%s/lbp_neg/", dstDirName_);

    if( !icvMkDir( dstDirName_ ) ) {
        fprintf( stderr, "Unable to create output file %s\n", dstDirName_);
        return 0;
    }
    if( !icvMkDir( dstDirName_p ) ) {
        fprintf( stderr, "Unable to create output file %s\n", dstDirName_p);
        return 0;
    }
    if( !icvMkDir( dstDirName_n ) ) {
        fprintf( stderr, "Unable to create output file %s\n", dstDirName_n);
        return 0;
    }

    // printf ("%s\n%s\n%s\n", dstDirName_, dstDirName_p, dstDirName_n);

    createTrainingSamples(dstDirName_, infoName, bgfileName, width, height);
    

    return 0;
}


static int icvMkDir( const char* filename )
{
    char path[PATH_MAX];
    char* p;
    int pos;

#ifdef _WIN32
    struct _stat st;
#else /* _WIN32 */
    struct stat st;
    mode_t mode;

    mode = 0755;
#endif /* _WIN32 */

    strcpy( path, filename );

    p = path;
    for( ; ; )
    {
        pos = (int)strcspn( p, "/\\" );

        if( pos == (int) strlen( p ) ) break;
        if( pos != 0 )
        {
            p[pos] = '\0';

#ifdef _WIN32
            if( p[pos-1] != ':' )
            {
                if( _stat( path, &st ) != 0 )
                {
                    if( _mkdir( path ) != 0 ) return 0;
                }
            }
#else /* _WIN32 */
            if( stat( path, &st ) != 0 )
            {
                if( mkdir( path, mode ) != 0 ) return 0;
            }
#endif /* _WIN32 */
        }

        p[pos] = '/';

        p += pos + 1;
    }

    return 1;
}
