#include "Inventory.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;


Inventory::Inventory(Layer* layer)
{
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

	backpackIcon = Button::create("icons/backpack.png","icons/backpack_seltected.png" );
	backpackIcon->setPosition(Vec2(origin.x + backpackIcon->getContentSize().width / 14,
		((origin.y + visibleSize.height) - backpackIcon->getContentSize().height/6)
	));
	backpackIcon->setScale(0.15f);
	

	itemsList = ScrollView::create();
	itemsList->setDirection(ui::ScrollView::Direction::VERTICAL);
	itemsList->setClippingEnabled(false);
	

	/*Layout* invBackground = Layout::create();
	invBackground->setLayoutType(ui::Layout::Type::RELATIVE);*/
	itemsList->setBackGroundImage("icons/popup_base.png");
	itemsList->setContentSize(Size(380, 300));
	//itemsList->setClippingEnabled();
	itemsList->setInnerContainerPosition(Vec2(100,100));
	itemsList->setInnerContainerSize(Size(381, 300));
	itemsList->setBounceEnabled(true);
	itemsList->setAnchorPoint(Vec2(0.5, 0.5));
	
	ImageView* header = ImageView::create("icons/logo.png");
	header->setScale(0.4f);
	//header->setTouchEnabled(true);
	header->setPosition(Vec2(itemsList->getContentSize().width / 2, itemsList->getContentSize().height));
	itemsList->addChild(header);
	itemsList->setColor(Color3B(128.f,128.f,128.f));
	itemsList->setOpacity(85);
	for (int i = 0;i < 4; i++) {
		items[i] = Button::create(itemsPath[i][0], itemsPath[i][0]);
		items[i]->setScale(0.2f);
		items[i]->setPosition(Vec2(itemsList->getContentSize().width / 4,
			itemsList->getContentSize().height- (i+1)*items[i]->getContentSize().height/4));
		itemsList->addChild(items[i]);
		itemName[i] = Text::create(itemsPath[i][1], "fonts/arial.ttf", 25);
		itemName[i]->setPosition(Vec2(itemsList->getContentSize().width / 2,
			itemsList->getContentSize().height - (i+1)*items[i]->getContentSize().height/4));
		itemsList->addChild(itemName[i]);
		inventory[Items(i)] = 0;
	}
	

	itemsList->setPosition(Vec2(origin.x + backpackIcon->getContentSize().width/2.65,
		((origin.y + visibleSize.height) - backpackIcon->getContentSize().height/2.65)
	));

	layer->addChild(itemsList);
	itemsList->setVisible(false);
	layer->addChild(backpackIcon);
	backpackIcon->addTouchEventListener(CC_CALLBACK_2(Inventory::showInventory, this));
}

void Inventory::showInventory(cocos2d::Ref* sender, ui::Widget::TouchEventType evt)
{
	if (evt == ui::Widget::TouchEventType::ENDED) {
		//CCLOG("CLICKED MOUSE");
		if (itemsList->isVisible()) {
			CCLOG("HIDE INVENTORY ");
			itemsList->setVisible(false);
			backpackIcon->setColor((Color3B::WHITE));
		}
		else {
			itemsList->setVisible(true);
			backpackIcon->setColor(Color3B(180, 180, 180));
		}
	}
	CCLOG("CLICKED MOUSE");
	Inventory::updateText();
}

void Inventory::updateText()
{
	if (itemsList->isVisible()) {
		for (int i = 0; i <4; i++) {
			std::string str = itemsPath[i][1] + " : "+ std::to_string(getItemAmount(Items(i)));
			//CCLOG("ITEM ID : %i AMOUNT : %i", i, getItemAmount(Items(i)));
			itemName[i]->setText(str);
		}
	}
}

int Inventory::convertNameToID(std::string name)
{
	for (int i = 0; i<4; i++) {
		if (itemsPath[i][2] == name) {
			return i;
		}
	}
}


void  Inventory::giveItemByName(std::string name)
{
	Inventory::addItem(Items(Inventory::convertNameToID(name)),1);
}

void Inventory::addItem(Items item, int amount)
{
	if (inventory[item] == 0){
		inventory[item] = amount;
		Inventory::updateText();
		return;
	}
	inventory[item] += amount;
	//CCLOG("INVENTORY SYSTEM : ADDITEM CALLED : %i", inventory[item]);
	Inventory::updateText();
}

void Inventory::removeItem(Items item, int amount)
{
	if (inventory[item] == 0) {
		inventory[item] = 0;
		return;
	}
	// IF ITS GOING TO BE NEGITIVE SET TO 0 ELSE DECREASE IT
	(inventory[item]-amount < 0) ? inventory[item] = 0 : inventory[item] -= amount;
	Inventory::updateText();
}

int Inventory::getItemAmount(Items item)
{
	if (inventory[item] != 0) {
		//CCLOG("CALLIN GET ITEM ID : %i AMOUNT : %i ", item, inventory[item] == 0);
		return inventory[item];
	}
	else
		return 0;
}