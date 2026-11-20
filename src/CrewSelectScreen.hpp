//shows the six crew members (their real values: strength, usage, puzzle time) IN screen
//the player chooses who walks in first.
#ifndef CREWSELECTSCREEN_HPP
#define CREWSELECTSCREEN_HPP
#include "Screen.hpp"

class CrewSelectScreen:public Screen{
    private:
        int choice; //which card is currently highlighted (0 to 5)
        float timer; //counts up over time, used to animate the highlighted thief used by update() and draw()

        bool viewOnly;//true when opened from the menu ("View Crew")

    public:
        CrewSelectScreen(Game* newGame, bool newViewOnly);
        virtual void handleEvent(const sf::Event& event);
        virtual void update(float dt);
        virtual void draw(sf::RenderWindow& window);
};

#endif
