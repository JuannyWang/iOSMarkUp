// stasm_test.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
//#include <iostream>
//#include "stasm_lib.h"
//using namespace std;
//using namespace cv;
//
//
//
//
//int faceDect()
//{
//	static const char* path = "D:/pic/57.jpg";
//    const char *mainPath = [[NSBundle mainBundle].bundlePath UTF8String];
//	if (!stasm_init(mainPath, 0)){
//		printf("stasm_init failed %s\n", stasm_lasterr());
//		return 0;
//	}
//	Mat_<unsigned char> img(cv::imread(path,0));
//	int minwidth = 20;
//	Mat srcimg = imread(path, -1);
//	//cv::imshow("stasm minima2222", srcimg);
//	Mat grayimg;
//	cvtColor(srcimg, grayimg, CV_RGB2GRAY);
//	
//	if (!stasm_open_image((const char*)grayimg.data, grayimg.cols, grayimg.rows, path, 0, minwidth)){
//		printf("stasm_open_image failed %s\n", stasm_lasterr());
//		return 0;
//	}
//	int foundface = 0;
//	float landmarks[2 * stasm_NLANDMARKS];
//	if (!stasm_search_auto(&foundface, landmarks)){
//		printf("stasm_search_auto failed %s\n", stasm_lasterr());
//		return 0;
//	}
//    
//	//IplImage *src;
//    
//    //src=&IplImage(srcimg);
//    
//	if (foundface < 1){
//		printf("not find face. \n");
//		return 0;
//	}else{
//		stasm_force_points_into_image(landmarks, img.cols, img.rows);
//		for (int i = 0; i < stasm_NLANDMARKS; i++){
//            cv::Point center = cv::Point(landmarks[i*2],landmarks[i*2+1]);
//			cv::circle(srcimg, center, 2, Scalar(0,128,0),-1);
//			//printf("%d:%d=== landmarks1:%f landmarks2:%f \n", i*2, (i*2+1), landmarks[i*2], landmarks[i*2+1]);
//            
//			/*char str[10];
//             sprintf(str,"%d",i);;
//             cvText(src, str, landmarks[i*2], landmarks[i*2+1]);
//             */
//		}
//	}
//	
//	//cvNamedWindow("图像显示",CV_WND_PROP_FULLSCREEN);
//	//cvShowImage("图像显示",src);
//	cvNamedWindow("图像显示",1);
//	cv::imshow("图像显示", srcimg);
//	cv::waitKey();
//    
//    
//	return 0;
//}
//
//
//void cvText(IplImage* img, const char* text, int x, int y)
//{
//    CvFont font;
//    
//    double hscale = 1.0;
//    double vscale = 1.0;
//    int linewidth = 2;
//    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX |CV_FONT_ITALIC,hscale,vscale,0,linewidth);
//    
//    CvScalar textColor =cvScalar(0,255,255);
//    CvPoint textPos =cvPoint(x, y);
//    
//    cvPutText(img, text, textPos, &font,textColor);
//}

