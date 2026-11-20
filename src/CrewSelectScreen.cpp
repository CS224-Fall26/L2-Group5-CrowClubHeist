// CrewSelectScreen.cpp
#include "CrewSelectScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include <string>
//set screen up
CrewSelectScreen::CrewSelectScreen(Game* newGame, bool newViewOnly) : Screen(newGame){
    viewOnly=newViewOnly;
    choice=0;
     timer=0.0f;
}

//handles the key presses
//left and right to choose, enter to start mission, escape to go back
//if viewOnly is true, enter just goes back to the menu
//if viewOnly is false, escape goes back to the intel screen
void CrewSelectScreen::handleEvent(const sf::Event& event){
    if (event.type != sf::Event::KeyPressed){//ignore all events except key presses
        return;
    }
    int count = game->getCrewCount();//how many crew members are there? (6 hone chhaye

    //wrap around to the other side if we go past the first or last crew member
    if (event.key.code == sf::Keyboard::Left){
        choice = (choice + count - 1) % count;
    }
    else if (event.key.code == sf::Keyboard::Right){
        choice = (choice + 1) % count;
    }

    //if the player presses escape or enter,,back to the menu or start the mission
    else if (event.key.code == sf::Keyboard::Escape){
        //if viewOnly is true, go back to the menu. If viewOnly is false, go back to the intel screen
        if (viewOnly == true){
            game->changeScreen(SCREEN_MENU);
        }
        else{
            game->changeScreen(SCREEN_INTEL);
        }
    }
    //if the player presses enter, either go back to the menu 
    //(if viewOnly is true] or start the mission (if viewOnly is false)
    else if (event.key.code == sf::Keyboard::Enter){
        if (viewOnly == true){
            game->changeScreen(SCREEN_MENU);
        }
        else{
            game->startMission(choice);
        }
    }
}

//updates the timer for the animation of the highlighted crew member
void CrewSelectScreen::update(float dt){
    timer = timer + dt;//counts up over time, used to animate the highlighted mem
}


//draws the screen
//shows the six crew members (their real values: strength, usage, puzzle time) IN screen
//the player chooses who walks in first
void CrewSelectScreen::draw(sf::RenderWindow& window){
    if (viewOnly == true){
        drawText(window, "THE CREW", 40, 640, 28, COLOR_GOLD, ALIGN_CENTER);
        drawText(window, "Usage is rolled again at the start of every mission.", 20, 640, 88, COLOR_CREAM, ALIGN_CENTER);
    }
    else{//viewOnly is false, so the player is choosing who walks in first
        drawText(window, "CHOOSE YOUR FIRST CREW MEMBER", 40, 640, 28, COLOR_GOLD, ALIGN_CENTER);
        
        //the artifact the player is trying to steal in this mission
        Artifact* target = game->getArtifact(game->getSelectedArtifact());

        //show the name of the artifact and a note that usage was rolled for this mission
        drawText(window, "Target: " + target->getName() + "   (usage was rolled for this mission)", 20, 640, 88, COLOR_CREAM, ALIGN_CENTER);
    }

    //one card per crew member
    //drawn with a loop
    for (int i = 0; i < game->getCrewCount(); i++){//draw the card for crew member i
        CrewMember* member = game->getCrewMember(i);//getthe crew member object for this card
        float x = 40.0f + i * 200.0f; //the x position of this card (cards are 200 pixels apart)
        float y = 140.0f;//the y position of this card!! all cards are at the same height

        int column = 0;//standing still
        if (i == choice){//the highlighted card (the one the player is currently choosing)
            drawPanel(window, x, y, 185, 340, COLOR_PANEL_SELECTED, COLOR_GOLD); //the highlighted card has a different color
            column = 1 + (int)(timer * 6.0f) % 2; //the highlighted one walks on the spot
        }

        //the non-highlighted cards are darker and have a different border color
        else{
            drawPanel(window, x, y, 185, 340, COLOR_PANEL, sf::Color(120, 90, 60));
        }

        member->drawAt(window, x + 92.5f - 48.0f, y + 24, 8.0f, DIR_DOWN, column);
        drawText(window, member->getName(), 26, x + 92.5f, y + 165, COLOR_CREAM, ALIGN_CENTER);
        drawText(window, "Strength  " + std::to_string(member->getStrength()), 18, x + 20, y + 215);
        drawText(window, "Usage     " + std::to_string(member->getUsage()), 18, x + 20, y + 245);
        drawText(window, "Puzzle    " + std::to_string(member->getPuzzleTime()) + " s", 18, x + 20, y + 275);
    }

    //the description and the quote of the highlighted crew member.
    drawPanel(window, 40, 520, 1200, 100);
    CrewMember* chosen = game->getCrewMember(choice);
    drawText(window, chosen->getName() + ": " + chosen->getDescription(), 22, 70, 535);
    drawText(window, "\"" + chosen->getQuote() + "\"", 22, 70, 575, COLOR_GOLD);
    if (viewOnly == true){
        drawText(window, "Left / Right look at the crew    Esc back", 18, 640, 660, COLOR_GOLD, ALIGN_CENTER);
    }
    else{
        drawText(window, "Left / Right choose    Enter start mission    Esc back", 18, 640, 660, COLOR_GOLD, ALIGN_CENTER);
    }
}
