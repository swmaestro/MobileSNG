//
//  MobileSNGAppController.h
//  MobileSNG
//
//  Created by 박 진 on 12. 7. 14..
//  Copyright __MyCompanyName__ 2012년. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

