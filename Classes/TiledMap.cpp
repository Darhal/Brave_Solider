#include "TiledMap.h"
#include "cocos2d.h"
#include "Enemies.h"
#include "Inventory.h"
#include <iostream>

#define COCOS2D_DEBUG 1 //for debug
#define PTMRATIO 1
#define MAP_SIZE 499

USING_NS_CC;

cocos2d::Action* coinActions[3];

TiledMapManager::TiledMapManager(cocos2d::Layer* layer, cocos2d::Node* node, cocos2d::PhysicsWorld* world1)
{	
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//TiledMap::loadTiledMap(1, layer);
	const char* path = "tmxMaps/map%01d.tmx";
	char str[50] = { 0 };
	sprintf(str, path, 1);
	CCLOG("LOADING MAP ID: %01d | PATH : %s", 1, str);

	map = TMXTiledMap::create(str);

	//map->setScale(2);
	//experimental::Viewport::Viewport(0,0, visibleSize.width, visibleSize.height);
	TiledMapManager::initCollisionMap(map);

	// SET TAG FOR EACH COIN TYPE
	TiledMapManager::coinTags['G'] = 6;
	TiledMapManager::coinTags['S'] = 7;
	TiledMapManager::coinTags['B'] = 8;
	coinActions[3] = { NULL };
	TiledMapManager::loadCoins(node);
	TiledMapManager::loadEnemySpawnPoint(layer, node,map);
	TiledMapManager::loadChest(node);
	TiledMapManager::loadKeys(node);
	
	/*for (const auto& child : map1->getChildren())
	{
		static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
	}*/
	node->addChild(map);
	mainLayer = node;
	CCLOG("TILED MAP SUCCESS : MAP LOADED !");
}

char* iteamNameToPath(std::string name)
{
	if (name == "Health") {
		return "Icons/heart.png";
	}
	else if (name == "Gold") {
		return "entities/coins/Gold/Gold_1.png";
	}
	else if (name == "Silver") {
		return "entities/coins/Silver/Silver_1.png";
	}
	else if (name == "Bronze") {
		return  "entities/coins/Bronze/Bronze_1.png";
	}
}

void TiledMapManager::loadChest(Node* layer)
{
	auto collisionGroup = map->getObjectGroup("Chest");
	cocos2d::ValueVector collisionObjects = collisionGroup->getObjects();

	for (cocos2d::Value objectValue : collisionObjects)
	{	
			auto prop = objectValue.asValueMap();
			int xC = prop["x"].asInt();
			int yC = prop["y"].asInt();
			std::string name = prop["name"].asString();
			char* path = "entities/Chest/1.png";
			Sprite* chestSprite = Sprite::create(path);
			chestSprite->setPosition(Vec2(xC, yC));
			chestSprite->setScale(0.3f);
			chestSprite->setAnchorPoint(Vec2(0.5, 0.5));
			auto chestBody = PhysicsBody::createBox(chestSprite->getContentSize()/2);
			chestBody->setCategoryBitmask(16);
			chestBody->setContactTestBitmask(0xFFFFFFFF);
			chestBody->setCollisionBitmask(1 + 2);
			chestBody->setDynamic(true);
			//chestBody->setGravityEnable(true);
			chestBody->setAngularVelocity(0);
			chestBody->setTag(chestTag);
			chestBody->setName(name);
			chestSprite->setPhysicsBody(chestBody);
			layer->addChild(chestSprite, 88);
			CCLOG("LOADING A CHEST !");
	}
}

void TiledMapManager::loadKeys(Node* layer)
{
	auto collisionGroup = map->getObjectGroup("Keys");
	cocos2d::ValueVector collisionObjects = collisionGroup->getObjects();

	for (cocos2d::Value objectValue : collisionObjects)
	{
		auto prop = objectValue.asValueMap();
		int xC = prop["x"].asInt();
		int yC = prop["y"].asInt();
		std::string name = prop["name"].asString();
		char* path = "Icons/4key.png";
		Sprite* keySprite = Sprite::create(path);
		keySprite->setPosition(Vec2(xC, yC));
		keySprite->setScale(0.2f);
		keySprite->setAnchorPoint(Vec2(0.5, 0.5));
		auto keyBody = PhysicsBody::createBox(keySprite->getContentSize() / 2);
		keyBody->setCategoryBitmask(16);
		keyBody->setContactTestBitmask(0xFFFFFFFF);
		keyBody->setCollisionBitmask(1 + 2);
		keyBody->setDynamic(false);
		//chestBody->setGravityEnable(true);
		keyBody->setAngularVelocity(0);
		keyBody->setTag(keyTag);
		keyBody->setName(name);
		keySprite->setPhysicsBody(keyBody);
		keySprite->runAction(RepeatForever::create(RotateBy::create(0.1,Vec3(25, 0,0))));
		layer->addChild(keySprite, 88);
		CCLOG("LOADING A KEY !");
	}
}

void TiledMapManager::destroySpite(Sprite* sprt)
{
	sprt->removeFromParentAndCleanup(true);
}

void TiledMapManager::playChestAnimation(Node* sprt, std::string name, Player* plr)
{
	Vector<SpriteFrame*> animFrames(8);
	char str[100] = { 0 };
	for (int i = 1; i < 7; i++)
	{
		sprintf(str, "entities/Chest/%01d.png", i);
		auto frame = SpriteFrame::create(str, Rect(0, 0, 256,256)); //we assume that the sprites' dimentions are 40*40 rectangles.
		animFrames.pushBack(frame);
	}

	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.01f);
	auto animate = Animate::create(animation);
	sprt->runAction(animate);
	Vec2 pos = sprt->getPosition();
	auto iconItem = Sprite::create(iteamNameToPath(name));
	iconItem->setPosition(pos);
	mainLayer->addChild(iconItem, 89);
	if (name == "Health")
		iconItem->setScale(0.75);
	else
		iconItem->setScale(0.25);
	auto actionmove = MoveTo::create(3, Vec3(pos.x-visibleSize.width,
		pos.y+ visibleSize.height,0));
		//visibleSize.height+origin.y, 0));
	auto actionScale = ScaleBy::create(3, 0.1);
	FadeIn* fadeIn = FadeIn::create(3.f);
	iconItem->setCascadeOpacityEnabled(true);
	iconItem->runAction(fadeIn);
	iconItem->runAction(actionmove);
	iconItem->runAction(actionScale);
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(TiledMapManager::destroySpite, this, iconItem));
	mainLayer->runAction(Sequence::create(DelayTime::create(2.75f), runCallback, nullptr));
	plr->playerInventory->removeItem(Inventory::Items::KEY, 1);
	plr->playerInventory->giveItemByName(name);
}

bool isInBox(int x, int y, int xmin, int ymin, int xmax, int ymax)
{
	if ((x>xmin) && (y>ymin) && (y<ymax) && (x<xmax))
		return true;
	else
		return false;
}


void TiledMapManager::loadEnemySpawnPoint(Layer* layer, Node* node, TMXTiledMap* map)
{
	TMXObjectGroup* enem = map->getObjectGroup("SpawnEnemy");
	Enemies* tempEnm[20];
	auto enemData = enem->getObjects();
	int counter = 0;
	for (auto&coinD : enemData)
	{
		auto prop = coinD.asValueMap();
		int xC = prop["x"].asInt();
		int yC = prop["y"].asInt();
		int id = prop["name"].asInt();
		tempEnm[counter] = new Enemies(layer, node, map, xC, yC, id);
		counter += 1;
	}
	auto CreateEnemyPointGroup = map->getObjectGroup("CreateEnemyPoint");
	ValueVector CreateEnemyPointObjects = CreateEnemyPointGroup->getObjects();
	indexTagEnmDetector = 100;
	
	PhysicsBody* CreateEnemyPointBody = PhysicsBody::create();
	map->getLayer("CreateEnemyPoint")->getTileAt(Vec2(0, MAP_SIZE))->setAnchorPoint(Vec2(0.5, 0.5));
	map->getLayer("CreateEnemyPoint")->getTileAt(Vec2(0, MAP_SIZE))->setPhysicsBody(CreateEnemyPointBody);
	for (cocos2d::Value objectValue : CreateEnemyPointObjects)
	{
		auto bodyShape = createBody(objectValue.asValueMap());
		if (bodyShape != NULL) {
			auto  object = objectValue.asValueMap();
			auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
			float width = object["width"].asFloat() / PTMRATIO;
			float height = object["height"].asFloat() / PTMRATIO;
			CCLOG("Create enemy detection rectangle !");
			CreateEnemyPointBody->setDynamic(false);
			CreateEnemyPointBody->addShape(bodyShape);
			CreateEnemyPointBody->setCategoryBitmask(8);
			CreateEnemyPointBody->setContactTestBitmask(1);
			CreateEnemyPointBody->setCollisionBitmask(0);
			CreateEnemyPointBody->setDynamic(false);
			bodyShape->setRestitution(0);
			//CreateEnemyPointBody->setTag(indexTagEnmDetector);
			bodyShape->setTag(indexTagEnmDetector);
			std::vector<Enemies*> allEnemiesBasedOnDetector;
			for (int i = 0; i <= (counter-1); i++) {
				if (tempEnm[(i)]) {
					//LOOPING THROUGH ALL ENEMIES ARROUND THE MAP (ID : %01d),i
					auto eEntity = tempEnm[(i)]->getSprite();
					auto eBody = eEntity->getPhysicsBody();
					Vec2 ePos = eEntity->getPosition();
					CCLOG("ePos.x : %05d, ePos.y : %05d, position.x : %05d, position.y: %05d, position.x + width : %05d, position.y + height: %05d", ePos.x, ePos.y, position.x, position.y, position.x + width,position.y + height);
					if ((eEntity) && isInBox(ePos.x, ePos.y, position.x, position.y, position.x + width,
						position.y + height)) {
						CCLOG("INSERTING ENEMY INTO VECTOR BASED ON THE DETECTOR ID : %01d ", i);
						allEnemiesBasedOnDetector.push_back(tempEnm[(i)]);
					}
				}
			}
			CCLOG("INDEXING FOR DETECTOR ZONE (TAG : %01d)", indexTagEnmDetector);
			TiledMapManager::EnemiesRoot[bodyShape] = allEnemiesBasedOnDetector;
			indexTagEnmDetector += 1;
		}
	}
}



Action* TiledMapManager::loadCoinAnimations(std::string name)
{
	/*if (name == "Gold" && coinActions[0] != NULL) {
		return coinActions[0]->clone();
		//CCLOG("RETURN STORED ACTION FOR GOLD !");
	}else if (name == "Silver" && coinActions[1] != NULL) {
		return coinActions[1]->clone();
		//CCLOG("RETURN STORED ACTION FOR SILVER !");
	}else if (name == "Bronze" && coinActions[2] != NULL) {
		return coinActions[2]->clone();
		//CCLOG("RETURN STORED ACTION FOR BRONZE !");
	}*/

	const int num = 30;
	cocos2d::Vector<cocos2d::SpriteFrame*> Frames;
	for (int k = 1; k <= num; ++k)
	{
		char str[50] = { 0 };
		const char* path = "entities/coins/Bronze/Bronze_%d.png";
		if (name == "Gold")
			path = "entities/coins/Gold/Gold_%d.png";
		else if (name == "Silver")
			path = "entities/coins/Silver/Silver_%d.png";
		else if (name == "Bronze")
			path = "entities/coins/Bronze/Bronze_%d.png";

		sprintf(str, path, k);

		auto coinSprite1 = Sprite::create(str);
		SpriteFrame* frame = SpriteFrame::create(str, Rect(0, 0,
			coinSprite1->getContentSize().width,
			coinSprite1->getContentSize().height));
		frame->setAnchorPoint(Vec2(0.5, 0.5));
		Frames.pushBack(frame);
		coinSprite1->removeFromParentAndCleanup(true);
	}

	auto AnimLoad = Animation::createWithSpriteFrames(Frames, 0.09f);
	auto Anim = Animate::create(AnimLoad);
	auto coinAction = RepeatForever::create(Anim);
	if (name == "Gold")
		coinActions[0] = coinAction;
	else if (name == "Silver")
		coinActions[1] = coinAction;
	else if (name == "Bronze")
		coinActions[2] = coinAction;
	return coinAction;
}

void TiledMapManager::loadCoins(Node* node)
{
	TMXObjectGroup* coins = map->getObjectGroup("Coins");
	auto coinData = coins->getObjects();
	//coinSprite->removeFromParentAndCleanup(true);

	

	for (auto&coinD : coinData)
	{
		auto prop = coinD.asValueMap();
		int xC = prop["x"].asInt();
		int yC = prop["y"].asInt();
		std::string name = prop["name"].asString();
		const char* path = "entities/coins/Bronze/Bronze_21.png";
		if (name == "Gold")
			path = "entities/coins/Gold/Gold_21.png";
		else if (name == "Silver")
			path = "entities/coins/Silver/Silver_21.png";
		else if (name == "Bronze")
			path = "entities/coins/Bronze/Bronze_21.png";
		else
			name = "Bronze";

		Sprite* coinSprite = Sprite::create(path);

		coinSprite->setPosition(Vec2(xC, yC));
		coinSprite->setScale(0.27f);
		auto coinAction = TiledMapManager::loadCoinAnimations(name);
		coinSprite->runAction(coinAction);
		auto coinBody = PhysicsBody::createCircle(coinSprite->getContentSize().height/2);
		coinBody->setDynamic(false);
		coinSprite->setPhysicsBody(coinBody);
		coinBody->setCategoryBitmask(4);
		coinBody->setContactTestBitmask(0xFFFFFFFF);
		coinBody->setCollisionBitmask(1);
		coinBody->setTag(coinTags[name[0]]);
		coinBody->setMass(0.0);
		node->addChild(coinSprite, 5);
	}
}

void TiledMapManager::loadTiledMap(int id, cocos2d::Layer* layer)
{
	
}

PhysicsShapePolygon*  TiledMapManager::createObjectShape(ValueMap object)
{
	auto position = Point(object["x"].asFloat() / PTMRATIO, object["y"].asFloat() / PTMRATIO);
	float width = object["width"].asFloat() / PTMRATIO;
	float height = object["height"].asFloat() / PTMRATIO;

	Vec2 vertices[4];
	int vindex = 4;

	vertices[0].x = position.x + 0.0f;
	vertices[0].y = position.y + 0.0f;

	vertices[1].x = position.x + 0.0f;
	vertices[1].y = position.y + height;

	vertices[2].x = position.x + width;
	vertices[2].y = position.y + height;

	vertices[3].x = position.x + width;
	vertices[3].y = position.y + 0.0f;

	PhysicsShapePolygon* rectShape = PhysicsShapePolygon::create(vertices, vindex);
	return rectShape;
}


PhysicsShape*  TiledMapManager::createBody(cocos2d::ValueMap object)
{
	return createObjectShape(object);
}

void TiledMapManager::initCollisionMap(TMXTiledMap* map)
{
	auto collisionGroup = map->getObjectGroup("Collision");
	cocos2d::ValueVector collisionObjects = collisionGroup->getObjects();

	auto collisionBody = PhysicsBody::create();
	collisionBody->setDynamic(false);
	map->getLayer("earth")->getTileAt(Vec2(0, MAP_SIZE))->setAnchorPoint(Vec2(0.5, 0.5));
	map->getLayer("earth")->getTileAt(Vec2(0, MAP_SIZE))->setPhysicsBody(collisionBody);

	for (cocos2d::Value objectValue : collisionObjects)
	{
		auto bodyShape = createBody(objectValue.asValueMap());
		if (bodyShape != NULL) {
			collisionBody->addShape(bodyShape);
			collisionBody->setCategoryBitmask(2);
			collisionBody->setContactTestBitmask(0xFFFFFFFF);
			collisionBody->setCollisionBitmask(1+16);
			bodyShape->setRestitution(0);
			collisionBody->setTag(earthTag);
		}
	}
}


