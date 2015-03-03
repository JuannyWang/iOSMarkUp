//
//  Adapter.h
//  OpenCVTest
//
//  Created by MAXToooNG on 14-5-15.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#import <Foundation/Foundation.h>
class OpenCVMethods;
//class com_rcplatform_beauty_Beauty;
@interface Adapter : NSObject
{
    OpenCVMethods *openCVObject;
}
- (void)qudou:(const cv::Mat&)src dust:(cv::Mat&) dst R:(int) rp px:(float) xp py:(float) yp;
- (void)whiten:(const cv::Mat &)src dust:(cv::Mat &)dst params:(int) s;
- (UIImage *)matToImage:(const cv::Mat &)mat;
- (void)uiImageToMat:(UIImage *)image dstMat:(cv::Mat &)dst;
//- (void)microdermabrasion:(const cv::Mat &)src dstMat:(cv::Mat &)dst params:(int)s;
- (UIImage *)faceDect:(NSString *)path;
@end
