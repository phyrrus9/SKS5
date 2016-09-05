#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include <vector>

using std::vector;

namespace inventory
{
	typedef struct
	{
		char *property_name;
	int property_value;
	} item_property;

	typedef struct item
	{
		char *item_name;
		char *combines_with;
		char *makes;
		int item_value;
		int strength;
		vector<item_property> properties;
		bool (*item_use)(struct item *);
		void (*item_draw)(int, int, bool);
	} item;

	typedef bool (*item_use_func)(item *);
	typedef void (*item_draw)(int, int, bool);

	extern vector<item *> user_inventory;

	item *item_create(const char *name);
	item *item_create(const char *name, const int value);
	item *item_create(const char *name, const int value, const int strength);
	item *item_create(const char *name, const int value, const int strength, item_use_func item_use);
	void  item_draw_default(int, int, bool);
	void item_free(item *i);
}

void inventory_enter();


#endif // INVENTORY_H_INCLUDED
