#ifndef __TILED_MAP_H__
#define __TILED_MAP_H__

#include "cocos2d.h"
#include <iostream>
#include <map>
#include <vector>
#include "Enemies.h"

class TiledMapManager :  public cocos2d::Layer
{
public:
	std::map<char, int> TiledMapManager::coinTags;

	TiledMapManager(cocos2d::Layer* layer, cocos2d::Node* node,
		cocos2d::PhysicsWorld* world );
	void loadTiledMap(int, cocos2d::Layer* layer);

	void TiledMapManager::loadEnemySpawnPoint(cocos2d::Layer* layer, cocos2d::Node* node,
		cocos2d::TMXTiledMap* map);
	void loadCoins(cocos2d::Node* node);
	void loadChest(cocos2d::Node*);
	void loadKeys(cocos2d::Node*);
	void playChestAnimation(cocos2d::Node*, std::string, Player*);
	void destroySpite(cocos2d::Sprite*);
	cocos2d::Action* loadCoinAnimations(std::string);

	cocos2d::TMXTiledMap* map;
	const int earthTag = 2;
	const int chestTag = 14;
	const int keyTag = 15;
	int indexTagEnmDetector = 100;
	cocos2d::Node* mainLayer;

	std::map<cocos2d::PhysicsShape*, std::vector<Enemies*>> EnemiesRoot;

private:
	cocos2d::PhysicsShapePolygon*  createObjectShape(cocos2d::ValueMap object);
	cocos2d::PhysicsShape*  createBody(cocos2d::ValueMap object);
	void initCollisionMap(cocos2d::TMXTiledMap* map);
};

#endif // __Splash_SCENE_H__
