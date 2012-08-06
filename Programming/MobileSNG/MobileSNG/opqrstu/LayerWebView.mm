//
//  LayerWebView.mm
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "LayerWebView.h"
#include "UIWebViewBridge.h"
#include "HelloWorldScene.h"

    static UIWebViewBridge *gUIWebViewBridge=nil;

    LayerWebView::LayerWebView()
    {
    
    }
                  
    LayerWebView::~LayerWebView()
    {
      [gUIWebViewBridge release];
    }

    void LayerWebView::webViewDidFinishLoad()
    {
      
    }

    void LayerWebView::onBackbuttonClick()
    {
        cocos2d::CCDirector::sharedDirector()->replaceScene(cocos2d::CCTransitionSlideInT::transitionWithDuration(0.0, 
                                                                                             HelloWorld::scene()));
    }

    bool LayerWebView::init()
    {
        //////////////////////////////
        // 1. super init first
        if ( !CCLayer::init() )
        {
            return false;
        }
         
        gUIWebViewBridge = [[UIWebViewBridge alloc] init];  
        [gUIWebViewBridge setLayerWebView : this];
        return true;
    } 
    
