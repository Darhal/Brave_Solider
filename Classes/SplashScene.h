#ifndef __Splash_SCENE_H__
#define __Splash_SCENE_H__

#include "cocos2d.h"

class SplashScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

	void SplashScene::GoToGameScene(float);
	void SplashScene::RotateLoading(float);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
};

#endif // __Splash_SCENE_H__
