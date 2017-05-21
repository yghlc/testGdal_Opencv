//
//  main.cpp
//  testGdal
//
//  Created by HuangLingcao on 5/21/17.
//  Copyright © 2017 HuangLingcao. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

#define FORMAT_CHAR_LENGTH 2048

#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()


// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;



cv::Mat ReadImageToCVMat(const string& filename,
                         const int height, const int width, const bool is_color,
                         int* img_height, int* img_width) {
    //LOG(INFO) << "ReadImageToCVMat, I'm here (lingcao) " << filename;
    cv::Mat cv_img;
    int cv_read_flag = (is_color ? CV_LOAD_IMAGE_COLOR :
                        CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat cv_img_origin = cv::imread(filename, cv_read_flag);
    if (!cv_img_origin.data) {
        //LOG(ERROR) << "Could not open or find file " << filename;
        return cv_img_origin;
    }
    
    if (height > 0 && width > 0) {
        cv::resize(cv_img_origin, cv_img, cv::Size(width, height));
    } else {
        cv_img = cv_img_origin;
    }
    
    if (img_height != NULL) {
        *img_height = cv_img.rows;
    }
    if (img_width != NULL) {
        *img_width = cv_img.cols;
    }
    
    return cv_img;
}

int main(int argc, const char * argv[])
{
    

    return 0;
}
