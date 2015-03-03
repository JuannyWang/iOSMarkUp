//
//  Adapter.m
//  OpenCVTest
//
//  Created by MAXToooNG on 14-5-15.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#import "Adapter.h"
#include "OpenCVMethods.h"
//#include "com_rcplatform_beauty_Beauty.h"
@implementation Adapter
- (id) init {
    if (self = [super init]) {
        openCVObject = new OpenCVMethods();
    }
    
    return self;
}

- (void) dealloc {
    if (openCVObject != NULL) {
        delete openCVObject;
        openCVObject = NULL;
    }
}

- (void)qudou:(const cv::Mat &)src dust:(cv::Mat &)dst R:(int)rp px:(float)xp py:(float)yp{
    openCVObject->qd(src, dst, rp, xp, yp);
}

- (void)whiten:(const cv::Mat &)src dust:(cv::Mat &)dst params:(int) s{
    openCVObject->whitening(src, dst, s);
}

- (UIImage *)matToImage:(const cv::Mat &)mat{
    return openCVObject->MattoUIImage(mat);
}

- (void)uiImageToMat:(UIImage *)image dstMat:(cv::Mat &)dst{
    openCVObject->UIImagetoMat(image, dst);
}

- (UIImage *)faceDect:(NSString *)path
{
    return openCVObject->faceDect(path);
}

@end
