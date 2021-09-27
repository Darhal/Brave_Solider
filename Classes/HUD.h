#ifndef __HUD_H__
#define __HUD_H__

#include "cocos2d.h"

class HUD
{
public:
	HUD(cocos2d::Layer*, int);

	cocos2d::Node* HUDNode;

	cocos2d::ProgressTimer* progressTimer;
	cocos2d::Sprite* healthIcon;
	void showLoseMenu();
	void drawLoseMenu();
	void OnRePlayButtonPress(cocos2d::Ref*);
	void scroll_to_percentage(float percentage);
	void replay();

private:
	cocos2d::Layer* mainLayer;
};

#endif // __HUD_H__
