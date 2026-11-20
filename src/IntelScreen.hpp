//Kkaz's intel: a map of the mansion (3 floors x 3 rooms) and the three artifacts.
//the player picks the artifact to steal. The map marks the TARGET vault.

#ifndef INTELSCREEN_HPP
#define INTELSCREEN_HPP

#include "Screen.hpp"

class IntelScreen : public Screen{
    private:
        int choice;//the artifact the player has chosen to steal (0, 1, or 2)
        float timer; //makes the TARGET text blink
        bool viewOnly;    //true when opened from the menu: you can only read it, not start a mission

    public:
        IntelScreen(Game* newGame, bool newViewOnly); 
        virtual void handleEvent(const sf::Event& event);
        virtual void update(float dt);
        virtual void draw(sf::RenderWindow& window);
};

#endif
