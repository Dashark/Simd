/*
*  The use examples of Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2017 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* In order to be enable of this example for Visual Studio 2015 you have to rename file 'Ocv.prop.default' into 'Ocv.prop' and set there correct paths to OpenCV.
*/
#include <iostream>
#include <string>

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"

typedef Simd::View<Simd::Allocator> View;
typedef Simd::Detection<Simd::Allocator> Detection;

int main(int argc, char * argv[])
{/*
*  The use examples of Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2017 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* In order to be enable of this example for Visual Studio 2015 you have to rename file 'Ocv.prop.default' into 'Ocv.prop' and set there correct paths to OpenCV.
*/
#include <iostream>
#include <string>

#include "Simd/SimdDetection.hpp"
#include "Simd/SimdDrawing.hpp"

#include <glib.h>

typedef Simd::View<Simd::Allocator> View;
typedef Simd::Detection<Simd::Allocator> Detection;

int main(int argc, char * argv[])
{
    gint64 start, end;
    
    Detection detection;
    // detection.Load("../../data/cascade/haar_face_1.xml");
    detection.Load("../../data/cascade/lbp_face.xml");

    std::string img_path = "../../data/image/face/face2.jpg";
    Detection::View image;
    if (!image.Load(img_path)) {
        std::cout << "Can't load test image '" << img_path << "' !\n";
        return 0;
    }

    detection.Init(image.Size(), 1.2, image.Size() / 20);

    start = g_get_real_time ();
    Detection::Objects objects;
    detection.Detect(image, objects);

    end = g_get_real_time ();

    std::cout << "Detect num = " << objects.size() << std::endl;
    std::cout << "elapse " << end - start << " us" <<std::endl;

    return 0;
}



    Detection detection;
    detection.Load("../../data/cascade/haar_face_0.xml");

    std::string img_path = "../../data/image/face/lena.pgm";
    Detection::View image;
    if (!image.Load(img_path)) {
        std::cout << "Can't load test image '" << img_path << "' !";
        return 0;
    }

    detection.Init(image.Size(), 1.2, image.Size() / 20);

    Detection::Objects objects;
    detection.Detect(image, objects);

    std::cout << "Detect num = " << objects.size() << std::endl;

    return 0;
}



