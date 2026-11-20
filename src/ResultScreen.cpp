#include "ResultScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"
#include <cmath>
#include <string>

//sets up the result screen. picks a background picture based on whether we won or lost.
ResultScreen::ResultScreen(Game* newGame, bool newWon) : Screen(newGame) {
    won = newWon;
    timer = 0.0f;
    if (won == true) {
        background.loadFromFile("assets/rooms/study.png"); // happy background for winning
    }
    else {
        background.loadFromFile("assets/rooms/bedroom.png"); // sad background for losing
    }
}

//key presses: Enter or Esc goes back to the main menu
void ResultScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape) {
            game->changeScreen(SCREEN_MENU);
        }
    }
}

//updates the timer so the artifact can bob up and down
void ResultScreen::update(float dt) {
    timer = timer + dt;
}

//draws the whole result screen (poori screen draw karo)
void ResultScreen::draw(sf::RenderWindow& window) {
    // Big darkened room picture behind everything.
    sf::Sprite picture(background);
    picture.setScale(5.0f, 5.0f);
    picture.setPosition((WINDOW_WIDTH - background.getSize().x * 5.0f) / 2.0f, -40.0f);
    window.draw(picture);
    
    //add a dark see-through shade so the text is easy to read
    sf::RectangleShape shade(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    shade.setFillColor(sf::Color(10, 6, 10, 200));
    window.draw(shade);

    //show the big title depending on whether the player won or lost
    if (won == true) {
        drawText(window, "MISSION SUCCESSFUL", 56, 640, 60, COLOR_GREEN, ALIGN_CENTER);
    }
    else {
        drawText(window, "MISSION FAILED", 56, 640, 60, COLOR_RED, ALIGN_CENTER);
    }
    
    //get the mission data to show the results
    Mission* mission = game->getMission();
    if (mission == nullptr) {
        return; // safety check: if there is no mission, just stop drawing
    }

    //Won: say who stole what. Lost: say why (the reason is kept in the Mission).
    if (won == true) {
        drawText(window, mission->getActive()->getName() + " stole the " + mission->getTarget()->getName() + "!", 24, 640, 140, COLOR_CREAM, ALIGN_CENTER);
    }
    else {
        drawText(window, mission->getFailureReason(), 24, 640, 140, COLOR_CREAM, ALIGN_CENTER);
    }

    //On a win, show the stolen artifact bobbing up and down.
    //sin() makes it move up and down smoothly like a wave.
    if (won == true) {
        sf::Sprite icon(mission->getTarget()->getIcon());
        icon.setScale(8.0f, 8.0f);
        icon.setPosition(640 - 48.0f, 190.0f + 6.0f * std::sin(timer * 3.0f));
        window.draw(icon);
    }

    //the crew report: one line per crew member.
    drawPanel(window, 340, 320, 600, 300);
    drawText(window, "Crew report   -   rooms cleared: " + std::to_string(mission->getRoomsCleared()) + "/3", 24, 640, 335, COLOR_GOLD, ALIGN_CENTER);
    
    //we will loop through all 6 crew members and show their final status
    for (int i = 0; i < mission->getCrewCount(); i++) {
        CrewMember* member = mission->getCrewMember(i);
        float y = 385.0f + i * 36.0f;

        std::string status;
        sf::Color color = COLOR_CREAM;
        
        //check if they were lost, tired, or still had usage left
        if (member->isLost() == true){
            status = "LOST";
            color = COLOR_RED;
        }
        else if (member->isExhausted() == true) {
            status = "exhausted";
        }
        else {
            status = "usage left: " + std::to_string(member->getUsage());
        }
        
        // draw the crew member's name on the left and their status on the right
        drawText(window, member->getName(), 20, 380, y, color);
        drawText(window, status, 20, 900, y, color, ALIGN_RIGHT);
    }
    
    //instruction to go back to the menu
    drawText(window, "Press Enter to return to the menu", 20, 640, 660, COLOR_GOLD, ALIGN_CENTER);
}