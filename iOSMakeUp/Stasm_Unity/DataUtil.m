//
//  DataUtil.m
//  BeautyCameraDemo
//
//  Created by MAXToooNG on 14-5-16.
//  Copyright (c) 2014年 MAXToooNG. All rights reserved.
//

#import "DataUtil.h"

@implementation DataUtil
static DataUtil *dataUtil = nil;
+ (DataUtil *)defaultDateUtil{
    if (dataUtil == nil) {
        dataUtil = [[DataUtil alloc] init];
    }
    return dataUtil;
}

@end
