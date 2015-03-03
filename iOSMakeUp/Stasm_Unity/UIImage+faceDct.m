//
//  UIImage+faceDct.m
//  BeautyCameraDemo
//
//  Created by MAXToooNG on 14-6-23.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#import "UIImage+faceDct.h"

@implementation UIImage (faceDct)

- (UIImage *) opencvFaceDetect:(UIImage *)originalImage {
    cvSetErrMode(CV_ErrModeParent);
    
    IplImage *image = [self CreateIplImageFromUIImage:originalImage];
    
    // Scaling down
    IplImage *small_image = cvCreateImage(cvSize(image->width/2,image->height/2), IPL_DEPTH_8U, 3);
    cvPyrDown(image, small_image, CV_GAUSSIAN_5x5);
    int scale = 2;
    
    // Load XML
    NSString *path = [[NSBundle mainBundle] pathForResource:@"haarcascade_frontalface_default" ofType:@"xml"];
    CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad([path cStringUsingEncoding:NSASCIIStringEncoding], NULL, NULL, NULL);
    CvMemStorage* storage = cvCreateMemStorage(0);
    
    // Detect faces and draw rectangle on them
    CvSeq* faces = cvHaarDetectObjects(small_image, cascade, storage, 1.2f, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(20, 20), cvSize(100, 100));
    cvReleaseImage(&small_image);
    
    NSLog(@"found %d faces in image", faces->total);
    
    // Create canvas to show the results
    CGImageRef imageRef = originalImage.CGImage;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef contextRef = CGBitmapContextCreate(NULL, originalImage.size.width, originalImage.size.height, 8, originalImage.size.width * 4,
                                                    colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault);
    CGContextDrawImage(contextRef, CGRectMake(0, 45, originalImage.size.width, originalImage.size.height), imageRef);
    
    CGContextSetLineWidth(contextRef, 4);
    CGContextSetRGBStrokeColor(contextRef, 0.0, 0.0, 1.0, 0.5);
    
    // Draw results on the iamge
    for(int i = 0; i < faces->total; i++) { 
        @autoreleasepool {
            // Calc the rect of faces
            CvRect cvrect = *(CvRect*)cvGetSeqElem(faces, i);
            CGRect face_rect = CGContextConvertRectToDeviceSpace(contextRef,
                                                                 CGRectMake(cvrect.x * scale, cvrect.y * scale, cvrect.width * scale, cvrect.height * scale));
            CGContextStrokeRect(contextRef, face_rect);
        }
}
    
    UIImage *returnImage = [UIImage imageWithCGImage:CGBitmapContextCreateImage(contextRef)];
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
    
    cvReleaseMemStorage(&storage);
    cvReleaseHaarClassifierCascade(&cascade);
    
    return returnImage;
}

- (IplImage *)CreateIplImageFromUIImage:(UIImage *)image {
    // Getting CGImage from UIImage
    CGImageRef imageRef = image.CGImage;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    // Creating temporal IplImage for drawing
    IplImage *iplimage = cvCreateImage(
                                       cvSize(image.size.width,image.size.height), IPL_DEPTH_8U, 4
                                       );
    // Creating CGContext for temporal IplImage
    CGContextRef contextRef = CGBitmapContextCreate(
                                                    iplimage->imageData, iplimage->width, iplimage->height,
                                                    iplimage->depth, iplimage->widthStep,
                                                    colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault
                                                    );
    // Drawing CGImage to CGContext
    CGContextDrawImage(
                       contextRef,
                       CGRectMake(0, 0, image.size.width, image.size.height),
                       imageRef
                       );
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
    
    // Creating result IplImage
    IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, 3);
    cvCvtColor(iplimage, ret, CV_RGBA2BGR);
    cvReleaseImage(&iplimage);
    
    return ret;
}

//使用玩iplimage之后，别忘记释放内存。cvreleaseImage().
//
//从iplimage创建一张UIImage
//
//代码如下：

// NOTE You should convert color mode as RGB before passing to this function
- (UIImage *)UIImageFromIplImage:(IplImage *)image {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    // Allocating the buffer for CGImage
    NSData *data =
    [NSData dataWithBytes:image->imageData length:image->imageSize];
    CGDataProviderRef provider =
    CGDataProviderCreateWithCFData((CFDataRef)data);
    // Creating CGImage from chunk of IplImage
    CGImageRef imageRef = CGImageCreate(
                                        image->width, image->height,
                                        image->depth, image->depth * image->nChannels, image->widthStep,
                                        colorSpace, kCGImageAlphaNone|kCGBitmapByteOrderDefault,
                                        provider, NULL, false, kCGRenderingIntentDefault
                                        );
    // Getting UIImage from CGImage
    UIImage *ret = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    return ret;
}

@end
