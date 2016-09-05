#include "engine.h"

viewType o_view;
int o_Yangle, o_Zangle;

void inventory_exit();

namespace inventory_menu
{
	void inventory_render();
	void inventory_keyPress(int, int, int);
	#define inventorysize 3
	unsigned int selected_item = 0;

	void p_displayItemAction(int x, bool available, const char *text)
	{
		if (available)
			displayText(x, -1.25, 0, 255, 0, text);
		else
			displayText(x, -1.25, 255, 0, 0, text);
	}

	bool p_in_inventory(const char *name)
	{
		for (int i = 0; i < inventory::user_inventory.size(); i++)
			if (!strcmp(name, inventory::user_inventory.at(i)->item_name))
				return true;
		return false;
	}

	void display_itemactions(inventory::item *i)
	{
		char buf[64];
		placeObject(B_WAL, -6, 10, 6);
		displayText(1, -1.25, 255, 0, 64, i->item_name);
		sprintf(buf, "V: %d", i->item_value);
		displayText(1, -1.55, 255, 0, 64, buf);
		sprintf(buf, "S: %d", i->strength);
		displayText(1, -1.85, 255, 0, 64, buf);
		p_displayItemAction(5, i->item_use != NULL, "[u] Use");
		displayText(6, -1.25, 0, 255, 0, "[q] Drop");
		p_displayItemAction(7, i->combines_with != NULL && p_in_inventory(i->combines_with), "[c] Craft");
	}

	void inventory_render()
	{
		int in = 0;
		if (inventory::user_inventory.size() > 0)
		{
			for (int y = -inventorysize - 1; y <= inventorysize - 1; y++)
				for (int x = -inventorysize; x <= inventorysize && in < inventory::user_inventory.size(); x++, in++)
					inventory::user_inventory.at(in)->item_draw(x, -y, in == selected_item);
			display_itemactions(inventory::user_inventory.at(selected_item));
		}
	}

	void inventory_keyPress(int key, int x, int y)
	{
		if (key != 'i')
		{
			if (key == '+')
			{
				inventory::item *newitem = inventory::item_create("Test item");
				inventory::user_inventory.push_back(newitem);
			}
			if (inventory::user_inventory.size() > 0)
			{
				//navigation
				if (key == GLUT_KEY_RIGHT && selected_item < 48)
					++selected_item;
				else if (key == GLUT_KEY_LEFT && selected_item > 0)
					--selected_item;
				else if (key == GLUT_KEY_UP && selected_item > 6)
					selected_item -= 7;
				else if (key == GLUT_KEY_DOWN && selected_item < 39)
					selected_item += 7;
				else if (key == 'u')
				{
					inventory::item_use_func fnc = inventory::user_inventory.at(selected_item)->item_use;
					if (fnc != NULL)
						if (!fnc(inventory::user_inventory.at(selected_item)))
							inventory::user_inventory.erase(inventory::user_inventory.begin() + selected_item);
				}
				else if (key == 'c')
				{

				}
				else if (key == 'q')
				{
					inventory::user_inventory.erase(inventory::user_inventory.begin() + selected_item);
				}
				if (selected_item >= inventory::user_inventory.size())
					selected_item = inventory::user_inventory.size() - 1;
			}
		}
		else
			inventory_exit();
	}
}

namespace inventory
{
	vector<item *> user_inventory;
	item *item_create(const char *name, const int value, const int strength, item_use_func item_use)
	{
		item *ritem = new item;
		if (name != NULL)
		{
			ritem->item_name = new char[strlen(name) + 1];
			strcpy(ritem->item_name, name);
		}
		else
			ritem->item_name = NULL;
		ritem->combines_with = NULL;
		ritem->makes = NULL;
		ritem->item_value = value;
		ritem->strength = strength;
		ritem->item_use = item_use;
		ritem->item_draw = item_draw_default;
		return ritem;
	}
	item *item_create(const char *name, const int value, const int strength)	{ return item_create(name, value, strength, NULL); }
	item *item_create(const char *name, const int value)					{ return item_create(name, value, 0); }
	item *item_create(const char *name)								{ return item_create(name, 0); }
	void item_free(item *i)
	{

		if (i->item_name != NULL)
			delete[] i->item_name;
		if (i->combines_with != NULL)
			delete[] i->combines_with;
		if (i->makes != NULL)
			delete[] i->makes;
		i->item_name = i->combines_with = i->makes = NULL;
		delete i;
	}
	void  item_draw_default(int x, int y, bool selected)
	{
		if (!selected)
			placeObject(B_WAL, x, 10, y);
		else
			placeObject(B_TEL, x, 10, y);
	}
}

void inventory_exit()
{
	pl_view = o_view;
	Yangle = o_Yangle;
	Zangle = o_Zangle;
	gfx_exit();
	keyboard_exit();
	keyboard_exit(true);
}

void inventory_enter()
{
	o_view = pl_view;
	o_Yangle = Yangle;
	o_Zangle = Zangle;
	pl_view = V_NORTH;
	Yangle = 0;
	Zangle = 0;
     gfx_enter(inventory_menu::inventory_render);
     keyboard_enter(inventory_menu::inventory_keyPress);
     keyboard_enter(inventory_menu::inventory_keyPress, true);
}

