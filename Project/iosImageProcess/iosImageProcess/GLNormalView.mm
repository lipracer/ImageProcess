//
//  GLNormalView.m
//  iosImageProcess
//
//  Created by 陈龙 on 2019/10/12.
//  Copyright © 2019 陈龙. All rights reserved.
//

#import "GLNormalView.h"
#include "es_context.hpp"

ALESContext _esContext;

@interface GLNormalView ()
{
    
    
}
//@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;


- (void)setupGL;
- (void)tearDownGL;
@end

@implementation GLNormalView

-(instancetype)init
{
    if(self = [super init])
    {
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        [self setupGL];
    }
    return self;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    _esContext = std::make_shared<LESContext>();
    
    UIImage *img_lena = [UIImage imageNamed:@"lena.png"];
    
    CGImageAlphaInfo alphaInfo = CGImageGetAlphaInfo(img_lena.CGImage);
    CGColorSpaceRef colorRef = CGColorSpaceCreateDeviceRGB();

    float width = img_lena.size.width;
    float height = img_lena.size.height;

    // Get source image data
    uint8_t *imageData = (uint8_t *) malloc(width * height * 4);

    CGContextRef imageContext = CGBitmapContextCreate(imageData,
            width, height,
            8, static_cast<size_t>(width * 4),
            colorRef, alphaInfo);

    CGContextDrawImage(imageContext, CGRectMake(0, 0, width, height), img_lena.CGImage);
    CGContextRelease(imageContext);
    CGColorSpaceRelease(colorRef);
    
    _esContext->CreateSimpleTexture2D((char*)imageData, width, height);
}

-(void)drawRect:(CGRect)rect
{
    _esContext->width  = self.drawableWidth;
    _esContext->height = self.drawableHeight;
    _esContext->draw();
}

@end
