#include "AdMobHelper.h"
#include "cocos2d.h"

bool AdMobHelper::isAdShowing = true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>

const char* AppActivityClassName = "org/cocos2dx/cpp/AppActivity";

void AdMobHelper::hideAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = false;
	}
}

void AdMobHelper::showAd()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		isAdShowing = true;
	}
}

#else

void AdMobHelper::hideAd()
{
	CCLOG("hideAd() called");
	isAdShowing = false;
	return;
}


void AdMobHelper::showAd()
{
    CCLOG("showAd() called");
    isAdShowing = true;
    return; //nothing;

}

#endif
