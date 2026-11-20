// Game.cpp
#include "Game.hpp"

//the first screen the player sees when the game starts
#include "MenuScreen.hpp" 

//the screen that explains how to play the game
#include "HowToPlayScreen.hpp"  

//the screen that shows the artifacts and their floor numbers, and lets the player choose one
#include "IntelScreen.hpp"  

//the screen that shows the crew members and lets the player choose them for the mission
#include "CrewSelectScreen.hpp" 

//the screen where the player actually plays the mission (walks around the mansion, fights guards, solves puzzles)
#include "PlayScreen.hpp"

//the screen that shows the result of the mission (win or lose)
#include "ResultScreen.hpp"

#include <cstdlib>//for rand() and srand()
#include <ctime>//for time() to seed the random number generator

Game::Game(){   //onstructor: sets up the game, loads the crew and artifacts, opens the window, shows the menu screen
    //make rand() give different numbers every time the game starts.
    srand((unsigned int)time(0));

    //open the window (width, height) with a title.
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "The Crow Club Heist");
    window.setFramerateLimit(60);

    //the six crew members: name, combat strength, puzzle time (seconds), quote.
    crew[0] = new CrewMember("Kaz", 7, 30, "The leader of the crew, boss, schemer.", "The easiest way to steal a man's wallet is to tell him you're going to steal his watch.");
    crew[1] = new CrewMember("Inej", 8, 20, "A silent climber, the Wraith, stealthy one.", "You won't hear me. You won't see me. Until it's too late..");
    crew[2] = new CrewMember("Jesper", 9, 15, "A sharpshooter who loves a gamble.", "Maybe I liked your stupid face, Wylan.");
    crew[3] = new CrewMember("Nina", 6, 60, "Brave, Heartrender", "I can make your heart stop. Or I can make it beat faster. Depends on what you're worth to me.");
    crew[4] = new CrewMember("Matthias", 10, 10, "A former soldier.", "I fight with honour.");
    crew[5] = new CrewMember("Wylan", 5, 70, "A young inventor. explosives expert, great with puzzles.", "I'll solve it. Just give me a moment.");

    crewCount = 6;//we filled 6 places of the crew array

    //the three artifacts: name, floor, picture file, description.
    artifacts[0] = new Artifact("Silver Raven", 1, "SilverRaven.png", "A small silver bird on floor 1. The easiest target.");
    artifacts[1] = new Artifact("Emerald Crown", 2, "EmeraldCrown.png", "A heavy green crown on floor 2. Tougher guards.");
    artifacts[2] = new Artifact("Obsidian Seal", 3, "ObsidianSeal.png", "A black stone seal on floor 3. The strongest guards.");

    artifactCount = 3;//we filled 3 places of the artifacts array

    //re`ading the room facts :- walls, doors, guard spots... from the text file.
    roomInfoCount = loadRoomInfos("data/rooms.txt", roomInfos);

    mission = nullptr;  //the mission is created when the player starts it, and deleted when the mission ends
    selectedArtifact = 0;   //the first artifact is selected by default
    nextScreen = -1;    //no screen change wanted yet
    prepareNewMission();  //roll the usage once now, so "View Crew" has real numbers to show
    currentScreen = new MenuScreen(this);//show the menu screen first
}

Game::~Game(){//delete the screens, the mission, the crew and the artifacts
    delete currentScreen;
    delete mission;

    for (int i = 0; i < crewCount; i++){//delete the crew members, Game owns them
        delete crew[i];}

    for (int i = 0; i < artifactCount; i++){ //delete the artifacts, Game owns them
        delete artifacts[i];
    }
}

void Game::run(){ //events, update, draw, repeat until the window is closed
    sf::Clock clock;// measures time

    //THEE MAIN LOOP: runs until the window is closed
    while (window.isOpen() == true){
        //1)EVENTS: ask the window what happened (key pressed, window closed...)
        sf::Event event;
        while (window.pollEvent(event) == true){//while there are events to handle, handle them one by one

            if (event.type == sf::Event::Closed){//the user clicked the X button to close the window
                window.close();
            }

            else{//let the current screen handle the event (key pressed, mouse clicked...)
                currentScreen->handleEvent(event);
            }
        }

        //2)UPDATE
        //dt = how many seconds the last frame took. We never let it get too big
        float dt = clock.restart().asSeconds();//restart() returns the time since the last restart, and restarts the clock for the next frame
        if (dt > 0.05f){    //if the computer is too slow, don't let dt get too big (it would make the game jumpy)
            dt = 0.05f;     //20 frames per second is the slowest we allow
        }
        currentScreen->update(dt); //let the current screen update itself (move the crew, animate the guards, etc.)

        //3)DRAW
        //clear the window, let the screen draw itself, show the result
        window.clear(sf::Color(24, 18, 20));
        currentScreen->draw(window);
        window.display();

        //4)If a screen asked to change, do it now (nNOOT in the middle of the screen's own code)
        showNextScreen();
    }
}

void Game::changeScreen(int screenName){    //the screens call this to ask Game to change the screen
    nextScreen = screenName; //the actual change will happen safely once per frame in showNextScreen()
}

void Game::showNextScreen(){  //if a screen asked to change, delete the old screen and create the new one
    if (nextScreen == -1){ //no screen change wanted
        return;
    }

    delete currentScreen; //remove the old screen

    if (nextScreen == SCREEN_MENU){//show the menu screen
        currentScreen = new MenuScreen(this);
    }
    else if (nextScreen == SCREEN_HOWTO){//show the how to play screen
        currentScreen = new HowToPlayScreen(this);
    }
    else if (nextScreen == SCREEN_INTEL){//show the intel screen to choose an artifact for the mission
        currentScreen = new IntelScreen(this, false);
    }

    else if (nextScreen == SCREEN_INTEL_VIEW){//show the intel screen to view the artifacts and their floor numbers
        currentScreen = new IntelScreen(this, true);
    }

    else if (nextScreen == SCREEN_CREW){//show the crew select screen to choose the crew for the mission
        currentScreen = new CrewSelectScreen(this, false);
    }

    else if (nextScreen == SCREEN_CREW_VIEW){//show the crew select screen to view the crew members and their usage numbers
        currentScreen = new CrewSelectScreen(this, true);
    }
    else if (nextScreen == SCREEN_PLAY){//show the play screen to play the mission
        currentScreen = new PlayScreen(this);
    }
    else if (nextScreen == SCREEN_WIN){//show the result screen to show that the mission was won
        currentScreen = new ResultScreen(this, true);
    }
    else{//show the result screen to show that the mission was lost
        currentScreen = new ResultScreen(this, false);
    }
    nextScreen = -1;
}

int Game::getCrewCount() const{
    return crewCount;
}

//returns a pointer to the crew member at the given index (0 to 5)
//the crew members are owned by Game, the screens only use them
//the screens can ask Game for the crew members, but they cannot delete them
CrewMember* Game::getCrewMember(int index){
    return crew[index];
}

//returns how many artifacts there are (3)
//the artifacts are owned by Game, the screens only use them
//the screens can ask gaem for the artifacts but they cannot delete them
int Game::getArtifactCount() const{
    return artifactCount;
}

Artifact* Game::getArtifact(int index){//returns a pointer to the artifact at the given index (0 to 2)
    return artifacts[index];
}

//returns a pointer to the current mission (nullptr if there is none)
Mission* Game::getMission(){
    return mission;
}

int Game::getSelectedArtifact() const{
    return selectedArtifact;
}

void Game::setSelectedArtifact(int index){
    selectedArtifact = index;
}

void Game::prepareNewMission(){
    delete mission;
    mission = nullptr;
    //usage is random (2 to 5) and is rolled again for every mission
    for (int i = 0; i < crewCount; i++){
        crew[i]->rollUsage();
    }
}


void Game::startMission(int starterIndex){ 
    delete mission; 
    mission = new Mission(crew, crewCount, artifacts[selectedArtifact], roomInfos, roomInfoCount, artifacts, artifactCount, starterIndex);
    changeScreen(SCREEN_PLAY);
}

void Game::quit(){
    window.close();
}
