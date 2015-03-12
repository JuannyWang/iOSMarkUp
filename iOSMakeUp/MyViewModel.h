/*
 作者: 羊羊羊
 描述: 
 时间:
 文件名: MyViewModel.h
 */

#import <Foundation/Foundation.h>

@interface MyViewModel : NSObject

+ (id)viewModelWithColor:(UIColor *)color Path:(UIBezierPath *)path Width:(CGFloat)width;

@property (strong, nonatomic) UIColor *color;

@property (strong, nonatomic) UIBezierPath *path;

@property (assign, nonatomic) CGFloat width;

@property (assign, nonatomic) BOOL isFill;

@property (assign, nonatomic) BOOL isClean;

@property (assign, nonatomic) CGFloat alphaValue;

@end