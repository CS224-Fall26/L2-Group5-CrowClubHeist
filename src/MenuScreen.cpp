#include "MenuScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"

//setup the menu screen. put the 5 options in the list.
MenuScreen::MenuScreen(Game* newGame) : Screen(newGame){
    items[0] = "New Mission";
    items[1] = "View Crew";
    items[2] = "Kaz's Intel";
    items[3] = "How to Play";
    items[4] = "Exit";
    
    choice = 0;  //start mein pehla option select hogata rehga
    timer = 0.0f;// timer for the walking animation at the bottom
    background.loadFromFile("assets/rooms/lobby.png"); // load the picture for the background
}

//check what key the player pressed
void MenuScreen::handleEvent(const sf::Event& event){
    //We only care about key presses here.
    if (event.type != sf::Event::KeyPressed){
        return;
    }

    //if Up arrow, move up,, the math makes it go to the bottom if we are at the top.
    if (event.key.code == sf::Keyboard::Up){
        choice = (choice + ITEM_COUNT - 1) % ITEM_COUNT;     //go up, wrap around at the top
    }
    // if Down arrow, move down. go back to top if we are at the bottom.
    else if (event.key.code == sf::Keyboard::Down){
        choice = (choice + 1) % ITEM_COUNT;                  //go down, wrap around at the bottom
    }

    //if Enter is pressed, do whatever option is selected
    else if (event.key.code == sf::Keyboard::Enter){
        if (choice == 0){
            game->prepareNewMission(); //roll new usage and build the mansion
            game->changeScreen(SCREEN_INTEL);
        }
        else if (choice == 1){
            game->changeScreen(SCREEN_CREW_VIEW); //just look at the crew
        }
        else if (choice == 2){
            game->changeScreen(SCREEN_INTEL_VIEW); //just look at the map
        }
        else if (choice == 3){
            game->changeScreen(SCREEN_HOWTO); // go to the rules screen
        }
        else{
            game->quit(); //close the game
        }
    }
}

//update the timer so the crew at the bottom can walk
void MenuScreen::update(float dt){
    timer = timer + dt; //add the time passed since last frame
}

//  draw the whole menu screen
void MenuScreen::draw(sf::RenderWindow& window){

    //Background: the lobby picture, made big enough to cover the whole window, then darkened.
    sf::Sprite picture(background);
    float scale = WINDOW_WIDTH / 504.0f;

    //check if we need to scale based on height too, so the picture fits perfectly
    if (WINDOW_HEIGHT / 394.0f > scale){
        scale = WINDOW_HEIGHT / 394.0f;
    }
    picture.setScale(scale, scale);


    // center the picture on the screen
    picture.setPosition((WINDOW_WIDTH - 504 * scale) / 2.0f, (WINDOW_HEIGHT - 394 * scale) / 2.0f);
    window.draw(picture);

    //put a dark see through box over the picture so the text is easy to read
    sf::RectangleShape shade(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    shade.setFillColor(sf::Color(12, 8, 12, 170));
    window.draw(shade);

    //draw the game title
    drawText(window, "THE CROW CLUB HEIST", 62, WINDOW_WIDTH / 2.0f, 90, COLOR_GOLD, ALIGN_CENTER);
    drawText(window, "A six-thief heist through a guarded mansion", 22, WINDOW_WIDTH / 2.0f, 170, COLOR_CREAM, ALIGN_CENTER);

    //the menu buttons, drawn with a loop (one pass for each item)
    for (int i = 0; i < ITEM_COUNT; i++){
        float boxX = WINDOW_WIDTH / 2.0f - 190;
        float boxY = 215.0f + i * 66.0f;
        
        //if this is the item we are currently hovering on, make it gold and white text
        if (i == choice){
            drawPanel(window, boxX, boxY, 380, 54, COLOR_PANEL_SELECTED, COLOR_GOLD);
            drawText(window, items[i], 26, WINDOW_WIDTH / 2.0f, boxY + 10, sf::Color::White, ALIGN_CENTER);
        }
        //otherwis make it normal brown and cream text
        else{
            drawPanel(window, boxX, boxY, 380, 54, COLOR_PANEL, sf::Color(120, 90, 60));
            drawText(window, items[i], 26, WINDOW_WIDTH / 2.0f, boxY + 10, COLOR_CREAM, ALIGN_CENTER);
        }
    }

    //The six crew members walking on the spot along the bottom.
    //calculate the animation frame (1 or 2) to make them walk
    int column = 1 + (int)(timer * 6.0f) % 2;
    int crewCount = game->getCrewCount();
    
    //loop through all 6 crew members and draw them at the bottom
    for (int i = 0; i < crewCount; i++){
        float x = WINDOW_WIDTH / 2.0f - crewCount * 55.0f + i * 110.0f + 25.0f;
        game->getCrewMember(i)->drawAt(window, x, 585, 5.0f, DIR_DOWN, column);
    }

    //show the instructions at the very bottom
    drawText(window, "Up / Down to choose, Enter to select", 18, WINDOW_WIDTH / 2.0f, 680, COLOR_GOLD, ALIGN_CENTER);
}