//
//  MUHairView.m
//  iOSMakeUp
//
//  Created by gaoluyangrc on 15-3-9.
//  Copyright (c) 2015年 rcplatform. All rights reserved.
//

#import "MUHairView.h"
#import "MyViewModel.h"
@interface MUHairView()

@property (assign, nonatomic) CGMutablePathRef path;
@property (strong, nonatomic) NSMutableArray *pathArray;
@property (assign, nonatomic) BOOL isHavePath;

@end

@implementation MUHairView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        self.backgroundColor = [UIColor clearColor];
        _lineColor = [UIColor redColor];
        _lineWidth = 4.0;
        _isFill = YES;
    }
    
    return self;
}

- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    [self drawView:context];
}

- (void)drawView:(CGContextRef)context
{
    for (MyViewModel *myViewModel in _pathArray)
    {
        CGContextAddPath(context, myViewModel.path.CGPath);
        [myViewModel.color set];
        if (myViewModel.isClean)
        {
            CGContextSetBlendMode(context, kCGBlendModeClear);
            //这个颜色随意，因为kCGBlendModeClear
            [[UIColor whiteColor] setStroke];
        }
        else
        {
            CGContextSetBlendMode(context, kCGBlendModeCopy);
        }
        CGContextSetLineWidth(context, myViewModel.width);
        CGContextSetLineCap(context, kCGLineCapRound);
        CGContextDrawPath(context, myViewModel.isFill?kCGPathFill:kCGPathStroke);
    }
    
    if (_isHavePath)
    {
        CGContextAddPath(context, _path);
        [_lineColor set];
        CGContextSetLineWidth(context, _lineWidth);
        CGContextSetLineCap(context, kCGLineCapRound);
        if (_isClean)
        {
            CGContextSetBlendMode(context, kCGBlendModeClear);
        }
        else
        {
            CGContextSetBlendMode(context, kCGBlendModeCopy);
        }
        CGContextDrawPath(context, kCGPathStroke);
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint point = [touch locationInView:self];
    _isHavePath = YES;
    _path = CGPathCreateMutable();
    CGPathMoveToPoint(_path, NULL, point.x, point.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    CGPathAddLineToPoint(_path, NULL, location.x, location.y);
    [self setNeedsDisplay];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (_pathArray == nil)
    {
        _pathArray = [NSMutableArray array];
    }
    
    UIBezierPath *path = [UIBezierPath bezierPathWithCGPath:_path];
    MyViewModel *myViewModel = [MyViewModel viewModelWithColor:_lineColor Path:path Width:_lineWidth];
    myViewModel.isFill = _isFill;
    myViewModel.isClean = _isClean;
    [_pathArray addObject:myViewModel];
    
    CGPathRelease(_path);
    _isHavePath = NO;
}

- (void)setColorChangeValue:(CGFloat)colorChangeValue
{
    _colorChangeValue = colorChangeValue;
    for (MyViewModel *model in _pathArray)
    {
        if (!model.isClean)
        {
            model.alphaValue = colorChangeValue;
        }
    }
    [self setNeedsDisplay];
}

- (void)cleanUpContext
{
    [_pathArray removeAllObjects];
    [self setNeedsDisplay];
}

- (void)changeDrawwingColor
{
    self.lineColor = [UIColor purpleColor];
    for (MyViewModel *model in _pathArray)
    {
        if (!model.isClean)
        {
            model.color = _lineColor;
        }
    }
    [self setNeedsDisplay];
}

@end
