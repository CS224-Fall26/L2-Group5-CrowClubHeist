// The main gameplay screen. The crew walks around a room, meets guards, fights, unlocks the door,
// moves on to the next room and finally takes the artifact in the vault.

// How the room is drawn: we use an sf::View (a "camera"). It lets us draw everything with the
// pixel positions of the room picture, and SFML zooms it to fit the window for us.
// LEARN (SFML views): https://www.sfml-dev.org/tutorials/2.6/graphics-view.php

#ifndef PLAYSCREEN_HPP
#define PLAYSCREEN_HPP

#include "Screen.hpp"
#include "Mission.hpp"
#include <string>

//the fight box never shows more than 3 lines
const int MAX_FIGHT_LINES = 3;

class PlayScreen : public Screen{
    private:
        //The two things this screen can be doing.
        static const int STATE_WALKING = 0;
        static const int STATE_FIGHT = 1;
        int state;

        Mission* mission;//the mission is owned by Game, we only use it here

        //where and how big the room is drawn on the window.
        float scale;
        float originX;
        float originY;

        //a short message at the top of the screen ("Room cleared...").
        std::string toastText;
        float toastTimer;

        //details of the last fight, shown in the fight box.
        std::string fightName;
        int fightCrewStrength;
        int fightGuardStrength;
        int fightResult;
        std::string fightLines[MAX_FIGHT_LINES];
        int fightLineCount;//how many lines are really used in the array

        void enterRoom();// puts the crew at the room's start point
        void setupView();// works out scale and origin for the current room
        void moveActive(float dt);// reads the keys and moves the active crew member
        void startFight(Guard* guard);
        void closeFight();
        void showToast(std::string text);
        void drawHud(sf::RenderWindow& window);
        void drawGuardNumbers(sf::RenderWindow& window);
        void drawFightBox(sf::RenderWindow& window);

    public:
        PlayScreen(Game* newGame);
        virtual void handleEvent(const sf::Event& event);
        virtual void update(float dt);
        virtual void draw(sf::RenderWindow& window);
};

#endif
