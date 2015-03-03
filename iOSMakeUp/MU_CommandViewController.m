//
//  MU_CommandViewController.m
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-30.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import "MU_CommandViewController.h"
#import "MU_EyesViewController.h"
#import "MU_HairViewController.h"

@interface MU_CommandViewController ()

- (IBAction)btnClick:(id)sender;

@end

@implementation MU_CommandViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.imageView.image = _select_image;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

}


- (IBAction)btnClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    switch (btn.tag) {
        case 0:
        {
            MU_EyesViewController *eyes = [[MU_EyesViewController alloc] initWithNibName:@"MU_EyesViewController" bundle:nil];
            eyes.eyesImage = _select_image;
            [self.navigationController pushViewController:eyes animated:YES];
        }
            break;
        case 1:
        {
            MU_HairViewController *hair = [[MU_HairViewController alloc] initWithNibName:@"MU_HairViewController" bundle:nil];
            [self.navigationController pushViewController:hair animated:YES];
        }
            break;
        default:
            break;
    }
}

@end
