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

#include "CFileSystem.h"
CFileSystem filesys;

//#define FORMAT_CHAR_LENGTH 2048

#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()

#include "RSImage.h"



// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;



cv::Mat read_image_by_gdal(const string& filename)
{
    cv::Mat cv_img;

    CRSImage* img_obj = new CRSImage;
    if (!img_obj->Open(filename.c_str()))
    {
        filesys.OutputLogMessage("can not open image: " + filename);
        return cv_img;
    }

    int nband = 1 ;
    int width = img_obj->GetWidth();
    int height = img_obj->GetHeight();
    int band_count = img_obj->GetBandCount();
    
    cout<<"width: "<<width<<" height: "<<height<<" bandcount: "<<band_count<<endl;
    
    BYTE **dataPerband = new BYTE*[band_count] ;
    for(int i=0;i<band_count;i++)
    {
        dataPerband[i] = new BYTE[width*height];
        memset(dataPerband[i],0,sizeof(BYTE)*width*height);
    }
    
    img_obj->ReadImageDataToByte(dataPerband);
    
    //create cv::Mat
//    cv::Mat cvimg = cv:imread("");
//    cv::Mat cv_img(width,height,) ;
    // copy image data
    cv::Mat read_img(width,height,CV_8UC(band_count));
    cout<<"width: "<<read_img.cols<<" height: "<<read_img.rows<<" bandcount: "<<read_img.channels()<<endl;


    for(int i=0;i<band_count;i++)
    {
        delete []dataPerband[i];
    }
    delete []dataPerband;
    dataPerband = nullptr;

    return cv_img;

}


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
    string img_path = "/Users/huanglingcao/Data/aws_SpaceNet/voc_format/AOI_3_Paris_Train/8bit_image/RGB-PanSharpen_AOI_3_Paris_8bit_img997.tif";
    img_path = argv[1];
    read_image_by_gdal(img_path);

    return 0;
}
