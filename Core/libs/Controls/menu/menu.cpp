#include "menu.hpp"

Menu::Menu(menu_item_t *menuitems, int size_menu) : menu_items(menuitems),
                                                    size_menu(size_menu)
{
    current_menu = 0;
}

void Menu::up()
{
    if (current_menu < size_menu)
    {
        current_menu++;
    }
    else
    {
        current_menu = 0;
    }
    drawMenu();
}

void Menu::dowm()
{
    if (current_menu > 0)
    {
        current_menu--;
    }
    else
    {
        current_menu = size_menu - 1;
    }
    drawMenu();
}

void Menu::enter()
{
    if (current_menu >= 0 && current_menu < size_menu && menu_items[current_menu].action_h != NULL)
    {
        menu_items[current_menu].action_h();
    }
}

void Menu::drawMenu()
{
    SSD1306_Clear();
    for (int i = 0; i < size_menu; i++)
    {
        if (current_menu == i)
        {
            SSD1306_GotoXY(0, (Font_7x10.FontHeight * i) + 2);
            SSD1306_Puts("> ", &Font_7x10, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
        }
        SSD1306_GotoXY(Font_7x10.FontWidth + 1, (Font_7x10.FontHeight * i) + 2);
        SSD1306_Puts(menu_items[i].name, &Font_7x10, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
    }
    SSD1306_UpdateScreen();
}

int Menu::getCurrentId()
{
    return current_menu;
}