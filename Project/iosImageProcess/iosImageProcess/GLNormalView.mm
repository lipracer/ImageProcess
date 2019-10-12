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
}

-(void)drawRect:(CGRect)rect
{
    _esContext->width  = self.drawableWidth;
    _esContext->height = self.drawableHeight;
    _esContext->draw();
}

@end
