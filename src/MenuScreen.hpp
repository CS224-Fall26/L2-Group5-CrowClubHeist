// The welcome screen with five choices: New Mission, View Crew, Kaz's Intel, How to Play, Exit.

#ifndef MENUSCREEN_HPP
#define MENUSCREEN_HPP

#include "Screen.hpp"
#include <string>

class MenuScreen : public Screen{
    private:
        static const int ITEM_COUNT = 5;
        std::string items[ITEM_COUNT]; // the text of every menu item    
        int choice;// which item is highlighted
        float timer;// for the walking crew at the bottom
        sf::Texture background; 

    public:
        MenuScreen(Game* newGame);
        virtual void handleEvent(const sf::Event& event);
        virtual void update(float dt);
        virtual void draw(sf::RenderWindow& window);
};

#endif
