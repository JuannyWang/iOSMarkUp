//
//  MU_HairViewController.h
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-30.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MU_HairViewController : UIViewController<UIImagePickerControllerDelegate,UINavigationControllerDelegate>

@property (weak, nonatomic) IBOutlet UIImageView *hairImageView;
@property (strong ,nonatomic) UIImage *hairImage;
@property (weak, nonatomic) IBOutlet UISlider *colorSlider;
- (IBAction)sliderValueChange:(id)sender;

@end
