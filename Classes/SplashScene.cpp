#include "MainMenuScene.h"
#include "SplashScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#define COCOS2D_DEBUG 1 //for debug
#define DISPLAY_TIME_SPLASH_SCENE 3
#define TRANSITION_TIME 0.5

USING_NS_CC;

using namespace cocostudio::timeline;

cocos2d::Sprite* loadingBar;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    

	auto backgroundSprite = Sprite::create("BG/SplashBackgorund.jpg");
	// position the sprite on the center of the screen
	backgroundSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backgroundSprite->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(backgroundSprite);

	/*auto loadingBar = Sprite::create("loading_bar.png");
	// position the sprite on the center of the screen
	loadingBar->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	//loadingBar->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(loadingBar);*/
	//this->schedule(schedule_selector(SplashScene::RotateLoading), 0.5);

	this->scheduleOnce(schedule_selector(SplashScene::GoToGameScene), DISPLAY_TIME_SPLASH_SCENE);
   
	return true;
}

void SplashScene::RotateLoading(float dt)
{
	float lastRot = loadingBar->getRotation();
	loadingBar->setRotation(lastRot + 1);
}


void SplashScene::GoToGameScene(float dt)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));

	CCLOG("GOING TO MAIN MENU !");
}