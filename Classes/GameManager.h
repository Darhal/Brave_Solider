#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "Player.h"
#include "TiledMap.h"
#include "cocos2d.h"
#include <vector>
#include <map>

class GameManager : public cocos2d::Layer
{
public:
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyStatus;

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	// Physics Contact Listener.....
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactPreSolve(cocos2d::PhysicsContact& contact, 
		cocos2d::PhysicsContactPreSolve& solve);
	void onContactSeperate2(cocos2d::PhysicsContact& contact);
	void onContactSeperate();

	void updateReady(float);
	void update(float delta) override;

	void GameManager::updatePhysics(float);

    // implement the "static create()" method manually
	CREATE_FUNC(GameManager);
private:
	
	Player* GameManager::player;
	TiledMapManager* GameManager::map;
	cocos2d::PhysicsWorld* physicsWorld;
	cocos2d::Sprite* bgs[5];
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { physicsWorld = world; };

	void GameManager::resetToAction();

	std::vector<Enemies*> detectedEnmPack;
	std::vector<Enemies*> emptyEnmPack;

	cocos2d::Node* edgeNode;


};


#endif // __HELLOWORLD_SCENE_H__
