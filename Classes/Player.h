#ifndef __Player_H__
#define __Player_H__

#include "HUD.h"
#include "cocos2d.h"
#include "Inventory.h"

class Player 
{
public:
	Player(cocos2d::Layer* Layer, cocos2d::Node* node);

	void setPlayerMvtDir(bool);
	void movePlayer(int, int, int, bool);
	void stopMovment();
	void updatePosition(bool, int, int);
	cocos2d::Sprite* PlayerSprite;
	void setPlayerAction(int id);
	
	void loadBullets();
	void kill();
	void takeDamage(int);
	void finishDamageEffect();

	const int bodyTag = 1;
	const int bulletTag = 3;
	bool isPlayerFalling;
	bool canJumpAgain = true;
	bool isPlayerSliding = false;
	cocos2d::Node* mainNode;

	bool blockActions = false;
	int health;
	int mvtActionId;
	// CUSTOM OBJECTS !
	HUD* PlayerHud;
	Inventory* playerInventory;
	
	
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Vector<cocos2d::Action*> plrActions;
	
	cocos2d::Action* lastAction;
	cocos2d::Layer* mainLayer;
	
	// FUNCTIONS
	void loadAnimations();
	void helpFunctionOfPlayerAction(int id);
	void createPlayerPhysics();
	cocos2d::Action* getPlayerActionByID(int);

};

#endif // __Player_H__
