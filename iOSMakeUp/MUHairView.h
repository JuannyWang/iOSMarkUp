//
//  MUHairView.h
//  iOSMakeUp
//
//  Created by gaoluyangrc on 15-3-9.
//  Copyright (c) 2015年 rcplatform. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MUHairView : UIView

@property (assign, nonatomic) BOOL isFill;
@property (assign, nonatomic) BOOL isClean;
@property (assign, nonatomic) CGFloat lineWidth;
@property (strong, nonatomic) UIColor *lineColor;
@property (assign ,nonatomic) CGFloat colorChangeValue;

- (void)cleanUpContext;
- (void)changeDrawwingColor;

@end
