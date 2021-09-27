#include "Enemies.h"
#include "cocos2d.h"

#define COCOS2D_DEBUG 1 //for debug

USING_NS_CC;
const int  totalAnims = 7;//9;
std::map<int, cocos2d::Vector<cocos2d::Action*>> enmActionsOpt;
bool animLoaded[4];

char* Enemies::getAnimPath(int id)
{
	switch (id){
	case 0:
		return "entities/Enemies/%01d/idle/idle (%01d).png";
			break;
	case  1:
		return "entities/Enemies/%01d/walk/walk (%01d).png";
		break;
	case 2:
		return "entities/Enemies/%01d/run/run (%01d).png";
		break;
	case 3:
		return "entities/Enemies/%01d/jump/jump (%01d).png";
		break;
	case 4:
		return "entities/Enemies/%01d/hurt/hurt (%01d).png";
			break;
	case 5:
		return "entities/Enemies/%01d/attack1/attack (%01d).png";
		break;
	case 6:
		return "entities/Enemies/%01d/hurt/hurt (%01d).png";
		break;
	default:
		return "NOT FOUND ";
		break;
	}
}

Enemies::Enemies(cocos2d::Layer* layer, cocos2d::Node* node, cocos2d::TMXTiledMap* map,
				int xC, int yC, int id)
{
	if (!id || id == 0) {
		id = 1;
	}
	enmSprite = Sprite::create("entities/Enemies/3/idle/Idle (1).png");
	enmSprite->setScale(0.5f);
	auto enmBody = PhysicsBody::createCircle(enmSprite->getContentSize().height / 2.3,
		PhysicsMaterial(0.0f, 1.0f, 0.0f));
	enmSprite->setAnchorPoint(Vec2(0.2f, 0.2f));
	enmBody->setCategoryBitmask(16);
	enmBody->setContactTestBitmask(0xFFFFFFFF);
	enmBody->setCollisionBitmask(1+2);
	enmBody->setTag(bodyTag);
	enmBody->setGravityEnable(true);
	enmBody->setAngularVelocity(0);
	enmSprite->addComponent(enmBody);
	enmSprite->setPhysicsBody(enmBody);
	enmBody->setDynamic(true);
	//id = 1;
	loadAnimations(id);
	valueToChangeAction = random(270, 350); // set it   here better than updating this
	node->addChild(enmSprite);
	health = 100;
	enmId = id;
	enmSprite->setPosition(Vec2(xC, yC));
}

void Enemies::performActionAI(Layer* layer, Player* plr)
{
	if (!isActive) {
		Enemies::plr = plr;
		//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::update, this));
		//layer->runAction(Sequence::create(DelayTime::create(1), runCallback, nullptr));
		isActive = true;
		mainLayer = layer;
		Enemies::update();
	}
}

Enemies::~Enemies()
{

}

void Enemies::decreaseAlpha()
{
	if (sprtAlpha <= 0) {
		//Enemies::~Enemies();//DESTROY THE OBJECT
		//enmSprite->removeFromParentAndCleanup(true);
		return;
	}
	// TODO : CLEAN UP STUFF COMING SOON
	sprtAlpha -= 10;
	enmSprite->setOpacity(sprtAlpha);
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::decreaseAlpha, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(0.15), runCallback, nullptr));
	freezeEnemy = true;
	isActive = false;
	//Enemies::changeAction(6);
}

void Enemies::kill()
{
	/*ParticleExplosion* dEffect = ParticleExplosion::create();
	dEffect->setVisible(true);
	dEffect->setScale(0.65);
	mainLayer->addChild(dEffect);
	dEffect->setPosition(enmSprite->getPosition());*/
	Enemies::changeAction(6);
	freezeEnemy = true;
	isActive = false;
	if (enmSprite->getPhysicsBody() != nullptr) {
		enmSprite->getPhysicsBody()->removeFromWorld();
	}
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::decreaseAlpha, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(0.2), runCallback, nullptr));
	
}

void Enemies::finishDamageEffect()
{
	Enemies::changeAction(0);
	enmSprite->setColor(Color3B::WHITE);
	freezeEnemy = false;
	Enemies::update();
}

void Enemies::takeDamage(int dmg)
{
	if (freezeEnemy) { return; }
	health -= dmg;
	if (health <= 0) {
		Enemies::kill();
		return;
	}
	Enemies::changeAction(4);
	enmSprite->setColor(Color3B::RED);
	freezeEnemy = true;
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::finishDamageEffect, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(0.1), runCallback, nullptr));
}

void Enemies::attack()
{
	
	if (freezeEnemy) { return; }
	if (health <= 0) {
		Enemies::kill();
		return;
	}
	auto eBody = enmSprite->getPhysicsBody();
	eBody->setVelocity(Vec2(0,0));
	Enemies::changeAction(5);
	Enemies::loadBullet();
	valueToChangeAction = random(270, 350); // set it   here better than updating this
}

// * SHOOTING ENEMY BULLET SPEED vx *
void Enemies::loadBullet()
{
	// load the sprite
	const char* path;
	if (enmId == 3 || enmId == 2)
		path = "player/bullets/bullet1.png";
	else if (enmId == 1)
		path = "player/bullets/bullet4.png";
	Sprite* bulletSprite = Sprite::create(path);
	bulletSprite->setScale(0.08f);
	plr->mainNode->addChild(bulletSprite, 11);
	// set it to the player
	if (mvtDir) // left
		bulletSprite->setPosition(
			enmSprite->getPosition().x - (enmSprite->getContentSize().width)*0.05,
			(enmSprite->getPosition().y+(enmSprite->getContentSize().height*0.13)));
	else  // right
		bulletSprite->setPosition(
			enmSprite->getPosition().x + (enmSprite->getContentSize().width)*0.05,
			(enmSprite->getPosition().y+(enmSprite->getContentSize().height*0.13)));
	bulletSprite->setFlippedX(mvtDir); // set the same direction as the player
									   //create physics body for it : a circle for exemple
	PhysicsBody* bBody = PhysicsBody::createCircle(bulletSprite->getContentSize().width / 2);
	bBody->setGravityEnable(false); // NOT EFFECTED BY GRAVITY
	bulletSprite->setPhysicsBody(bBody);
	long long int vx = 350;
	mvtDir ? vx = -vx : vx;
	bBody->setVelocity(Vec2(vx, 0));
	
	bBody->setCategoryBitmask(32);
	bBody->setContactTestBitmask(0xFFFFFFFF);
	bBody->setCollisionBitmask(1+2);
	//bBody->setContactTestBitmask(0xFFFFFFFF);
	bBody->setTag(eBulletTag);

	// PARTICLE EFFECTS
	const char* pathEffect;
	if (enmId == 3 || enmId == 2)
		pathEffect = "ParticleEffect/blueFire.plist";
	else if (enmId == 1)
		pathEffect = "ParticleEffect/FireEffect.plist";
	ParticleSystemQuad* bEffect = ParticleSystemQuad::create(pathEffect);
	bEffect->setVisible(true);
	bEffect->setScale(0.85);
	bulletSprite->addChild(bEffect);
	bEffect->setPosition(0, bulletSprite->getContentSize().width / 4);

	//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::cleanUpBullets,this, bulletSprite));
	//mainLayer->runAction(Sequence::create(DelayTime::create(10), runCallback, nullptr));
}

void Enemies::cleanUpBullets(cocos2d::Sprite* sprt)
{
	if (sprt->getParent()) {
		sprt->removeFromParentAndCleanup(true);
	}
	
}
	

void Enemies::update() {
	// UPDATING TO TAKE ACTION THAT FIT THE SITUATION
	Vec2 pPos = plr->PlayerSprite->getPhysicsBody()->getPosition();
	Vec2 ePos = enmSprite->getPhysicsBody()->getPosition();
	Enemies::facePlayer(ePos, pPos);

	if (isActive) {
		if (health <= 0) {
			Enemies::kill();
			return;
		}
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::update, this));
		mainLayer->runAction(Sequence::create(DelayTime::create(1), runCallback, nullptr));
	}
	else {
		if (health <= 0 && !freezeEnemy) {
			Enemies::kill();
			return;
		}
		Enemies::stopMovement();
	}
	if (freezeEnemy) { ; return; }
	if ((abs(pPos.x-ePos.x) <=  (valueToChangeAction)) && 
		(abs(pPos.y - ePos.y) <= (valueToChangeAction*0.25))) {
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::attack, this));
		CallFunc *runCallback1 = CallFunc::create(CC_CALLBACK_0(Enemies::moveToPlayer, this));
		mainLayer->runAction(Sequence::create(DelayTime::create(rand_0_1()),
			runCallback, DelayTime::create(rand_0_1()), runCallback1, 
			nullptr));
	}else if ((long long int ) abs(pPos.x - ePos.x) >= valueToChangeAction) {
			Enemies::moveToPlayer();
	}
	else {
			Enemies::stopMovement();
	}
	if (health <= 0 && !freezeEnemy) {
		Enemies::kill();
		return;
	}
	
}

void Enemies::facePlayer(Vec2 ePos, Vec2 pPos)
{
	auto ebody = enmSprite->getPhysicsBody();
	if ((pPos.x - ePos.x) < -50) { // FLIP TO THE LEFT
		enmSprite->setFlipX(true);
		mvtDir = true;
		//Enemies::update();
		if ((eLastAction == 2)) { // if he is moving change his movment direction
			Vec2 v = ebody->getVelocity();
			ebody->setVelocity(Vec2(0, v.y));
			ebody->setVelocity(Vec2(random(-250, -200), v.y));
		}
	}
	else if ((pPos.x - ePos.x) > 50) {
		enmSprite->setFlipX(false);
		mvtDir = false;
		//Enemies::update();
		if ((eLastAction == 2)) { // if he is moving change his movment direction
			Vec2 v = ebody->getVelocity();
			ebody->setVelocity(Vec2(0, v.y));
			ebody->setVelocity(Vec2(random(200, 250), v.y));
		}
	}
}

void Enemies::moveToPlayer()
{
	//if (eLastAction == 2) return; // he is already moving
	auto ebody = enmSprite->getPhysicsBody();
	if (!(eLastAction == 2) || !isMoving) {
		Enemies::changeAction(2);
		Vec2 pPos = plr->PlayerSprite->getPhysicsBody()->getPosition();
		Vec2 ePos = enmSprite->getPhysicsBody()->getPosition();
		
		Enemies::facePlayer(ePos, pPos);
		if ((pPos.x - ePos.x) < -50) { // FLIP TO THE LEFT
			Vec2 v = ebody->getVelocity();
			ebody->setVelocity(Vec2(random(-250, -200), v.y));
			//Enemies::update();
		}
		else if ((pPos.x - ePos.x) > 50) {
			Vec2 v = ebody->getVelocity();
			ebody->setVelocity(Vec2(random(200, 250), v.y));
			//Enemies::update();
		}
		isMoving = true;
	}
	else {
		Vec2 ePos = ebody->getPosition();
		Vec2 eV = ebody->getVelocity();
		if ((eLastPos != Vec2::ZERO) && ((abs(eLastPos.x-ePos.x)<200))) { // THEN HE IS STUCK
			//CCLOG("ENEMY GOING TO JUMP !");
			Enemies::jump(false);
		}
		//TODO : JUMP WHILE FALLING 
		if (((eV.y < -50) && (eLastAction != 3))) {
			//CCLOG("ENEMY GOING TO DO SUPER JUMP !");
			Enemies::jump(true);
		}
	}
	eLastPos = ebody->getPosition();
}

void Enemies::helpJump()
{
	Vec2 jumpVal;
	Vec2 eV = enmSprite->getPhysicsBody()->getVelocity();
	Vec2 pPos = plr->PlayerSprite->getPhysicsBody()->getPosition();
	Vec2 ePos = enmSprite->getPhysicsBody()->getPosition();
	int moveby = abs(ePos.x-pPos.x)*0.75;
	mvtDir ? moveby = -350 : moveby = 350;
	jumpVal = Vec2(moveby, eV.y);
	if (true)
		jumpVal = Vec2(moveby, eV.y);
	else
		jumpVal = Vec2(moveby, eV.y);
	enmSprite->getPhysicsBody()->setVelocity(jumpVal);
	//CCLOG("MOVING FORWARD AFTER JUMP !");
}

void Enemies::jump(bool superJump)
{
	Vec2 jumpVal;
	
	if (superJump)
		jumpVal = Vec2(0, random(500, 560));
	else 
		jumpVal = Vec2(0, random(340, 400));
	enmSprite->getPhysicsBody()->setVelocity(jumpVal);
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::helpJump, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(0.07), runCallback, nullptr));
	Enemies::changeAction(3);
}

void Enemies::stopMovement()
{
	if (isMoving) {
		Enemies::changeAction(0);
		auto eBody = enmSprite->getPhysicsBody();
		Vec2 v = eBody->getVelocity();
		enmSprite->getPhysicsBody()->setVelocity(Vec2(0, 0));
		isMoving = false;
		//Enemies::update();
	}
}

void Enemies::changeAction(int id)
{
	/*if (id == 1)
		enmActions = enmActions1;
	else if (id == 2)
		enmActions = enmActions2;
	else if (id == 3)
		enmActions = enmActions3;*/
	if (freezeEnemy) {
		return;
	}
	if ((id != eLastAction)) {
		if ((eLastAction == -1) || (enmSprite->getNumberOfRunningActions() == 0)) {
			//CCLOG("ACTION START ACTION TO RUN : %01d ", id);
			enmSprite->runAction(enmActions.at(id));
			eLastAction = id;
		}else{
			//CCLOG("ACTION STOP : %01d, ACTION TO RUN : %01d ", eLastAction, id);
			enmSprite->stopAction(enmActions.at(eLastAction));
			enmSprite->runAction(enmActions.at(id));
			eLastAction = id;
		}
	}
}

Sprite* Enemies::getSprite()
{
	return enmSprite;
}


void Enemies::loadAnimations(int id)
{
	//FOR LOOP TO LOAD ALL ANIMATIONS AND SPRITES
	//std::vector<cocos2d::Action*> enmActions;
	//std::map<int, cocos2d::Vector<cocos2d::Action*>> enmActionsOpt;
	if (animLoaded[id]) { // its already loaded let just copy it
		for (int i = 0; i < totalAnims; i++) {
			Action* tmpAct = enmActionsOpt[id].at(i)->clone();
			enmActions.pushBack(tmpAct);
		}
		//enmActions = enmActionsOpt[id]; 
		CCLOG("REUSING ALREADY LOADED ANIMATION FOR ID %i !", id);
		return; 
	} 
	enmActions.reserve(totalAnims);
	struct enmAnimData {
		cocos2d::Vector<cocos2d::SpriteFrame*> Frames;
		cocos2d::Animation* AnimLoad;
		cocos2d::Animate* Anim;
		cocos2d::Sprite* Sprite;
		cocos2d::Action* eAction;
	}enmData[10];
	/*if (id == 1)
		enmActions = enmActions1;
	else if (id == 2)
		enmActions = enmActions2;
	else if (id == 3)
		enmActions = enmActions3;*/

	
	//
	struct animPack {
		char* path[250];
		int num;
	}packData[8];	/*
					ALL ANIMATIONS WITH ID:
					*/
	packData[0].path[250] = "entities/Enemies/%01d/idle/idle (%01d).png";
	packData[0].num = 8;

	packData[1].path[250] = "entities/Enemies/%01d/walk/walk (%01d).png";
	packData[1].num = 8;

	packData[2].path[250] = "entities/Enemies/%01d/run/run (%01d).png";
	packData[2].num = 5;

	packData[3].path[250] = "entities/Enemies/%01d/jump/jump (%01d).png";
	packData[3].num = 8;

	packData[4].path[250] = "entities/Enemies/%01d/hurt/hurt (%01d).png";
	packData[4].num = 6;

	packData[5].path[250] = "entities/Enemies/%01d/attack1/attack (%01d).png";
	packData[5].num = 6;

	packData[6].path[250] = "entities/Enemies/%01d/hurt/hurt (%01d).png";
	packData[6].num = 9;

	
	for (int i = 0; i < totalAnims; i++) {
		char str[50] = { 0 };
		char* path = Enemies::getAnimPath(i);//packData[i].path[250];//"entities/Enemies/3/idle/Idle (%01d).png";//(char*) packData[0].path;
		enmData[i].Frames.reserve(packData[i].num);
		for (int k = 1; k <= packData[i].num; ++k)
		{
			sprintf(str, path, id,k);
			auto sprt = Sprite::create(str);
			//we assume that the sprites' dimentions are w*h rectangles.
			SpriteFrame* frame = SpriteFrame::create(str, Rect(0, 0,
				sprt->getContentSize().width,
				sprt->getContentSize().height)
			);
			frame->setAnchorPoint(Vec2(0.2f, 0.2f));
			enmData[i].Frames.pushBack(frame);
			sprt->removeFromParentAndCleanup(true);
		}

		if (i != 6) // DEAD ANIMATION SHOULD BE PLAYED ONCE !
		{
			enmData[i].AnimLoad = Animation::createWithSpriteFrames(enmData[i].Frames, 0.09f);
			enmData[i].Anim = Animate::create(enmData[i].AnimLoad);
			enmData[i].eAction = RepeatForever::create(enmData[i].Anim);
		}
		else {
			enmData[i].AnimLoad = Animation::createWithSpriteFrames(enmData[i].Frames, 0.02f);
			enmData[i].Anim = Animate::create(enmData[i].AnimLoad);
			enmData[i].eAction = Repeat::create(enmData[i].Anim, 1);
		}
		
		enmActions.pushBack(enmData[i].eAction); //
		enmActionsOpt[id].pushBack(enmData[i].eAction);
		CCLOG("LOADING ENEMY ID : %01d, ANIMATION ID : %01d", id, i);
	}
	animLoaded[id] = true;
	Enemies::changeAction(0);
}
