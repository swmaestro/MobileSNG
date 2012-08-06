//
//  UIDelegateBridge.mm
//  MyApp1
//
//  Created by DAC-TOAN HO on 11-10-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "UIWebViewBridge.h"
#import "EAGLView.h"

@implementation UIWebViewBridge

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
   	[mBackButton release];
    [mToolbar release];
	[mWebView release];
	[mView release]; 
    [super dealloc];
}


-(void) setLayerWebView : (LayerWebView*) iLayerWebView
{
    mLayerWebView = iLayerWebView;
    cocos2d::CCSize size = mLayerWebView-> getContentSize();
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, size.width , size.height)];
    
    // create webView
    int wBottomMargin = size.height*0.10;
    int wWebViewHeight = size.height - wBottomMargin;
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, size.width, wWebViewHeight)];
    mWebView.delegate = self;
    
    NSString *urlBase = @"http://google.com";
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are 
    
    
    //create a tool bar for the bottom of the screen to hold the back button
    mToolbar = [UIToolbar new];
    [mToolbar setFrame:CGRectMake(0, wWebViewHeight, size.width, wBottomMargin)];
    mToolbar.barStyle = UIBarStyleBlackOpaque;
    
    //Create a button
    mBackButton = [[UIBarButtonItem alloc] initWithTitle:@"Return to the game" 
                                                                   style: UIBarButtonItemStyleDone 
                                                                   target: self
                                                                   action:@selector(backClicked:)];
    //[backButton setBounds:CGRectMake(0.0, 0.0, 95.0, 34.0)];
    [mToolbar setItems:[NSArray arrayWithObjects:mBackButton,nil] animated:YES];
    [mView addSubview:mToolbar];
    //[mToolbar release];
    
    
    // add the webView to the view
    [mView addSubview:mWebView];
    [[EAGLView sharedEGLView] addSubview:mView];

}


- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
	
    
}

- (void)webViewDidFinishLoad:(UIWebView *)thisWebView{ 
    [mWebView setUserInteractionEnabled:YES];
    mLayerWebView->webViewDidFinishLoad();
}

- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error 
{
	if ([error code] != -999 && error != NULL) { //error -999 happens when the user clicks on something before it's done loading.
		
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"MultiTest was unable to load the page. Please try again later when you have a network connection."
													   delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
		[alert show];
		[alert release];
		
	}		
    
} 


-(void) backClicked:(id)sender {
	mWebView.delegate = nil; //keep the webview from firing off any extra messages
    
	//remove items from the Superview...just to make sure they're gone
	[mToolbar removeFromSuperview];
	[mWebView removeFromSuperview];
	[mView removeFromSuperview];    
   
	mLayerWebView->onBackbuttonClick();	
}


@end
