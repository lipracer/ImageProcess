//
//  ViewController.m
//  iosImageProcess
//
//  Created by 陈龙 on 2019/10/12.
//  Copyright © 2019 陈龙. All rights reserved.
//

#import "ViewController.h"
//#include "cv_algorithm.h"
#import "GLNormalView.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    GLNormalView *_glView = [[GLNormalView alloc] init];
    _glView.frame = self.view.frame;
    [self.view addSubview:_glView];
}

@end
