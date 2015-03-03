//
//  FTF_Global.m
//  iOSChangeFace
//
//  Created by gaoluyangrc on 14-7-26.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import "FTF_Global.h"

@implementation FTF_Global

+ (instancetype)shareGlobal
{
    static FTF_Global *_public_Global;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _public_Global = [[[self class] alloc] init];
        
    });
    
    return _public_Global;
}


@end
