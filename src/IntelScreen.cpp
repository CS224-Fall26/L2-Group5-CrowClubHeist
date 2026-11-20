#include "IntelScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include <cmath>
#include <string>


//set screen up. viewOnly decides if we are just looking or actually starting a mission.
IntelScreen::IntelScreen(Game* newGame, bool newViewOnly) : Screen(newGame){
    viewOnly = newViewOnly;
    choice = 0;//start with the first artifact highlighted
    timer = 0.0f; //animation clock starts at zero
}

//handles the key presses
//up and down to choose an artifact, enter to confirm, escape to go back
//if viewOnly is true, enter just goes back to the menu
//if viewOnly is false, enter saves the choice and goes to the crew screen
void IntelScreen::handleEvent(const sf::Event& event){
    if (event.type != sf::Event::KeyPressed){//ignore all events except key presses
        return;
    }

    
    int count = game->getArtifactCount();//how many artifacts are there? (3)

    //wrap around to the other side if we go past the top or bottom artifact
    if (event.key.code == sf::Keyboard::Up){
        choice = (choice + count - 1) % count;
    }
    else if (event.key.code == sf::Keyboard::Down){
        choice = (choice + 1) % count;
    }
    else if (event.key.code == sf::Keyboard::Escape){//if the player presses escape, go back to the menu
        game->changeScreen(SCREEN_MENU);
    }
    else if (event.key.code == sf::Keyboard::Enter){
        if (viewOnly == true){//if viewOnly is true, enter just goes back to the menu
            game->changeScreen(SCREEN_MENU);
        }
        else{//if viewOnly is false, save the choice and go to the crew select screen
            game->setSelectedArtifact(choice);
            game->changeScreen(SCREEN_CREW);
        }
    }
}

//updates the timer for the blinking text
void IntelScreen::update(float dt){
    timer = timer + dt;//counts up over time, used to make the TARGET text blink
}

//draws the screen
//shows the mansion map on the left and the artifact list on the right
void IntelScreen::draw(sf::RenderWindow& window){
    drawText(window, "KAZ'S INTEL", 44, 640, 28, COLOR_GOLD, ALIGN_CENTER);
    drawText(window, "Choose the artifact the crew will steal.", 20, 640, 90, COLOR_CREAM, ALIGN_CENTER);

    //find out which floor the currently highlighted artifact is on
    //so we can highlight that whole floor on the map
    int targetFloor = game->getArtifact(choice)->getFloorNumber();

    //Left side: the map of the mansion 
    drawPanel(window, 60, 140, 500, 330);
    drawText(window, "MANSION MAP", 22, 310, 150, COLOR_GOLD, ALIGN_CENTER);

    //draw floor 3 at the top and floor 1 at the bottom, like a real building.
    for (int floorNumber = 3; floorNumber >= 1; floorNumber--){//draw the floor number and the three rooms on that floor
        float y = 200.0f + (3 - floorNumber) * 84.0f;//the y position of this floor's row of boxes
        bool isTargetFloor = (floorNumber == targetFloor);//the TARGET vault is on this floor

        //draw the floor number on the left side of the row of boxes
        drawText(window, "Floor " + std::to_string(floorNumber), 20, 80, y + 22);

        //three boxes per floor. The third one is the vault.
        for (int room = 0; room < 3; room++){
            float x = 200.0f + room * 115.0f;
            bool isVault = (room == 2);

            sf::RectangleShape box(sf::Vector2f(100, 64));
            box.setPosition(x, y);
            if (isVault == true){//the vault is a darker brown than the rooms
                box.setFillColor(sf::Color(90, 70, 30));
            }
            else{//the rooms are a lighter brown than the vault
                box.setFillColor(sf::Color(60, 42, 34));
            }
            box.setOutlineThickness(3);
            if (isTargetFloor == true){//the TARGET floor has a gold outline
                box.setOutlineColor(COLOR_GOLD);
            }
            else{//the other floors have a darker brown outline
                box.setOutlineColor(sf::Color(110, 80, 60));
            }
            window.draw(box);

            if (isVault == true){//the vault has the text "VAULT" and, if it's the TARGET floor, also the blinking text "TARGET"
                drawText(window, "VAULT", 16, x + 50, y + 8, COLOR_CREAM, ALIGN_CENTER);
                if (isTargetFloor == true){
                    //the text blinks: its green value goes up and down with a sine wave.
                    int blink = (int)(120 + 100 * std::sin(timer * 5.0f));
                    drawText(window, "TARGET", 18, x + 50, y + 34, sf::Color(255, blink, 100), ALIGN_CENTER);
                }
            }
            else{
                drawText(window, "Room " + std::to_string(room + 1), 16, x + 50, y + 8, COLOR_CREAM, ALIGN_CENTER);
                //the rooms have the text "Room 1", "Room 2", or "Room 3"
            }
        }
    }

    //Right side: the list of artifacts 
    for (int i = 0; i < game->getArtifactCount(); i++){//draw the artifact's name, floor number, description, and icon the chosen artifact has a gold outline.
        Artifact* artifact = game->getArtifact(i);//get the artifact to draw
        float y = 140.0f + i * 110.0f;//the y position of this artifact's box

        if (i == choice){//the chosen artifact has a gold outline
            drawPanel(window, 620, y, 600, 96, COLOR_PANEL_SELECTED, COLOR_GOLD);
        }
        else{//the other artifacts have a brown outline
            drawPanel(window, 620, y, 600, 96, COLOR_PANEL, sf::Color(120, 90, 60));
        }

        sf::Sprite icon(artifact->getIcon());//the small picture of the artifact
        icon.setScale(6.0f, 6.0f);//scale the icon up to 48x48 pixels
        icon.setPosition(640, y + 12);
        window.draw(icon);

        drawText(window, artifact->getName() + "   (Floor " + std::to_string(artifact->getFloorNumber()) + ")", 26, 740, y + 12);
        drawText(window, artifact->getDescription(), 16, 740, y + 54);
    }

    //bottom: Kaz's advice 
    drawPanel(window, 60, 510, 1160, 110);
    drawText(window, "Kaz: \"The deeper the artifact, the stronger the guards. Pick wisely.\"", 22, 90, 535);
    drawText(window, "Guard strength: floor 1 = 2-5, floor 2 = 4-8, floor 3 = 7-10.", 18, 90, 580, COLOR_GOLD);
    
    //show the correct controls at the bottom depending on the mode
    if (viewOnly == true){
        drawText(window, "Up / Down look at the artifacts    Esc back", 18, 640, 660, COLOR_GOLD, ALIGN_CENTER);
    }
    else{ 
        drawText(window, "Up / Down choose    Enter confirm    Esc back", 18, 640, 660, COLOR_GOLD, ALIGN_CENTER);
    }
}