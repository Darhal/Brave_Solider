#include "GameManager.h"
#include "Player.h"
#include "cocos2d.h"
#include "Inventory.h"
#define COCOS2D_DEBUG 1

#define __FLT_EPSILON__ 1.19209290e-07F
float  _filteredUpdateDelta;

using namespace cocos2d;

Scene* GameManager::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
	//GameManager::physicsWorld = scene->getPhysicsWorld();
	
    // 'layer' is an autorelease object
    auto layer = GameManager::create();

	scene->getPhysicsWorld()->setAutoStep(false);

	scene->getPhysicsWorld()->setGravity(Vec2(0,-650));
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameManager::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
    
    //auto rootNode = CSLoader::createNode("MainScene.csb");
	//BACKGROUND DRAW
	//auto sprt = Sprite::create("BG/BG.png");
	// position the sprite on the center of the screen
	/*->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	sprt->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(sprt)*/

	// PARALLAX EFFECT :

	auto bgs0 = Sprite::create("Backgrounds/_11_background.png");
	// position the sprite on the center of the screen
	bgs0->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	bgs0->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(bgs0); //_06_hill2

	auto bgs1 = Sprite::create("Backgrounds/_04_bushes.png");
	// position the sprite on the center of the screen
	bgs1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	bgs1->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(bgs1); //_06_hill2

	auto bgs2 = Sprite::create("Backgrounds/_03_distant_trees.png");
	// position the sprite on the center of the screen
	bgs2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	bgs2->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(bgs2);


	GameManager::bgs[1] = Sprite::create("Backgrounds/_10_distant_clouds.png");
	// position the sprite on the center of the screen
	GameManager::bgs[1]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	GameManager::bgs[1]->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(GameManager::bgs[1]);

	GameManager::bgs[3] = Sprite::create("Backgrounds/_08_clouds.png");
	// position the sprite on the center of the screen
	GameManager::bgs[3]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	GameManager::bgs[3]->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(GameManager::bgs[3]);

	GameManager::bgs[2] = Sprite::create("Backgrounds/_09_distant_clouds1.png");
	// position the sprite on the center of the screen
	GameManager::bgs[2]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	GameManager::bgs[2]->setContentSize(Size(visibleSize.width, visibleSize.height));
	addChild(GameManager::bgs[2]);

	//auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeNode = Node::create();
	edgeNode->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	//edgeNode->setPhysicsBody(edgeBody);
	addChild(edgeNode);



	// CREATE ENTITIES
	
	GameManager::player = new Player(this, edgeNode);
	

	GameManager::map = new TiledMapManager(this,edgeNode, GameManager::physicsWorld);


	auto followTheSprite = Follow::create(GameManager::player->PlayerSprite, 
		GameManager::map->map->getBoundingBox());
		//Rect::Rect(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2,
		//	GameManager::map->map->getMapSize().width,
		//	GameManager::map->map->getMapSize().height));
	//edgeNode->setScale(1.5);
	edgeNode->runAction(followTheSprite);

	//Camera* cam = Camera::create();
	//cam->runAction(followTheSprite);
	//map->map->addChild(cam);
	
	///////////////////////////////////////
	auto eventListener2 = EventListenerKeyboard::create();
	eventListener2->onKeyPressed = CC_CALLBACK_2(GameManager::onKeyPressed, this);
	eventListener2->onKeyReleased = CC_CALLBACK_2(GameManager::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener2, this);

	EventListenerPhysicsContact *contactListener =
		EventListenerPhysicsContact::create();
	contactListener->onContactBegin =
		CC_CALLBACK_1(GameManager::onContactBegin, this);

	contactListener->onContactPreSolve =
		CC_CALLBACK_2(GameManager::onContactPreSolve, this);

	/*contactListener->onContactPostSolve =
		CC_CALLBACK_2(GameManager::onContactPostSolve, this);*/

	contactListener->onContactSeparate =
		CC_CALLBACK_1(GameManager::onContactSeperate2, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,
		this);

	//
	//scheduleOnce(CC_SCHEDULE_SELECTOR(GameManager::updateReady), 1);

	
	this->scheduleUpdate();
    return true;
}

// THOSE THINGS FIX PHYSICS NO BOUNCE AND NO NASTY MOVMENT
void GameManager::update(float delta)
{
	Vec2 pos1 = bgs[1]->getPosition();
	Vec2 pos2 = bgs[2]->getPosition();
	Vec2 pos3 = bgs[3]->getPosition();

	pos1.x -= rand_0_1() * 2;
	pos2.x -= rand_0_1() * 2;
	pos3.x -= rand_0_1() * 2;

	if (pos1.x <= -(visibleSize.width*0.5f))
	{
		pos1.x = pos3.x+ visibleSize.width;
	}
	if (pos2.x <= -(visibleSize.width*0.5f))
	{
		pos2.x = pos1.x + visibleSize.width;
	}
	if (pos3.x <= -(visibleSize.width*0.5f))
	{
		pos3.x = pos2.x + visibleSize.width;
	}
	bgs[1]->setPosition(pos1);
	bgs[2]->setPosition(pos2);
	bgs[3]->setPosition(pos3);

	GameManager::updatePhysics(delta);
	if ((player->PlayerSprite->getPhysicsBody()->getVelocity().y < -150) && !player->isPlayerFalling) {
		player->isPlayerFalling = true;
		player->setPlayerAction(7);
	}
}


void GameManager::updatePhysics(float delta)
{
	// How many times to supstep the phyiscs engine per game update.
	const int phyiscsSubSteps = 3;

	// The delta is divided by the number of substeps, to sync the game step with the physics step.
	float dt = delta / static_cast<float>(phyiscsSubSteps);

	// Apply a low-pass filter to dt to smoot it out to make the phyiscs simulation smoother.
	_filteredUpdateDelta = dt > __FLT_EPSILON__ ? 0.15 * dt + 0.85 * _filteredUpdateDelta : 0.0;

	// Apply the number of substeps to the phyiscs engine for this game update.
	for (int i = phyiscsSubSteps; i>0; i--)
	{
		physicsWorld->step(_filteredUpdateDelta);
	}
}

void GameManager::updateReady(float delta)
{
	//this->getScene()->getPhysicsWorld()->setAutoStep(false);
	/*for (int i = 0; i < 3; ++i)
	{
		this->getScene()->getPhysicsWorld()->step(1 / 180.0f);
	}
	this->scheduleUpdate();*/
}

void GameManager::onContactSeperate()
{
	player->isPlayerFalling = true;
	player->setPlayerAction(7);
}

bool GameManager::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* bodyA = contact.getShapeA()->getBody();
	PhysicsBody* bodyB = contact.getShapeB()->getBody();
	
	// CHECK IF COLLIDED WITH DETECTOR TO START CHACE THE PLAYER
	if ((contact.getShapeB()->getTag() >= map->indexTagEnmDetector-10)) {
		if ((bodyA->getTag() == player->bodyTag)) {
			for (int i = 0; i < map->EnemiesRoot[contact.getShapeB()].size(); i++)
				map->EnemiesRoot[contact.getShapeB()].at(i)->performActionAI(this, player);
			detectedEnmPack = map->EnemiesRoot[contact.getShapeB()];
			CCLOG("Contact body detector !");
			return false;
		}
		return false;
	}
	else if ((contact.getShapeA()->getTag() >= map->indexTagEnmDetector-10)) {
		if ((bodyB->getTag() == player->bodyTag)) {
			for (int i = 0; i < map->EnemiesRoot[contact.getShapeB()].size(); i++)
				map->EnemiesRoot[contact.getShapeA()].at(i)->performActionAI(this, player);
			detectedEnmPack = map->EnemiesRoot[contact.getShapeA()];
			CCLOG("Contact body detector !");
			return false;
		}
		return false;
	}
	if ((bodyA->getTag() == player->bulletTag) && (bodyB->getTag() == 12)) {
		if ((bodyA->getNode() != nullptr)) {
			/*ParticleExplosion* dEffect = ParticleExplosion::create();
			dEffect->setVisible(true);
			dEffect->setScale(0.65);
			edgeNode->addChild(dEffect);
			dEffect->setPosition(bodyA->getNode()->getPosition());*/
			bodyA->getNode()->removeFromParentAndCleanup(true);
		}
		if ((bodyB->getNode() != nullptr)) {
			/*ParticleExplosion* dEffect = ParticleExplosion::create();
			dEffect->setVisible(true);
			dEffect->setScale(0.65);
			edgeNode->addChild(dEffect);
			dEffect->setPosition(bodyB->getNode()->getPosition());*/
			bodyB->getNode()->removeFromParentAndCleanup(true);
		}
		return false;
	}
	else if ((bodyB->getTag() == player->bulletTag) && (bodyA->getTag() == 12)) {
		if (bodyA->getNode() != nullptr) {
			bodyA->getNode()->removeFromParentAndCleanup(true);
		}
		if ((bodyB->getNode() != nullptr)) {
			bodyB->getNode()->removeFromParentAndCleanup(true);
		}
		return false;
	}
	if ((bodyA->getTag() == 9) && (bodyB->getTag() == 9)) {
		return false;
	}
	else if ((bodyB->getTag() == 9) && (bodyA->getTag() == 9)) {
		return false;
	}
	// PLAYER COLLIDE WITH EARTH AND VICE VERSA
	if ((bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == map->earthTag)) {
		//RESET TO PLAYER PRESSED ACTION OTHERWISE IDLE
		//Vec2 pp = bodyA->getPosition();
		//Vec2 epos = contact.getShapeB()->getOffset();
		//if (pp.y-epos.y > 160) {
			//CCLOG("BODY IS ON THE TOP OF THE EARTH (PLAYER POS %i | EARTH POS : %i)", pp.y, epos.y);
			player->isPlayerFalling = false;
			GameManager::resetToAction();
			// PARTICLE EFFECT DUST
			ParticleSystemQuad* dEffect = ParticleSystemQuad::create("ParticleEffect/dustEffect.plist");
			dEffect->setVisible(true);
			dEffect->setScale(0.25);
			player->PlayerSprite->addChild(dEffect);
			dEffect->setPosition(
				player->PlayerSprite->getContentSize().height / 2,
				0
			);
			GameManager::player->canJumpAgain = true;
		//}
	}
	else if ((bodyB->getTag() == player->bodyTag) && (bodyA->getTag() == map->earthTag)) {
		//RESET TO PLAYER PRESSED ACTION OTHERWISE IDLE
		//Vec2 pp = bodyB->getPosition();
		//Vec2 epos = contact.getShapeA()->getOffset();
		//if (pp.y - epos.y > 160) {
			//CCLOG("BODY IS ON THE TOP OF THE EARTH (PLAYER POS %i | EARTH POS : %i)", pp.y, epos.y);
			player->isPlayerFalling = false;
			GameManager::resetToAction();
			// PARTICLE EFFECT DUST
			ParticleSystemQuad* dEffect = ParticleSystemQuad::create("ParticleEffect/dustEffect.plist");
			dEffect->setVisible(true);
			dEffect->setScale(0.25);
			player->PlayerSprite->addChild(dEffect);
			dEffect->setPosition(
				player->PlayerSprite->getContentSize().height / 2,
				0
			);
			GameManager::player->canJumpAgain = true;
		//}
	}
	//FILTER FOR ENEMY BULLET AND PLAYER 
	if ((bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == 12))
	{
		// TAKE DAMAGE AND DESTROY THE BULLET !
		player->takeDamage(2);
		if (bodyB->getNode() == nullptr) { return false; }
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if ((bodyB->getTag() == player->bodyTag) && ((bodyA->getTag() == 12)))
	{
		// TAKE DAMAGE AND DESTROY THE BULLET !
		player->takeDamage(2);
		if (bodyA->getNode() == nullptr) { return false; }
		bodyA->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	// CHECK FORPLAYER BULLET AND ENEMY
	if ((bodyA->getTag() == player->bulletTag) && (bodyB->getTag() == 9))
	{
		// TAKE DAMAGE AND DESTROY THE BULLET !
		if (detectedEnmPack == emptyEnmPack) { CCLOG("EMPTY PACK"); return false; }
		for (int i = 0; i < detectedEnmPack.size(); i++) {
			if (detectedEnmPack.at(i)->getSprite()->getPhysicsBody() 
				&& (detectedEnmPack.at(i)->getSprite()->getPhysicsBody() == bodyB)) {
				detectedEnmPack.at(i)->takeDamage(50);
				//CCLOG("Damage enemy %01d ", i);
			}
		}
		if (bodyA->getNode() == nullptr) { return false; }
		bodyA->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if ((bodyB->getTag() == player->bulletTag) && ((bodyA->getTag() == 9)))
	{
		// TAKE DAMAGE AND DESTROY THE BULLET !
		if (detectedEnmPack == emptyEnmPack) { CCLOG("EMPTY PACK"); return false; }
		for (int i = 0; i < detectedEnmPack.size(); i++) {
			if (detectedEnmPack.at(i)->getSprite()->getPhysicsBody() && 
				(detectedEnmPack.at(i)->getSprite()->getPhysicsBody() == bodyA)) {
				//CCLOG("Damage enemy %01d ", i);
				detectedEnmPack.at(i)->takeDamage(50);
			}
		}
		if (bodyB->getNode() == nullptr) { return false; }
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	// FILTER PLAYER FOR BULLET AND COINS
	if ((bodyA->getTag() == player->bulletTag) && (bodyB->getTag() == map->coinTags['G']
		|| bodyB->getTag() == map->coinTags['S'] ||
		bodyB->getTag() == map->coinTags['B']))
	{
		return false;
	}
	else if ((bodyB->getTag() == player->bulletTag) && (bodyA->getTag() == map->coinTags['G']
		|| bodyA->getTag() == map->coinTags['S'] ||
		bodyA->getTag() == map->coinTags['B']))
	{
		return false;
	}
	if ((bodyA->getTag() == player->bulletTag) && !(bodyB->getTag() == map->coinTags['G']
		|| bodyB->getTag() == map->coinTags['S'] ||
		bodyB->getTag() == map->coinTags['B'] || (bodyB->getTag() >= map->indexTagEnmDetector)))
	{
		//bodyA->getNode()->cleanup();
		if (bodyA->getNode() == nullptr) { return false; }
		bodyA->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if ((bodyB->getTag() == player->bulletTag) && !(bodyA->getTag() == map->coinTags['G']
		|| bodyA->getTag() == map->coinTags['S'] ||
		bodyA->getTag() == map->coinTags['B'] || (bodyA->getTag() >= map->indexTagEnmDetector)))
	{
		//bodyB->getNode()->cleanup();
		if (bodyB->getNode() == nullptr) { return false; }
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	// NOTE : THIS WORKING FINE
	if ((bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == map->coinTags['G']
		|| bodyB->getTag() == map->coinTags['S'] ||
		bodyB->getTag() == map->coinTags['B'])) {
		if (bodyB->getNode() == nullptr) { return false; }
		
		if (bodyB->getTag() == map->coinTags['B']) {
			player->playerInventory->addItem(Inventory::Items::BRONZE, 1);
		}
		else if (bodyB->getTag() == map->coinTags['G'])
		{
			player->playerInventory->addItem(Inventory::Items::GOLD, 1);
		}
		else if (bodyB->getTag() == map->coinTags['S']) {
			player->playerInventory->addItem(Inventory::Items::SILVER, 1);
		}
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if ((bodyB->getTag() == player->bodyTag) && (bodyA->getTag() == map->coinTags['G']
		|| bodyA->getTag() == map->coinTags['S'] ||
		bodyA->getTag() == map->coinTags['B'])) {
		if (bodyA->getNode() == nullptr) { return false; }
		
		if (bodyA->getTag() == map->coinTags['B']) {
			player->playerInventory->addItem(Inventory::Items::BRONZE, 1);
		}
		else if (bodyA->getTag() == map->coinTags['G'])
		{
			player->playerInventory->addItem(Inventory::Items::GOLD, 1);
		}
		else if (bodyA->getTag() == map->coinTags['S']) {
			player->playerInventory->addItem(Inventory::Items::SILVER, 1);
		}
		bodyA->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	if ((bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == map->keyTag))
	{
		if (bodyB->getNode() == nullptr) { return false; }
		player->playerInventory->addItem(Inventory::Items::KEY, 1);
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if((bodyB->getTag() == player->bodyTag) && (bodyA->getTag() == map->keyTag))
	{
		if (bodyA->getNode() == nullptr) { return false; }
		player->playerInventory->addItem(Inventory::Items::KEY, 1);
		bodyB->getNode()->removeFromParentAndCleanup(true);
		return false;
	}

	if ((bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == map->chestTag)
		&& (player->playerInventory->getItemAmount(Inventory::Items::KEY)>=1))
	{
		if (bodyB->getNode() == nullptr) { return false; }
		//player->playerInventory->addItem(Inventory::Items::KEY, 1);
		map->playChestAnimation(bodyB->getNode(), bodyB->getName(), player);
		CCLOG("CHEST TRIGGERED !");
		bodyB->removeFromWorld();//getNode()->removeFromParentAndCleanup(true);
		return false;
	}
	else if ((bodyB->getTag() == player->bodyTag) && (bodyA->getTag() == map->chestTag)
		&& (player->playerInventory->getItemAmount(Inventory::Items::KEY)>=1))
	{
		if (bodyA->getNode() == nullptr) { return false; }
		map->playChestAnimation(bodyA->getNode(), bodyA->getName(), player);
		CCLOG("CHEST TRIGGERED !");
		//player->playerInventory->addItem(Inventory::Items::KEY, 1);
		bodyA->removeFromWorld();//getNode()->removeFromParentAndCleanup(true);
		return false;
	}

	return true;
}

// Calling it to eliminate The Jumping Effect... Restitution
bool GameManager::onContactPreSolve(PhysicsContact& contact,
	PhysicsContactPreSolve& solve)
{
	solve.setRestitution(0);
	return true;
}

void GameManager::onContactSeperate2(cocos2d::PhysicsContact & contact)
{
	PhysicsBody* bodyA = contact.getShapeA()->getBody();
	PhysicsBody* bodyB = contact.getShapeB()->getBody();

	if ((contact.getShapeB()->getTag() >= map->indexTagEnmDetector - 10)) {
		CCLOG("Contact detector !");
		if ((bodyA->getTag() == player->bodyTag)) {
			for (int i = 0; i < map->EnemiesRoot[contact.getShapeB()].size(); i++)
				map->EnemiesRoot[contact.getShapeB()].at(i)->isActive = false;
			detectedEnmPack = emptyEnmPack;
			CCLOG("Contact body detector !");
		}
	}
	else if ((contact.getShapeA()->getTag() >= map->indexTagEnmDetector - 10)) {
		CCLOG("Contact detector !");
		if ((bodyB->getTag() == player->bodyTag)) {
			for (int i = 0; i < map->EnemiesRoot[contact.getShapeB()].size(); i++)
				map->EnemiesRoot[contact.getShapeB()].at(i)->isActive = false;
			detectedEnmPack = emptyEnmPack;
			CCLOG("Contact body detector !");
		}
	}
	/*if ((!keyStatus[EventKeyboard::KeyCode::KEY_UP_ARROW]) && 
		(bodyB->getTag() == player->bodyTag) && (bodyA->getTag() == map->earthTag))
	{
		player->isPlayerFalling = true;
		player->setPlayerAction(7);
		//if ((GameManager::player->isPlayerFalling)) {
			GameManager::player->canJumpAgain = false;
		//};
	}else if ((!keyStatus[EventKeyboard::KeyCode::KEY_UP_ARROW]) && 
		(bodyA->getTag() == player->bodyTag) && (bodyB->getTag() == map->earthTag))
	{
		player->isPlayerFalling = true;
		player->setPlayerAction(7);
		//if ((GameManager::player->isPlayerFalling)) {
			GameManager::player->canJumpAgain = false;
		//};
	}*/
}


// END OF THOSE THINGS FIX PHYSICS NO BOUNCE AND NO NASTY MOVMENT

void GameManager::resetToAction()
{
	GameManager::player->stopMovment();
	if (keyStatus[EventKeyboard::KeyCode::KEY_DOWN_ARROW]) {
		player->setPlayerAction(1);
		GameManager::player->isPlayerSliding = false;
		/*GameManager::player->movePlayer(0, 0, 5, false);
		GameManager::player->updatePosition(true, 15, 0);*/
	}else if (keyStatus[EventKeyboard::KeyCode(27)]) {
		GameManager::player->setPlayerMvtDir(false);
		GameManager::player->movePlayer(10, 0, 0, false);
	}else if (keyStatus[EventKeyboard::KeyCode(26)]) {
		GameManager::player->setPlayerMvtDir(true);
		GameManager::player->movePlayer(-10, 0, 0, false);
	}
	else if ((keyStatus[EventKeyboard::KeyCode::KEY_UP_ARROW])) {
		// UP KEY
		GameManager::player->movePlayer(0, 0, 6, true);
		GameManager::player->updatePosition(true, 0, 25);
		//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::onContactSeperate,
		//	this));
		//this->runAction(Sequence::create(DelayTime::create(0.9), runCallback, nullptr));
		//scheduleOnce(CC_SCHEDULE_SELECTOR(GameManager::onContactSeperate), 0.09f);
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::onContactSeperate, this));
		edgeNode->runAction(Sequence::create(DelayTime::create(0.07), runCallback, nullptr));
		if ((GameManager::player->isPlayerFalling)) {
			GameManager::player->canJumpAgain = false;
		}
	}
	else {
		player->setPlayerAction(1);
		GameManager::player->stopMovment();
	}
	player->PlayerSprite->getPhysicsBody()->setAngularDamping(0);
	player->PlayerSprite->getPhysicsBody()->setAngularVelocity(0);
	GameManager::player->isPlayerSliding = false;
}

void GameManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	// STOP ACTION AND CHECK  IF HERE IS ANY OTHER BUTTON PRESSED
	// THEN PERFORM ACTION WHICH ASSIGNED TO THE PRESSED BUTTON
	keyStatus[keyCode] = false;
	if ((keyCode == EventKeyboard::KeyCode(26)) || (keyCode == EventKeyboard::KeyCode(27))) 
	{
		GameManager::player->stopMovment();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		GameManager::player->isPlayerSliding = false;
	}
	
	GameManager::resetToAction();
}

void GameManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	/////////////////////////JUMPING HAVE THE PIRIORTY/////////////////////////////////////
	if ((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) &&
		(GameManager::player->canJumpAgain)) {
		// UP KEY
		GameManager::player->movePlayer(0, 0, 6, true);
		GameManager::player->updatePosition(true, 0, 25);
		//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::onContactSeperate,
		//	this));
		//this->runAction(Sequence::create(DelayTime::create(0.9), runCallback, nullptr));
		//scheduleOnce(CC_SCHEDULE_SELECTOR(GameManager::onContactSeperate), 0.09f);
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::onContactSeperate, this));
		edgeNode->runAction(Sequence::create(DelayTime::create(0.07), runCallback, nullptr));
		keyStatus[keyCode] = true;
		if ((GameManager::player->isPlayerFalling)) {
			GameManager::player->canJumpAgain = false;
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	if ((keyCode == EventKeyboard::KeyCode(26) && !(GameManager::player->isPlayerSliding))) {
		GameManager::player->setPlayerMvtDir(true);
		GameManager::player->movePlayer(-10, 0, 0, false);
		//CCLOG("LEFT KEY PRESSED !");
		keyStatus[keyCode] = true;
	}

	///////////////////////////////////////////////////////////////////////////
	if ((keyCode == EventKeyboard::KeyCode(27) && !(GameManager::player->isPlayerSliding))) {
		//Right Key
		GameManager::player->setPlayerMvtDir(false);
		GameManager::player->movePlayer(10, 0, 0, false);
		//CCLOG("RIGHT KEY PRESSED !");
		keyStatus[keyCode] = true;
	}

	///////////////////////////////////////////////////////////////////////
	if ((keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) && 
		((!(player->isPlayerFalling)) || GameManager::player->isPlayerSliding)
		&& (!keyStatus[EventKeyboard::KeyCode(26)] ||
		!(keyStatus[EventKeyboard::KeyCode(27)]))) {
		// DOWN KEY
		GameManager::player->movePlayer(0, 0, 5, true);
		GameManager::player->updatePosition(true, 25, 0);
		keyStatus[keyCode] = true;
		// return to pressed action after some seconds
		//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::resetToAction,
		//	this));
		//this->runAction(Sequence::create(DelayTime::create(10), runCallback, nullptr));
		GameManager::player->isPlayerSliding = true;
		//scheduleOnce(CC_SCHEDULE_SELECTOR(GameManager::resetToAction), 2);
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(GameManager::resetToAction, this));
		edgeNode->runAction(Sequence::create(DelayTime::create(1), runCallback, nullptr));
	}


	if ((keyCode == EventKeyboard::KeyCode::KEY_LEFT_CTRL)) {
		// left ctrl KEY
		GameManager::player->movePlayer(0, 0, 4, false);
		keyStatus[keyCode] = true;
	}

}