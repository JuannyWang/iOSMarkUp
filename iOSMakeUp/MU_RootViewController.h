//
//  MU_RootViewController.h
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-29.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MU_RootViewController : UIViewController <UIImagePickerControllerDelegate ,UINavigationControllerDelegate>
{
    UIImagePickerController *imagePicker;
    UIImage *select_Image;
}

@end
