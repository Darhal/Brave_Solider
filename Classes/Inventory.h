#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Inventory
{
public:
	static enum Items {
		BRONZE,
		SILVER,
		GOLD,
		KEY,
		HEALTH,
		//CAKE,
		PISTOL_AMMO,
		PISTOL,
		GRENADES,
	};

	Inventory::Inventory(cocos2d::Layer*);
	void addItem(Items, int);
	void removeItem(Items, int);
	int getItemAmount(Items);
	void updateText();
	void  giveItemByName(std::string);
	int convertNameToID(std::string);
private:
	cocos2d::ui::Button* backpackIcon;
	void showInventory(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType);
	cocos2d::ui::ScrollView* itemsList;
	cocos2d::ui::Button* items[6];
	cocos2d::ui::Text* itemName[6];
	std::string itemsPath[6][2] = {
		{"Icons/1.png", "Bronze"},
		{"Icons/2.png", "Silver"},
		{"Icons/3.png", "Gold"},
		{"Icons/4key.png", "Key" },
		//{ "icons/5.png", "Cake" },
		{"Icons/6.png", "Ammo" },
	};
	std::map<Items, int> inventory;
};

#endif // __INVENTORY_H__
