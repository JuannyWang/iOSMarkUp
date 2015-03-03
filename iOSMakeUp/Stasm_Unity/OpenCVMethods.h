//
//  OpenCVMethods.h
//  BeautyCameraDemo
//
//  Created by MAXToooNG on 14-5-16.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#ifndef __BeautyCameraDemo__OpenCVMethods__
#define __BeautyCameraDemo__OpenCVMethods__

#include <iostream>
class OpenCVMethods{
    public :
    OpenCVMethods();
    virtual ~OpenCVMethods();
    static void whitening(const cv::Mat& src, cv::Mat& dst, int s);
    static void qd(const cv::Mat&src, cv::Mat& dst,int rp,float xp,float yp);
    static UIImage * MattoUIImage(const cv::Mat &mat);
    static void UIImagetoMat(const UIImage* image, cv::Mat& m);
    void Microdermabrasion(const cv::Mat& src, cv::Mat& dst, int s);
    UIImage * faceDect(NSString *imagePath);
    private :
    static void skinDetection(const cv::Mat& input_image, cv::Mat& output_mask, cv::Mat& output_image);
    static void meibai(cv::Mat& src, cv::Mat& dst_img, int value);

};
#endif /* defined(__BeautyCameraDemo__OpenCVMethods__) */
