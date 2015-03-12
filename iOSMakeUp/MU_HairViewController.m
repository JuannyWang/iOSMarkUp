//
//  MU_HairViewController.m
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-30.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import "MU_HairViewController.h"
#import "MUHairView.h"

@interface MU_HairViewController ()
{
    MUHairView *drawHairView;
}

- (IBAction)buttonClick:(id)sender;

@end

@implementation MU_HairViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _hairImageView.image = _hairImage;
    _hairImageView.userInteractionEnabled = YES;
    drawHairView = [[MUHairView alloc] initWithFrame:_hairImageView.frame];
    [self.view addSubview:drawHairView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)setHairImage:(UIImage *)hairImage
{
    _hairImage = hairImage;
}

- (IBAction)buttonClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    if (btn.tag == 0)
    {
        UIImagePickerController *imagePickerController = [[UIImagePickerController alloc] init];
        imagePickerController.allowsEditing = NO;
        imagePickerController.modalPresentationStyle = UIModalPresentationCurrentContext;
        imagePickerController.delegate = self;
        imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        
        [self.view.window.rootViewController presentViewController:imagePickerController animated:YES completion:nil];
    }
    else if (btn.tag == 1)
    {
        [drawHairView setNeedsDisplay];
    }
    else if (btn.tag == 2)
    {
        drawHairView.isFill = NO;
    }
    else if (btn.tag == 3)
    {
        drawHairView.lineColor = [UIColor clearColor];
        drawHairView.isClean = YES;
        drawHairView.isFill = NO;
    }
    else if (btn.tag == 4)
    {
        [drawHairView cleanUpContext];
    }
    else if (btn.tag == 5)
    {
        [drawHairView changeDrawwingColor];
    }
}

- (IBAction)sliderValueChange:(id)sender
{
    UISlider *slider = (UISlider *)sender;
    drawHairView.colorChangeValue = slider.value;
}

@end
