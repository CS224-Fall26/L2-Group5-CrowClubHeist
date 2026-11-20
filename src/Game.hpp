// Game is the "boss" of the program. It owns
//   - the window
//   - the 6 crew members and the 3 artifacts
//   - the room facts read from data/rooms.txt
//   - the current Mission
//   - the current Screen
// and it runs the MAIN LOOP (events -> update -> draw, many times per second).

// LEARN (SFML window + main loop): https://www.sfml-dev.org/tutorials/2.6/window-window.php
// LEARN (SFML events):             https://www.sfml-dev.org/tutorials/2.6/window-events.php
// LEARN (SFML keyboard input):     https://www.sfml-dev.org/tutorials/2.6/window-inputs.php
// LEARN (SFML time / delta time):  https://www.sfml-dev.org/tutorials/2.6/system-time.php

#ifndef GAME_HPP
#define GAME_HPP
#include "Screen.hpp"
#include "CrewMember.hpp"
#include "Artifact.hpp"
#include "Mission.hpp"
#include "RoomInfo.hpp"
#include <SFML/Graphics.hpp>

//the window size.
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

//a name for every screen, so we can say changeScreen(SCREEN_MENU).
const int SCREEN_MENU = 0;
const int SCREEN_HOWTO = 1;
const int SCREEN_INTEL = 2;
const int SCREEN_CREW = 3;
const int SCREEN_PLAY = 4;
const int SCREEN_WIN = 5;
const int SCREEN_LOSE = 6;

const int SCREEN_INTEL_VIEW = 7;// Kaz's intel opened from the menu (just to read it)
const int SCREEN_CREW_VIEW = 8; // the crew opened from the menu (just to read it)

class Game{
    private:
        sf::RenderWindow window;
        Screen* currentScreen;
        int nextScreen;//which screen to show next (-1 = no change wanted)

        CrewMember* crew[MAX_CREW_MEMBERS];
        int crewCount;//how many crew members are really in the array
        Artifact* artifacts[MAX_ARTIFACTS];
        int artifactCount;//how many artifacts are really in the array
        RoomInfo roomInfos[MAX_ROOM_INFOS];
        int roomInfoCount;//how many rooms are really in the array
        Mission* mission;
        int selectedArtifact;//index chosen on the intel screen

        void showNextScreen();//really switches the screen (called safely once per frame)

    public:
        Game();
        ~Game();

        void run();//main loop

        //asking Game for things (used by the screens).
        void changeScreen(int screenName);
        int getCrewCount() const;
        CrewMember* getCrewMember(int index);
        int getArtifactCount() const;
        Artifact* getArtifact(int index);
        Mission* getMission();
        int getSelectedArtifact() const;
        void setSelectedArtifact(int index);

        void prepareNewMission();//throws away the old mission and rolls a new usage for every crew member
        void startMission(int starterIndex);//builds the mansion and opens the play screen
        void quit();
};

#endif
