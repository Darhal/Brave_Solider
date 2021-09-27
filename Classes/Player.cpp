#include "Player.h"
#include "cocos2d.h"
#include "HUD.h"
#include "Inventory.h"

#define COCOS2D_DEBUG 1
#define totalAnims 9

USING_NS_CC;

const int h = 641 ;
const int w = 542 ;
bool movePlayerBool;
bool mvtDir;

/*
HELPER FUNCTION TO GET THE PATH :
*/
char* getAnimPath(int id)
{
	switch (id) {
	case 0:
		return "player/Run (%01d).png";
		break;
	case 1: 
		return "player/Idle (%01d).png";
		break;
	case 2: 
		return "player/Dead (%01d).png";
		break;
	case 3: 
		return  "player/Melee (%01d).png";
		break;
	case 4:
		return "player/Shoot (%01d).png";
		break;
	case 5: return "player/Slide (%01d).png";
		break;
	case 6: return "player/Jump (%01d).png";
		break;
	case 7: return  "player/Fall (%01d).png";
		break;
	case 8: // HURT
		return  "player/Dead (%01d).png";
		break;
	default:
		return "NOT FOUND";
		break;
	}
}

Action* Player::getPlayerActionByID(int id)
{
	return plrActions.at(id);
}

void Player::setPlayerAction(int id)
{
	// BLOCK ACTIONS!
	if (blockActions) { 
		if (health <= 0){
			PlayerSprite->stopAction(Player::getPlayerActionByID(mvtActionId));
			Player::helpFunctionOfPlayerAction(2);
			Player::mvtActionId = 2;
		}
		return; 
	}
	//DISABLE ANY OTHER ANIMATION WHILE MOVING DOWN !
	if (!((isPlayerFalling == true) && ((id == 0) || (id == 5) || (id == 1)))) 
	{
		if (!Player::lastAction) {
			Player::lastAction = Player::getPlayerActionByID(id);
			PlayerSprite->runAction(Player::getPlayerActionByID(id));
			Player::mvtActionId = id;
			if (id == 1) {
				PlayerSprite->getPhysicsBody()->setVelocity(Vec2(0, 0));
			}
		}
		else {
			//"STOPING LAST ACTION AND RUN NEW ONE !";
			PlayerSprite->stopAction(Player::getPlayerActionByID(mvtActionId));
			Player::helpFunctionOfPlayerAction(id);
			Player::mvtActionId = id;
			if (id == 4) { // he is shooting create a bullet
				Player::loadBullets();
			}
			if (id == 1) {
				PlayerSprite->getPhysicsBody()->setVelocity(Vec2(0, 0));
			}
		}
	}
}

void Player::helpFunctionOfPlayerAction(int id)
{
	if (blockActions) { return; }
	lastAction = Player::getPlayerActionByID(id);
	PlayerSprite->runAction(Player::getPlayerActionByID(id));
}


Player::Player(cocos2d::Layer* Layer, cocos2d::Node* node)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	PlayerSprite = Sprite::create("player/Idle (1).png");
	Player::createPlayerPhysics();
	PlayerSprite->setScale(0.35f);
	Player::loadAnimations();
	PlayerSprite->setAnchorPoint(Vec2(0.5, 0.5)); // HERE WHEN ITS  Vec2(0, -0.5) 
												// THE BOUNCING BUG IS FIXED
	node->addChild(PlayerSprite, 100);
	mainNode = node;
	Player::isPlayerFalling = true;
	Player::setPlayerAction(7);
	//ProgressBar(Layer, "entities/progressbar/progress.png", "entities/progressbar/background.png");
	//healthBar->target_percentage(0.4);
	health = 100;
	mainLayer = Layer;
	PlayerHud = new HUD(Layer, health);
	playerInventory = new Inventory(Layer);
	CCLOG("SUCCESS : PLAYER CREATED !");
}

void Player::createPlayerPhysics()
{
	// Done FIX PLAYER BODY PHYSICS !
	auto plrBody1 = PhysicsBody::createCircle(PlayerSprite->getContentSize().height/2.3,
	PhysicsMaterial(0.0f, 0.0f, 1.0f));
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	plrBody1->resetForces();
	plrBody1->setCategoryBitmask(1);
	plrBody1->setContactTestBitmask(0xFFFFFFFF);
	plrBody1->setCollisionBitmask(2+4);
	plrBody1->setTag(bodyTag);
	plrBody1->setAngularVelocity(0);
	PlayerSprite->addComponent(plrBody1);
	PlayerSprite->setPhysicsBody(plrBody1);
	//plrBody1->setDynamic(false);

	// position the sprite on the center of the screen
	PlayerSprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + (visibleSize.height / 2)));

	//plrBody1->setPositionOffset(Vec2(0, 0));
}


void Player::loadBullets()
{
	// load the sprite
	const char* path = "player/bullets/bullet3.png";
	Sprite* bulletSprite = Sprite::create(path);
	bulletSprite->setScale(0.075f);
	mainNode->addChild(bulletSprite, 11);
	// set it to the player
	if (mvtDir) // left
		bulletSprite->setPosition(
		PlayerSprite->getPosition().x - (PlayerSprite->getContentSize().width)*.35, 
		PlayerSprite->getPosition().y);
	else  // right
			bulletSprite->setPosition(
			PlayerSprite->getPosition().x + (PlayerSprite->getContentSize().width)*0.35,
			PlayerSprite->getPosition().y);
	bulletSprite->setFlippedX(mvtDir); // set the same direction as the player
	//create physics body for it : a circle for exemple
	PhysicsBody* bBody = PhysicsBody::createCircle(bulletSprite->getContentSize().width/2);
	bBody->setGravityEnable(false); // NOT EFFECTED BY GRAVITY
	bulletSprite->setPhysicsBody(bBody);
	long long int vx = 350;
	mvtDir ? vx = -vx : vx;
	bBody->setVelocity(Vec2(vx, 0));
	//CCLOG("SHOOTING BULLET SPEED %d ", vx);
	bBody->setCategoryBitmask(32);
	bBody->setContactTestBitmask(0xFFFFFFFF);
	bBody->setCollisionBitmask(16+2);
	//bBody->setContactTestBitmask(0xFFFFFFFF);
	bBody->setTag(bulletTag);

	// PARTICLE EFFECTS
	ParticleSystemQuad* bEffect = ParticleSystemQuad::create("ParticleEffect/FireEffect.plist");
	bEffect->setVisible(true);
	bEffect->setScale(0.85f);
	bulletSprite->addChild(bEffect);
	bEffect->setPosition(0, bulletSprite->getContentSize().width/4);
}

void Player::kill()
{
	Player::setPlayerAction(2);
	blockActions = true;
	if (PlayerSprite->getPhysicsBody() != nullptr) {
		PlayerSprite->getPhysicsBody()->removeFromWorld();
	}
	PlayerHud->showLoseMenu();
	//CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Enemies::decreaseAlpha, this));
	//mainLayer->runAction(Sequence::create(DelayTime::create(0.2), runCallback, nullptr));
}

void Player::takeDamage(int dmg)
{
	//CCLOG("PLAYER TAKING DAMAGE : %01d ! ", dmg);
	if (health <= 0) {
		Player::kill();
	}
	if (blockActions) { return; }
	health = (health - dmg);
	PlayerHud->scroll_to_percentage(health);
	Player::setPlayerAction(8);
	PlayerSprite->setColor(Color3B(255, 0, 0));
	blockActions = true;
	Vec2 pv = PlayerSprite->getPhysicsBody()->getVelocity();
	PlayerSprite->getPhysicsBody()->setVelocity(Vec2(0, pv.y));
	CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Player::finishDamageEffect, this));
	mainLayer->runAction(Sequence::create(DelayTime::create(0.07f), runCallback, nullptr));
}

void Player::finishDamageEffect()
{
	blockActions = false;
	PlayerSprite->setColor(Color3B::WHITE);
	Player::setPlayerAction(1);
}

void Player::loadAnimations()
{
	//FOR LOOP TO LOAD ALL ANIMATIONS AND SPRITES
	plrActions.reserve(totalAnims);
	struct plrAnimData {
		cocos2d::Vector<cocos2d::SpriteFrame*> Frames;
		cocos2d::Animation* AnimLoad;
		cocos2d::Animate* Anim;
		cocos2d::Sprite* Sprite;
		cocos2d::Action* plrAction;
	};

	struct animPack {
		char* path[50];
		int num;
	}packData[8];	/*
	ALL ANIMATIONS WITH ID:
	*/
	packData[0].path[50] = ("player/Run (%01d).png");
	packData[0].num = 8;

	packData[1].path[50] = ("player/Idle (%01d).png");
	packData[1].num = 10;

	packData[2].path[50] = ("player/Dead (%01d).png");
	packData[2].num = 10;

	packData[3].path[50] = ("player/Melee (%01d).png");
	packData[3].num = 7;

	packData[4].path[50] = ("player/Shoot (%01d).png");
	packData[4].num = 3;

	packData[5].path[50] = ("player/Slide (%01d).png");
	packData[5].num = 5;

	packData[6].path[50] = ("player/Jump (%01d).png");
	packData[6].num = 7;

	packData[7].path[50] = ("player/Fall (%01d).png");
	packData[7].num = 3;

	packData[8].path[50] = ("player/Dead (%01d).png");
	packData[8].num = 2;
	

	plrAnimData plrData[10];
	for (int i = 0; i < totalAnims; i++) {
		char str[50] = { 0 };
		char* path = getAnimPath(i);

		plrData[i].Frames.reserve(packData[i].num);
		for (int k = 1; k <= packData[i].num; ++k)
		{
			sprintf(str, path, k);
			//we assume that the sprites' dimentions are w*h rectangles.
			SpriteFrame* frame = SpriteFrame::create(str, Rect(0, 0, 
				PlayerSprite->getContentSize().width, 
				PlayerSprite->getContentSize().height));
			frame->setAnchorPoint(Vec2(0.5, 0.5));
			plrData[i].Frames.pushBack(frame);
		}
		if (i != 2) {
			plrData[i].AnimLoad = Animation::createWithSpriteFrames(plrData[i].Frames, 0.09f);
			plrData[i].Anim = Animate::create(plrData[i].AnimLoad);
			plrData[i].plrAction = RepeatForever::create(plrData[i].Anim);
		}
		else {
			plrData[i].AnimLoad = Animation::createWithSpriteFrames(plrData[i].Frames, 0.09f);
			plrData[i].Anim = Animate::create(plrData[i].AnimLoad);
			plrData[i].plrAction = Repeat::create(plrData[i].Anim,1);
		}
		

		plrActions.pushBack(plrData[i].plrAction);

	}
	mvtActionId = 1;
	movePlayerBool = false;
	Player::setPlayerAction(1); //setPlayer to idle
}

// CONTINUOUS MOVMENT
void Player::movePlayer(int xAdd, int yAdd, int actionId, bool cond) {
	if (!(movePlayerBool))
	{
		//if (actionId == 0 || actionId == )
		movePlayerBool = true; // CONTINUOUS MOVMENT
		Player::setPlayerAction(actionId);
		Player::updatePosition(cond, xAdd, yAdd);
	}
	
}

void Player::updatePosition(bool cond = false, int xAdd = 0, int yAdd = 0)
{
	if (blockActions) { return; }
	if (mvtDir && mvtActionId == 5) {
		// HE IS MOVING LEFT SO IT MUST BE MINUS
		xAdd = -xAdd;
	}
	// TODO : FIX THIS IT MAKE THE PLAYER MOVE SLOWER WHEN PEROFRMING ANOTHER ACTION
	Vec2 pv = PlayerSprite->getPhysicsBody()->getVelocity();
	if (xAdd != 0) {
		PlayerSprite->getPhysicsBody()->setVelocity(Vec2(xAdd*27, pv.y));
	}
	else if (yAdd != 0) {
		PlayerSprite->getPhysicsBody()->setVelocity(Vec2(pv.x, yAdd*15));
	}
	else if ((xAdd != 0) && (yAdd != 0))
	{
		PlayerSprite->getPhysicsBody()->setVelocity(Vec2(xAdd*27, yAdd * 15));
	}
}

void Player::stopMovment()
{
	if (blockActions) { return; }
	if (movePlayerBool) {
		movePlayerBool = false;
		if ((Player::mvtActionId == 0) || (Player::mvtActionId == 6) || (Player::mvtActionId == 5)) {
			//Player::setPlayerAction(1); NOT THE RIGHT PLACE HERE
			long long int vy = PlayerSprite->getPhysicsBody()->getVelocity().y;
			PlayerSprite->getPhysicsBody()->setVelocity(Vec2(0, vy));
			if (mvtActionId == 0)
				PlayerSprite->getPhysicsBody()->setVelocity(Vec2(0, 0));
			CCLOG("PLAYER  ACTION ID : %i !", mvtActionId);
		}
	}
}

/*
true is to flip left
false to flip right
*/
void Player::setPlayerMvtDir(bool dir)
{
	if (blockActions) { return; }
	mvtDir = dir;
	// SET FLIP IS JUST TO FLIP THE SPRITE FOR MOVING LEFT OR RIGHT 
	PlayerSprite->setFlippedX(dir);
}



/*

void PlayerControl(cocos2d::Layer* layer)
{
	auto eventListener = EventListenerKeyboard::create();

	Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// If a key already exists, do nothing as it will already have a time stamp
		// Otherwise, set's the timestamp to now
		if (keys.find(keyCode) == keys.end()) {
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// remove the key.  std::map.erase() doesn't care if the key doesnt exist
		keys.erase(keyCode);
	};

	//layer->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, layer);

	// Let cocos know we have an update function to be called.
	layer->scheduleUpdate();

}


bool Player::isKeyPressed(EventKeyboard::KeyCode code)
{
	// Check if the key is currently pressed by seeing it it's in the std::map keys
	// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

double Player::keyPressedDuration(EventKeyboard::KeyCode code)
{
	if (!isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL))
		return 0;  // Not pressed, so no duration obviously

				   // Return the amount of time that has elapsed between now and when the user
				   // first started holding down the key in milliseconds
				   // Obviously the start time is the value we hold in our std::map keys
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - keys[code]).count();
}

void Player::update(float delta)
{
	// Register an update function that checks to see if the CTRL key is pressed
	// and if it is displays how long, otherwise tell the user to press it
	//Node::update(delta);
	if (isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL)) {
		std::stringstream ss;
		ss << "Control key has been pressed for " <<
			keyPressedDuration(EventKeyboard::KeyCode::KEY_CTRL) << " ms";
		//label->setString(ss.str().c_str());
	}
	//else
		//label->setString("Press the CTRL Key");
}

// 
// Because cocos2d-x requres createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> keys;

	*/