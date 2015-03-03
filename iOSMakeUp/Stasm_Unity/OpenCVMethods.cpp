//
//  OpenCVMethods.cpp
//  BeautyCameraDemo
//
//  Created by MAXToooNG on 14-5-16.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#include "OpenCVMethods.h"
#include "ImgInpaintFMM.h"
#include "stdafx.h"
#include <iostream>
#include "stasm_lib.h"
#import "FTF_Global.h"

OpenCVMethods::OpenCVMethods(){}
OpenCVMethods::~OpenCVMethods(){}

#pragma mark - 
#pragma mark - 祛痘
void OpenCVMethods:: qd (const cv::Mat& mat_in,  cv::Mat& mat_out,int rP ,float xP, float yP){
    cv::Mat mat_tmp,mat_out_tmp;
    //     mat_in.create(src.size().height, src.size().width, CV_8UC4);
	mat_out.create(mat_in.size().height, mat_in.size().width, CV_8UC4);
    UIImage *image =  MattoUIImage(mat_in);
    NSString  *pngPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/mat_in.png"];
    // Write image to PNG
    [UIImagePNGRepresentation(image) writeToFile:pngPath atomically:YES];
	//CV_BGR2BGRA°¢CV_RGB2BGRA°¢CV_BGRA2RGBA°¢CV_BGR2BGRA°¢CV_BGRA2BGR
	cvtColor(mat_in, mat_tmp, CV_RGBA2RGB, 0);
    //     mat_in.copyTo(mat_tmp);
	int channel = mat_tmp.channels();
    cv::Size size = mat_tmp.size();
    //	LOGE("======================channel: %d \n", channel);
	int w = size.width;
	int h = size.height;
    
    RGB_Data ** ImgData = new RGB_Data*[h];
	for(int y=0;y<h;y++){
		ImgData[y]=new RGB_Data[w];				//创建一个数组来保存图片上的像素数据
	}
	
	for(int i = 0; i < h; i++ ){
		if (channel == 1){
			for(int j = 0; j < w; j += 1){
                cv::Vec3b vec = mat_tmp.at<cv::Vec3b>(i, j);
				//Vec3b[0]----B
				//Vec3b[1]----G
				//Vec3b[2]----R
				int r1 = vec[0];
				int r2 = vec[1];
				int r3 = vec[2];
				//用白色标识修复区域
                if ((i - yP) * (i - yP) + (j/3 - xP) * (j/3 - xP) <= rP *rP) {
                    r1 = 255;
                    r2 = 255;
                    r3 = 255;
                }
//				if (i >(yP-rP) && i <(yP+rP) && j>(xP-rP) && j <(xP+rP)){
//					r1=255;
//					r2=255;
//					r3=255;
//				}
                ImgData[i][j].b = r1;
				ImgData[i][j].g = r2;
				ImgData[i][j].r = r3;
				mat_tmp.at<cv::Vec3b>(i, j) = cv::Vec3b(r1, r2, r3);
			}
		}else{
			for(int j = 0; j < size.width*3; j += 3){
                cv::Vec3b vec = mat_tmp.at<cv::Vec3b>(i, j/3);
				//Vec3b[0]----B
				//Vec3b[1]----G
				//Vec3b[2]----R
				int r1 = vec[0];
				int r2 = vec[1];
				int r3 = vec[2];
				//用白色标识修复区域
                if ((i - yP) * (i - yP) + (j/3 - xP) * (j/3 - xP) <= rP *rP) {
                    r1 = 255;
                    r2 = 255;
                    r3 = 255;
                }
                
//				if (i >(yP-rP) && i <(yP+rP) && (j/3)>(xP-rP) && (j/3) <(xP+rP)){
//					r1=255;
//					r2=255;
//					r3=255;
//				}
				ImgData[i][j/3].b = r1;
				ImgData[i][j/3].g = r2;
				ImgData[i][j/3].r = r3;
				mat_tmp.at<cv::Vec3b>(i, j/3) = cv::Vec3b(r1, r2, r3);
			}
		}
	}
	
	//修复操作
	ImgInpaintFMM::ImgInpaintFMM(ImgData,h,w);
    //修复后的颜色值回填
    for(int i = 0; i < size.height; i++ ){
		if (channel == 1){
			for(int j = 0; j < size.width; j += 1){
				int r1 = ImgData[i][j].b;
				int r2 = ImgData[i][j].g;
				int r3 = ImgData[i][j].r;
				mat_tmp.at<cv::Vec3b>(i, j) = cv::Vec3b(r1, r2, r3);
			}
		}else{
			for(int j = 0; j < size.width*3; j += 3){
				int r1 = ImgData[i][j/3].b;
				int r2 = ImgData[i][j/3].g;
				int r3 = ImgData[i][j/3].r;
				mat_tmp.at<cv::Vec3b>(i, j/3) = cv::Vec3b(r1, r2, r3);
			}
		}
	}
	mat_tmp.copyTo(mat_out);
    
}

#pragma mark - 
#pragma mark - 美白
void OpenCVMethods::whitening(const cv::Mat& src, cv::Mat& dst, int s){
    cv::Mat dst_img;
    cv::Mat dst_mask;
	dst = src.clone();
    skinDetection(dst, dst_mask, dst_img);
    meibai(dst, dst_img, s);
}

#pragma mark -
#pragma mark - 皮肤检测
/**
 皮肤检测(椭圆模型)
 **/
void OpenCVMethods::skinDetection(const cv::Mat& input_image, cv::Mat& output_mask, cv::Mat& output_image){
    /*椭圆皮肤模型*/
    cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
    ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2), 43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
    
    cv::Mat ycrcb_image;
	//创建空图像（一通道）
    output_mask = cv::Mat::zeros(input_image.size(), CV_8UC1);
	//转换成到YCrCb空间
    
    cvtColor(input_image, ycrcb_image, CV_RGB2YCrCb);
    int n = ycrcb_image.channels();
    NSLog(@" n  = %d",n);
    NSLog(@"input.channels  = %d",input_image.channels());
    //利用椭圆皮肤模型进行皮肤检测
    for(int i = 0; i < input_image.cols; i++)
        for(int j = 0; j < input_image.rows; j++){
            cv::Vec3b ycrcb = ycrcb_image.at<cv::Vec3b>(j, i);
            if(skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0)
                output_mask.at<uchar>(j, i) = 255;
        }
    input_image.copyTo(output_image, output_mask);
}

/**
 **美白实现
 **/
void OpenCVMethods::meibai(cv::Mat& src, cv::Mat& dst_img, int value){
	
    cv::Size size = src.size();
    NSLog(@"size.width = %d" ,size.width);
	int cn = src.channels();
	//LOGE("-----------cn:%d \n" ,cn);
	int i,j;
	for( i = 0; i < size.height; i++ ){
		if( cn == 1 )
		{
			for( j = 0; j < size.width; j++ )
			{
                cv::Vec3b vec = src.at<cv::Vec3b>(i, j);
				if ((vec[0] + value) <=255){
					vec[0] = vec[0] + value;
				}else{
					vec[0] = 255;
				}
			}
		}else{
			for( j = 0; j < size.width*4; j += 3 )
			{
                cv::Vec3b vec = src.at<cv::Vec3b>(i, j/3);
				int r1 = vec[0];
				int r2 = vec[1];
				int r3 = vec[2];
                
				if ((r1 + value) <= 255){
					r1 = r1 + value;
				}else{
					r1 = 255;
				}
				if ((r2 + value) <= 255){
					r2 = r2 + value;
				}else{
					r2 = 255;
				}
				if ((r3 + value) <= 255){
					r3 = r3 + value;
				}else{
					r3 = 255;
				}
				src.at<cv::Vec3b>(i, j/3) = cv::Vec3b(r1, r2, r3);
			}
		}
	}
}


# pragma mark - 
# pragma mark - Mat to UIImage
UIImage* OpenCVMethods:: MattoUIImage( const cv::Mat& m)
{
    //CV_ASSERT(m.depth() == CV_8U);
    if (m.depth() !=CV_8U)
        return nil;
    NSData* data = [NSData dataWithBytes:m.data length:m.elemSize()*m.total()];
    CGColorSpaceRef colorSpace = m.channels() ==1 ? CGColorSpaceCreateDeviceGray():CGColorSpaceCreateDeviceRGB();
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    // Creating CGImage from cv::Mat®
    CGImageRef imageRef = CGImageCreate(m.cols, m.rows, m.elemSize1()*8, m.elemSize()*8, m.step[0], colorSpace, kCGImageAlphaNone|kCGBitmapByteOrderDefault, provider, NULL, false, kCGRenderingIntentDefault);
    UIImage* finalImage = [UIImage imageWithCGImage:imageRef];
    NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/result.jpg"];
    [UIImageJPEGRepresentation(finalImage, 1) writeToFile:path atomically:YES];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    return  finalImage;
}

#pragma mark - 
#pragma mark - UIImage to Mat
void OpenCVMethods::UIImagetoMat(const UIImage* image, cv::Mat& m)
{
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = image.size.width;
    CGFloat rows = image.size.height;
    m.create(rows, cols, CV_8UC4);
    
    CGContextRef contextRef = CGBitmapContextCreate(m.data, cols,rows, 8, m.step[0], colorSpace, kCGImageAlphaNoneSkipLast|kCGBitmapByteOrderDefault);
    
    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
}

using namespace std;
using namespace cv;

UIImage * OpenCVMethods::faceDect(NSString *imagePath)
{
    //待处理图片的地址
    const char* path = [imagePath UTF8String];
    const char *mainPath =[[NSHomeDirectory() stringByAppendingPathComponent:@"Caches/data/"] UTF8String];
	if (!stasm_init(mainPath, 0))
    {
		printf("stasm_init failed %s\n", stasm_lasterr());
		return 0;
	}
    cv::Mat_<unsigned char> img(cv::imread(path,0));
	int minwidth = 20;
    cv::Mat srcimg1 = imread(path, -1);
    cv::Mat grayimg;
    cv::Mat srcimg;
    cvtColor(srcimg1, srcimg, CV_BGR2RGB);
	cvtColor(srcimg, grayimg, CV_RGB2GRAY);
	NSString *tmpPath = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/1.jpg"];
    [UIImageJPEGRepresentation(MattoUIImage(grayimg), 1) writeToFile:tmpPath atomically:YES];
	if (!stasm_open_image((const char*)grayimg.data, grayimg.cols, grayimg.rows, path, 0, minwidth)){
		printf("stasm_open_image failed %s\n", stasm_lasterr());
		return 0;
	}
    
	int foundface = 0;
	float landmarks[2 * stasm_NLANDMARKS];
   
	if (!stasm_search_auto(&foundface, landmarks)){
		printf("stasm_search_auto failed %s\n", stasm_lasterr());
		return 0;
	}
    
    NSLog(@"foundface = %d",foundface);
	if (foundface < 1){
		printf("not find face. \n");
		return 0;
	}else{
		stasm_force_points_into_image(landmarks, img.cols, img.rows);
        NSMutableArray *array = [NSMutableArray arrayWithCapacity:0];
        NSMutableArray *eyeballPoints = [NSMutableArray arrayWithCapacity:0];
		for (int i = 0; i < stasm_NLANDMARKS; i++){
            if (i >= 30 && i <= 47 && i%2 == 0 && i != 38)
            {
                CGPoint point = CGPointMake(landmarks[i*2], landmarks[i*2+1]);
                [array addObject:NSStringFromCGPoint(point)];
            }
            else if (i == 38 || i == 39)
            {
                CGPoint point = CGPointMake(landmarks[i*2], landmarks[i*2+1]);
                [eyeballPoints addObject:NSStringFromCGPoint(point)];
            }
            [FTF_Global shareGlobal].eyeballsArray = eyeballPoints;
		}
        [[NSNotificationCenter defaultCenter] postNotificationName:@"reaciveEyesPoints" object:array];
	}

	return  OpenCVMethods::MattoUIImage(srcimg);
}


void cvText(IplImage* img, const char* text, int x, int y)
{
    CvFont font;
    
    double hscale = 1.0;
    double vscale = 1.0;
    int linewidth = 2;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX |CV_FONT_ITALIC,hscale,vscale,0,linewidth);
    
    CvScalar textColor =cvScalar(0,255,255);
    CvPoint textPos =cvPoint(x, y);
    
    cvPutText(img, text, textPos, &font,textColor);
}


