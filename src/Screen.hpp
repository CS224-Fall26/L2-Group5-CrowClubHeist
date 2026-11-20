// Screen is an ABSTRACT BASE CLASS: the "template" every screen (menu, intel, play...) must follow.
// A function written "= 0" is a PURE VIRTUAL function: it has no body here, and every child class
// MUST write its own version. Because of that, Game can hold one Screen* pointer and just call
// handleEvent / update / draw, and the right screen reacts (POLYMORPHISM).

#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <SFML/Graphics.hpp>

class Game; //we only keep a pointer to Game, so the name is enough here

class Screen{
    protected:
        Game* game; //the screens use this to ask Game for the crew, the mission, to change screen...

    public:
        Screen(Game* newGame);
        virtual ~Screen();

        virtual void handleEvent(const sf::Event& event) = 0;// a key was pressed
        virtual void update(float dt) = 0;// runs every frame (dt = seconds since last frame)
        virtual void draw(sf::RenderWindow& window) = 0;// draws the screen
};

#endif
