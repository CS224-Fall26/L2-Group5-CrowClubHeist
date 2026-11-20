// Shown at the end of a mission: "Mission successful" or "Mission failed", plus a crew report.
#ifndef RESULTSCREEN_HPP
#define RESULTSCREEN_HPP
#include "Screen.hpp"

class ResultScreen : public Screen{
    private:
        bool won;
        float timer;
        sf::Texture background;

    public:
        ResultScreen(Game* newGame, bool newWon);
        virtual void handleEvent(const sf::Event& event);
        virtual void update(float dt);
        virtual void draw(sf::RenderWindow& window);
};

#endif
