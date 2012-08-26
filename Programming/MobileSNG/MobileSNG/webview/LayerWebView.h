//
//  LayerWebView.h
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __CC_UIWEBVIEW_H__
#define __CC_UIWEBVIEW_H__


#include "CCCommon.h"
#include "cocos2d.h"



class LayerWebView : public cocos2d::CCLayer
{
private:
    //	CCMenu * menu;
	
//	Label *label;


	int mWebViewLoadCounter;

public:
    LayerWebView();
    ~LayerWebView();
    virtual bool init();  
    
    void webViewDidFinishLoad();
    void onBackbuttonClick();
};
    

#endif