//
//  MU_EyesViewController.h
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-30.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface MU_EyesViewController : UIViewController

@property (strong ,nonatomic) UIImage *eyesImage;
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
@property (weak, nonatomic) IBOutlet UIView *backView;

@end
