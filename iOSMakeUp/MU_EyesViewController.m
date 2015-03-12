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
#import "MUEyesView.h"

@interface MU_EyesViewController ()
{
    NSMutableArray *eyesPointArray;
    CGSize imageSize;
    float scale;
    MUEyesView *leftEye;
    MUEyesView *rightEye;
}
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
    
    _imageView.userInteractionEnabled = YES;
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] init];
    [pan addTarget:self action:@selector(panView:)];
    [_imageView addGestureRecognizer:pan];
    
    NSString *API_KEY = _API_KEY;
    NSString *API_SECRET = _API_SECRET;
    [FaceppAPI initWithApiKey:API_KEY andApiSecret:API_SECRET andRegion:APIServerRegionCN];
    [FaceppAPI setDebugMode:FALSE];
    imageSize = _eyesImage.size;
    
    leftEye = [[MUEyesView alloc] initWithFrame:_imageView.frame];
    [leftEye setEyeballImage:[UIImage imageNamed:@"05"]];
    leftEye.backgroundColor = [UIColor clearColor];
    leftEye.userInteractionEnabled = NO;
    [_backView addSubview:leftEye];
    
    rightEye = [[MUEyesView alloc] initWithFrame:_imageView.frame];
    [rightEye setEyeballImage:[UIImage imageNamed:@"05"]];
    rightEye.backgroundColor = [UIColor clearColor];
    rightEye.userInteractionEnabled = NO;
    [_backView addSubview:rightEye];
    
    [self detectWithImage:_eyesImage];
}

- (void)panView:(UIPanGestureRecognizer *)recognizer
{
    UIView *panView = recognizer.view;
    CGPoint locationPoint;
    if (recognizer.state == UIGestureRecognizerStateBegan)
    {
        locationPoint = [recognizer locationInView:panView];
    }
    
    CGPoint translation = [recognizer translationInView:self.view];
    [leftEye movePointNearly:locationPoint movePoint:translation];
    
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        leftEye.space = 0.0f;
    }
    
    [recognizer setTranslation:CGPointMake(0, 0) inView:self.view];
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
- (void)detectWithImage:(UIImage *)image
{
    //请求结果
    FaceppResult *result = [[FaceppAPI detection] detectWithURL:nil orImageData:UIImageJPEGRepresentation(image, 0.5) mode:FaceppDetectionModeNormal attribute:FaceppDetectionAttributeNone];
    
    if (result.success)
    {
        eyesPointArray = [NSMutableArray arrayWithCapacity:0];
        for (NSDictionary *dic in [result content][@"face"])
        {
            NSString *face_id = [dic objectForKey:@"face_id"];
            FaceppResult *eyesResult = [[FaceppAPI detection] detectEyesWithFaceID:face_id];
            
            if (eyesResult.success)
            {
                NSMutableArray *eyesArray = [self workOutPoints:eyesResult.content];
                [eyesPointArray addObject:eyesArray];
            }
        }

        leftEye.imageSize = imageSize;
        leftEye.eyePoints = [NSMutableArray arrayWithArray:eyesPointArray[0][0]];
        rightEye.imageSize = imageSize;
        rightEye.eyePoints = [NSMutableArray arrayWithArray:eyesPointArray[0][1]];

        self.imageView.image = image;
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
    switch (btn.tag)
    {
        case 0:
        {
            CGPoint firstPoint;
            CGPoint secendPoint;
            CGPoint eyeballPoint;
            for (NSMutableArray *eyesArray  in eyesPointArray)
            {
                for (NSMutableArray *array in eyesArray)
                {
                    int i = 0;
                    for (NSDictionary *pointDic in array)
                    {
                        float x = [pointDic[@"x"] floatValue] * 0.01 * imageSize.width/(imageSize.width/320);
                        float y = [pointDic[@"y"] floatValue] * 0.01 * imageSize.height/(imageSize.height/320.f);
                        if (i == 0)
                        {
                            firstPoint = CGPointMake(x, y);
                        }
                        else if (i == 2)
                        {
                            secendPoint = CGPointMake(x, y);
                        }
                        else if (i == 4)
                        {
                            eyeballPoint = CGPointMake(x, y);
                        }
                        i++;
                    }
                    break;
                }
            }
            
            CGFloat change_x = 160.f - eyeballPoint.x;
            CGFloat change_y = 160.f - eyeballPoint.y;
            CGRect originRect = _backView.frame;
            originRect.origin.x += change_x;
            originRect.origin.y += change_y;
            [_backView setFrame:originRect];
            
            scale = 160.f/(secendPoint.x - firstPoint.x);
            float anchorScale_x = eyeballPoint.x/320.f;
            float anchorScale_y = eyeballPoint.y/320.f;
            CABasicAnimation *pulse = [self scale:[NSNumber numberWithFloat:scale] orgin:[NSNumber numberWithFloat:1.f] durTimes:.5f Rep:1];
            [self setAnchorPoint:CGPointMake(anchorScale_x, anchorScale_y) forView:_backView];
            [_backView.layer addAnimation:pulse forKey:@"transform.scale"];
            leftEye.isScale = YES;
        }
            break;
        case 1:
        {
            [leftEye setIsAdjust:YES];
        }
            break;
        case 2:
        {
            [leftEye setIsShowEyeball:YES];
        }
            break;
        case 3:
        {
            CABasicAnimation *pulse = [self scale:[NSNumber numberWithFloat:1.f] orgin:[NSNumber numberWithFloat:scale] durTimes:0.5f Rep:1];
            [self setAnchorPoint:CGPointMake(0.5f, 0.5f) forView:_backView];
            [_backView.layer addAnimation:pulse forKey:@"transform.scale"];
            _backView.frame = CGRectMake(0, 0, 320, 320);
            leftEye.isScale = NO;
        }
            break;
        default:
            break;
    }
}

- (void)setAnchorPoint:(CGPoint)anchorpoint forView:(UIView *)view
{
    CGRect oldFrame = view.frame;
    view.layer.anchorPoint = anchorpoint;
    view.frame = oldFrame;
}

- (CABasicAnimation *)scale:(NSNumber *)Multiple orgin:(NSNumber *)orginMultiple durTimes:(float)time Rep:(float)repeatTimes
{
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.scale"];
    
    animation.fromValue=orginMultiple;
    
    animation.toValue=Multiple;
    
    animation.duration=time;
    
    animation.autoreverses = NO;
    
    animation.repeatCount=repeatTimes;
    
    animation.removedOnCompletion = NO;
    
    animation.fillMode=kCAFillModeForwards;
    
    return animation;
}

- (CAAnimationGroup *)groupAnimation:(NSArray *)animationAry durTimes:(float)time Rep:(float)repeatTimes
{
    CAAnimationGroup *animation=[CAAnimationGroup animation];
    
    animation.animations=animationAry;
    
    animation.duration=time;
    
    animation.repeatCount=repeatTimes;
    
    animation.removedOnCompletion=NO;
    
    animation.fillMode=kCAFillModeForwards;
    
    return animation;
}

@end
