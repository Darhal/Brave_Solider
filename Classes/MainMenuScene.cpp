#include "GameManager.h"
#include "MainMenuScene.h"
#include "ui/CocosGUI.h"

#define COCOS2D_DEBUG 1


#define DISPLAY_TIME_SPLASH_SCENE 1.5
#define TRANSITION_TIME 0.5


USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
	auto sprt = Sprite::create("Menu/background.jpg");
	// position the sprite on the center of the screen
	sprt->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	sprt->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(sprt, 1);

	auto playItem = MenuItemImage::create("Menu/buttonPlay.png", 
		"Menu/buttonPlayPressed.png", CC_CALLBACK_1(MainMenu::OnPlayButtonPress, this));
	playItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
   
	auto menu = Menu::create(playItem, NULL);
	menu->setPosition(Point::ZERO);
	addChild(menu,2);
	return true;
}

void MainMenu::OnPlayButtonPress(cocos2d::Ref* sender)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto loading = Sprite::create("BG/loading.jpeg");
	loading->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	loading->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(loading, 100);
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(MainMenu::play, this));
	this->runAction(Sequence::create(DelayTime::create(3), runCallback, nullptr));
	CCLOG("PLAY BUTTON PRESSED !");
}


void MainMenu::play()
{
	auto scene = GameManager::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
