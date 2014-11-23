#import <UIKit/UIKit.h>
#import "GADBannerView.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, GADBannerViewDelegate> {
    UIWindow *window;
    
    GADBannerView *bannerView;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end

