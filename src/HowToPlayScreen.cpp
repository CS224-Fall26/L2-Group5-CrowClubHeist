#include "HowToPlayScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"

//the rules. Adding a line here automatically adds it to the screen (the loop below draws them all)
//this is a constant array of strings, so it is easy to edit the text later
const int LINE_COUNT = 10;
const std::string RULE_LINES[LINE_COUNT] = {
    "1. Pick one of three artifacts from Kaz's intel. Deeper floors are harder.",
    "2. Each crew member has Strength, Usage (actions left) and Puzzle time.",
    "3. Walk with the arrow keys or WASD. TAB (or 1-6) changes the active crew member.",
    "4. Walk into a guard to fight. Compare your Strength (C) with the guard's (G):",
    "      C > G : the guard is defeated and you lose 1 usage.",
    "      C = G : your crew member is lost and the guard's strength is halved.",
    "      C < G : your crew member is lost and the guard stays the same.",
    "5. Defeat every guard in a room to unlock its door. The last room is the vault.",
    "6. In the vault, stand next to the artifact and press E to steal it.",
    "7. If nobody with usage left can act, the mission fails."
};

//set screen up. loads a background image to make the text screen look nicer.
HowToPlayScreen::HowToPlayScreen(Game* newGame) : Screen(newGame){
    background.loadFromFile("assets/rooms/kitchen.png");
}

//handles key presses. pressing Esc or Enter goes back to the menu.
void HowToPlayScreen::handleEvent(const sf::Event& event){
    if (event.type == sf::Event::KeyPressed){//ignore all events except key presses
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter){
            game->changeScreen(SCREEN_MENU);
        }
    }
}

//nothing to update on this screen (no timers or animations)
void HowToPlayScreen::update(float dt){}

//draws the screen
void HowToPlayScreen::draw(sf::RenderWindow& window){
    //draw the background picture first, scaled up to fill the screen
    sf::Sprite picture(background);
    picture.setScale(4.5f, 4.5f);
    window.draw(picture);

    //draw a dark transparent box over the picture so the text is easy to read
    sf::RectangleShape shade(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    shade.setFillColor(sf::Color(12, 8, 12, 190));
    window.draw(shade);

    //draw the main panel and the title
    drawPanel(window, 120, 50, 1040, 620);
    drawText(window, "HOW TO PLAY", 40, 640, 70, COLOR_GOLD, ALIGN_CENTER);

    //loop through the rules array and draw each line.
    //the y position goes down by 42 pixels for every line.
    for (int i = 0; i < LINE_COUNT; i++){
        drawText(window, RULE_LINES[i], 22, 160, 150.0f + i * 42.0f);
    }
    
    //draw the instruction at the bottom
    drawText(window, "Press Esc to go back", 18, 640, 590, COLOR_GOLD, ALIGN_CENTER);
}
