//
//  FTF_Global.h
//  iOSChangeFace
//
//  Created by gaoluyangrc on 14-7-26.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//


#import <Foundation/Foundation.h>

@interface FTF_Global : NSObject

@property (nonatomic ,assign) float smallValue;
@property (nonatomic ,strong) NSMutableArray *eyeballsArray;
//广告条
@property (nonatomic, strong) UIView *bannerView;
//@property (assign, nonatomic) CGSize originalImageSize;

+ (instancetype)shareGlobal;

@end
