//
//  MUEyesView.h
//  iOSMakeUp
//
//  Created by gaoluyangrc on 15-3-11.
//  Copyright (c) 2015年 rcplatform. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MUEyesView : UIView
{
    NSInteger index;
    CGPoint eyeballPoint;
    CGRect eyeballRect;
}
@property (nonatomic ,strong) NSMutableArray *eyePoints;
@property (nonatomic ,assign) CGSize imageSize;
@property (nonatomic ,assign) BOOL isScale;
@property (assign ,nonatomic) float space;
@property (nonatomic ,strong) UIImage *eyeballImage;
@property (nonatomic ,assign) BOOL isShowEyeball;
@property (nonatomic ,assign) BOOL isAdjust;
@property (nonatomic ,assign) CGMutablePathRef path;

- (void)movePointNearly:(CGPoint)locationPoint movePoint:(CGPoint)movePoint;

@end
