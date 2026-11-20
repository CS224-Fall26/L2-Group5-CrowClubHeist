//shows the rules of the game in short lines.

#ifndef HOWTOPLAYSCREEN_HPP
#define HOWTOPLAYSCREEN_HPP

#include "Screen.hpp"

class HowToPlayScreen : public Screen{
private:
    sf::Texture background;

public:
    HowToPlayScreen(Game* newGame);

    
    //handles key presses: Esc or Enter goes back to the menu
    virtual void handleEvent(const sf::Event& event);

    //nothing to update on this screen
    virtual void update(float dt);

    //draws the background, the dark shade, and the list of rules
    virtual void draw(sf::RenderWindow& window);
};

#endif
