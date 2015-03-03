//
//  MU_EyesViewController.m
//  iOSMakeUp
//
//  Created by gaoluyangrc on 14-9-30.
//  Copyright (c) 2014年 rcplatform. All rights reserved.
//

#import "MU_EyesViewController.h"
#import "MBProgressHUD.h"
#import "FaceppAPI.h"
#import "APIKey+APISecret.h"

@interface MU_EyesViewController ()

- (IBAction)sliderHaveChangedValue:(id)sender;
- (IBAction)buttonClick:(id)sender;

@end

@implementation MU_EyesViewController

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
    
    NSString *API_KEY = _API_KEY;
    NSString *API_SECRET = _API_SECRET;
    
    [FaceppAPI initWithApiKey:API_KEY andApiSecret:API_SECRET andRegion:APIServerRegionCN];
    
    // turn on the debug mode
    [FaceppAPI setDebugMode:TRUE];
    
    [self detectWithImage:_eyesImage];
}

- (NSMutableArray *)workOutPoints:(NSDictionary *)content
{
    NSArray *leftEyeNameArray = @[@"left_eye_left_corner",@"left_eye_top",@"left_eye_right_corner",@"left_eye_bottom",@"left_eye_pupil"];
    NSArray *rightEyeNameArray = @[@"right_eye_left_corner",@"right_eye_top",@"right_eye_right_corner",@"right_eye_bottom",@"right_eye_pupil"];
    
    NSMutableArray *eyesArray = [NSMutableArray arrayWithCapacity:2];
    
    NSMutableArray *leftEyeArray = [NSMutableArray arrayWithCapacity:5];
    NSMutableArray *rightEyeArray = [NSMutableArray arrayWithCapacity:5];
    
    NSDictionary *landmarkDic = [[[content objectForKey:@"result"] objectAtIndex:0] objectForKey:@"landmark"];
    for (int i = 0; i < 5; i++)
    {
        //左眼
        NSString *left_eye_bottom = [landmarkDic objectForKey:[leftEyeNameArray objectAtIndex:i]];
        [leftEyeArray addObject:left_eye_bottom];
        //右眼
        NSString *right_eye_bottom = [landmarkDic objectForKey:[rightEyeNameArray objectAtIndex:i]];
        [rightEyeArray addObject:right_eye_bottom];
    }
    [eyesArray addObject:leftEyeArray];
    [eyesArray addObject:rightEyeArray];
    
    return eyesArray;
}

// Use facepp SDK to detect faces
- (void)detectWithImage:(UIImage*)image
{
    //请求结果
    FaceppResult *result = [[FaceppAPI detection] detectWithURL:nil orImageData:UIImageJPEGRepresentation(image, 0.5) mode:FaceppDetectionModeNormal attribute:FaceppDetectionAttributeNone];
    
    if (result.success)
    {
        NSMutableArray *eyesPointArray = [NSMutableArray arrayWithCapacity:0];
        for (NSDictionary *dic in [result content][@"face"])
        {
            NSString *face_id = [dic objectForKey:@"face_id"];
            FaceppResult *eyesResult = [[FaceppAPI detection] detectEyesWithFaceID:face_id];
            
            if (eyesResult.success)
            {
                NSMutableArray *eyesArray = [self workOutPoints:eyesResult.content];
                NSLog(@"eyesArray.....%@",eyesArray);
                [eyesPointArray addObject:eyesArray];
            }
        }
        
        UIGraphicsBeginImageContext(image.size);
        [image drawAtPoint:CGPointZero];
        CGContextRef context = UIGraphicsGetCurrentContext();
        CGContextSetRGBFillColor(context, 1.0, 0, 0, 1.0);
        CGContextSetLineWidth(context, 2.f);
        
        for (NSMutableArray *eyesArray  in eyesPointArray)
        {
            for (NSMutableArray *array in eyesArray)
            {
                int i = 0;
                for (NSDictionary *pointDic in array)
                {
                    if (i != 4)
                    {
                        float x = [pointDic[@"x"] floatValue] * 0.01 * image.size.width;
                        float y = [pointDic[@"y"] floatValue] * 0.01 * image.size.height;
                        CGContextSetFillColorWithColor(context, [UIColor redColor].CGColor);
                        CGContextAddArc(context, x, y, 2, 0, 2 * M_PI, 0);
                        CGContextFillPath(context);
                    }
                    i++;
                }
            }
        }
        
        UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        
        self.imageView.image = newImage;
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:[NSString stringWithFormat:@"error message: %@", [result error].message]
                              message:@""
                              delegate:nil
                              cancelButtonTitle:@"OK!"
                              otherButtonTitles:nil];
        [alert performSelectorOnMainThread:@selector(show) withObject:nil waitUntilDone:YES];
    }
    [MBProgressHUD hideHUDForView:self.view animated:YES];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
}

- (IBAction)sliderHaveChangedValue:(id)sender
{
    
}

- (IBAction)buttonClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    switch (btn.tag) {
        case 0:
            
            break;
        case 1:
            
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        default:
            break;
    }
}

@end
