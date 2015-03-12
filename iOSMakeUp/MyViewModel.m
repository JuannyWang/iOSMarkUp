/*
 作者: 羊羊羊
 描述: 
 时间:
 文件名: MyViewModel.m
 */

#import "MyViewModel.h"

@implementation MyViewModel

+ (id)viewModelWithColor:(UIColor *)color Path:(UIBezierPath *)path Width:(CGFloat)width
{
    MyViewModel *myViewModel = [[MyViewModel alloc] init];
   
    myViewModel.color = color;
    myViewModel.path = path;
    myViewModel.width = width;
    
    return myViewModel;
}

- (void)setAlphaValue:(CGFloat)alphaValue
{
    _alphaValue = alphaValue;
    _color = [_color colorWithAlphaComponent:_alphaValue];
}

@end
