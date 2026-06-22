#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"

struct menu_item_t
{
    int8_t id;
    char *name;
    void (*action_h)();
    struct menu_item_t *sub_menu;
    int8_t sub_menu_length;
};

struct menu_items_info_t
{
    menu_item_t *menu_items;
    uint16_t size;
};

typedef struct menu_item_t;
typedef struct menu_info_t;

class Menu
{
private:
    menu_item_t *menu_items;
    int size_menu;
    int current_menu;

public:
    Menu(menu_item_t *menuitems, int size_menu);
    void up();
    void dowm();
    void enter();
    void drawMenu();
    int getCurrentId();
};