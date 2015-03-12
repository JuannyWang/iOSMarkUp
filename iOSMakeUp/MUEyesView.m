//
//  MUEyesView.m
//  iOSMakeUp
//
//  Created by gaoluyangrc on 15-3-11.
//  Copyright (c) 2015年 rcplatform. All rights reserved.
//

#import "MUEyesView.h"

@implementation MUEyesView

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    [self drawEyePoints:context];
    //放大后显示
    if (_isScale)
    {
        [self drawScaleimage:context];
    }
    
    if (_isShowEyeball || _isAdjust)
    {
        [self drawEyeBall:context];
    }
    
    CFRelease(_path);
}

- (void)drawEyeBall:(CGContextRef)context
{
    eyeballRect = CGRectMake(eyeballPoint.x - 10.f, eyeballPoint.y - 10.f, 20.f, 20.f);
    [_eyeballImage drawInRect:eyeballRect];
}

- (void)drawScaleimage:(CGContextRef)context
{
    CGContextSetFillColorWithColor(UIGraphicsGetCurrentContext(), [UIColor lightGrayColor].CGColor);
    CGContextAddPath(context, _path);
    if (!_isAdjust)
    {
        if (!_isShowEyeball)
        {
            CGContextFillPath(context);
        }
        else
        {
            CGContextClip(context);
        }
    }
}

- (void)drawEyePoints:(CGContextRef)context
{
    int i = 0;
    _path = CGPathCreateMutable();
    for (NSDictionary *pointDic in _eyePoints)
    {
        float x = [pointDic[@"x"] floatValue] * 0.01 * _imageSize.width/(_imageSize.width/320);
        float y = [pointDic[@"y"] floatValue] * 0.01 * _imageSize.height/(_imageSize.height/320.f);
        //第五个是眼珠中心点
        if (i != 4)
        {
            if (!_isShowEyeball || !_isAdjust)
            {
                CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);
                CGContextAddArc(context, x, y, 2, 0, 2 * M_PI, 0);
                CGContextFillPath(context);
            }
            
            if (i == 0)
            {
                CGPathMoveToPoint(_path, NULL, x, y);
            }
            else
            {
                CGPathAddLineToPoint(_path, NULL, x, y);
            }
        }
        else
        {
            eyeballPoint = CGPointMake(x, y);
        }
        i++;
    }
}

- (void)setIsShowEyeball:(BOOL)isShowEyeball
{
    _isShowEyeball = isShowEyeball;
    [self setNeedsDisplay];
}

- (void)setIsAdjust:(BOOL)isAdjust
{
    _isAdjust = isAdjust;
    [self setNeedsDisplay];
}

- (void)setEyePoints:(NSMutableArray *)eyePoints
{
    _eyePoints = eyePoints;
    [self setNeedsDisplay];
}

- (void)setIsScale:(BOOL)isScale
{
    _isScale = isScale;
    [self setNeedsDisplay];
}

- (void)movePointNearly:(CGPoint)locationPoint movePoint:(CGPoint)movePoint
{
    NSUInteger count = [_eyePoints count] - 1;
    int i = 0;
    if (_space == 0.0)
    {
        while (i < count)
        {
            float x = [_eyePoints[i][@"x"] floatValue] * 0.01 * _imageSize.width/(_imageSize.width/320);
            float y = [_eyePoints[i][@"y"] floatValue] * 0.01 * _imageSize.height/(_imageSize.height/320);
            
            CGPoint point = CGPointMake(x, y);
            //计算斜边长度
            float nextSpace = hypot(ABS(point.x - locationPoint.x), ABS(point.y - locationPoint.y));
            if (i == 0)
            {
                _space = nextSpace;
                index = 0;
            }
            
            if (_space > nextSpace)
            {
                _space = nextSpace;
                index = i;
            }
            i++;
        }
    }
    
    float x = [_eyePoints[index][@"x"] floatValue] * 0.01 * _imageSize.width/(_imageSize.width/320);
    float y = [_eyePoints[index][@"y"] floatValue] * 0.01 * _imageSize.height/(_imageSize.height/320);
    x += movePoint.x;
    y += movePoint.y;
    x = x * 100 * (_imageSize.width/320) /_imageSize.width;
    y = y * 100 * (_imageSize.height/320) /_imageSize.height;
    
    NSDictionary *newPointDic = @{@"x":[NSNumber numberWithFloat:x],@"y":[NSNumber numberWithFloat:y]};
    [_eyePoints replaceObjectAtIndex:index withObject:newPointDic];
    [self setNeedsDisplay];
}

@end
