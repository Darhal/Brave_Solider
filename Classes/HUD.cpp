#include "cocos2d.h"
#include "HUD.h"
#include "GameManager.h"

#define COCOS2D_DEBUG 1 //for debug
#define TRANSITION_TIME 3

USING_NS_CC;

HUD::HUD(Layer *layer, int pHealth)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// CREATE NEW NODE FOR HUD IT WILL BE ON THE TOP OF EVERYTHING 
	/*HUDNode = Node::create();
	//CENTER THE HUD NODE
	HUDNode->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + (visibleSize.height / 2)));

	// CREATE HEART ICON FOR THE HEALTH
	layer->addChild(HUDNode, 100);*/
	auto background = Sprite::create("icons/popup_base.png");
	healthIcon = Sprite::create("Icons/heart.png");
	background->setOpacity(98);
	background->setPosition(Vec2(origin.x + healthIcon->getContentSize().width / 2,
		((origin.y + visibleSize.height) - healthIcon->getContentSize().height /6)
	));
	background->setScaleX(2.3);
	background->setScaleY(0.35);
	background->setColor(Color3B(160, 160, 160));
	layer->addChild(background);
	healthIcon->setScale(0.8f);
	layer->addChild(healthIcon);
	healthIcon->setPosition(Vec2(origin.x + healthIcon->getContentSize().width / 4,
		((origin.y + visibleSize.height) - healthIcon->getContentSize().height / 4)
	));
	// CREATE HEALTH BAR !
	progressTimer = ProgressTimer::create(Sprite::create("entities/progressbar/progress.png"));
	progressTimer->setScaleY(0.2f);
	progressTimer->setScaleX(0.25f);
	if (progressTimer != NULL)
	{
		progressTimer->setType(ProgressTimer::Type::BAR);
		progressTimer->setMidpoint(Vec2(0, 0));
		progressTimer->setBarChangeRate(Vec2(1, 0));
		progressTimer->setPercentage(pHealth);
		progressTimer->setPosition(Vec2(100, 100));
		layer->addChild(progressTimer);
	}
	progressTimer->setPosition(Vec2(
		origin.x + healthIcon->getContentSize().width + progressTimer->getContentSize().width / 16,
		((origin.y + visibleSize.height) - healthIcon->getContentSize().height/5)
	));
	mainLayer = layer;
}

void HUD::scroll_to_percentage(float percentage)
{
	float duration = 0.09f;
	ProgressTo* front_prog_to = ProgressTo::create(duration, percentage);

	Sequence* front_sequence = Sequence::create(front_prog_to, NULL);
	progressTimer->runAction(front_sequence);
};

void HUD::drawLoseMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("ui/bg_overlay.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	bg->setContentSize(Size(visibleSize.width, visibleSize.height));
	auto replayItem = MenuItemImage::create("Menu/buttonPlay.png",
		"Menu/buttonPlayPressed.png", CC_CALLBACK_1(HUD::OnRePlayButtonPress, this));
	replayItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	auto menu = Menu::create(replayItem, NULL);
	menu->setPosition(Point::ZERO);
	mainLayer->addChild(bg);
	mainLayer->addChild(menu);
}


void HUD::showLoseMenu()
{
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(HUD::drawLoseMenu, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(6), runCallback, nullptr));
}

void HUD::OnRePlayButtonPress(cocos2d::Ref* sender)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto loading = Sprite::create("BG/loading.jpeg");
	loading->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	loading->setContentSize(Size(visibleSize.width, visibleSize.height));
	mainLayer->addChild(loading);
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(HUD::replay, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(3), runCallback, nullptr));
	CCLOG("RePLAY BUTTON PRESSED !");
}

void HUD::replay()
{
	auto scene = GameManager::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
