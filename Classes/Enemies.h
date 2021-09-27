#ifndef __ENEMIES_H__
#define __ENEMIES_H__

#include "cocos2d.h"
#include "Player.h"
#include <map>

class Enemies
{
public:

	Enemies(cocos2d::Layer*, cocos2d::Node*, cocos2d::TMXTiledMap*, int, int, int);
	void Enemies::loadAnimations(int);
	const int bodyTag = 9;
	cocos2d::Sprite* getSprite();

	void changeAction(int id);
	void performActionAI(cocos2d::Layer*, Player*);
	void update();
	// FACE TO THE PLAYER
	void facePlayer(cocos2d::Vec2, cocos2d::Vec2);
	void moveToPlayer();
	void stopMovement();
	void helpJump();
	void jump(bool);
	void attack();
	void loadBullet();
	void cleanUpBullets(cocos2d::Sprite*);
	void finishDamageEffect();
	void takeDamage(int);
	void decreaseAlpha();
	void kill();

	~Enemies();

	bool isActive = false;
	cocos2d::Layer* mainLayer;
	int health;
	bool freezeEnemy = false;

	Player* plr;
private:
	cocos2d::Vector<cocos2d::Action*> enmActions;
	char* getAnimPath(int);
	cocos2d::Sprite* enmSprite;
	bool isMoving;
	int eLastAction = -1;
	int valueToChangeAction;
	cocos2d::Vec2 eLastPos;
	bool mvtDir;
	int eBulletTag = 12;
	int enmId;
	int sprtAlpha = 100;
};


#endif // __Splash_SCENE_H__
