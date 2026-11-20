// Small helper functions for drawing text and boxes. Every screen uses them,
// so we write the drawing code once instead of copying it everywhere.

// LEARN (SFML text and fonts): https://www.sfml-dev.org/tutorials/2.6/graphics-text.php
// LEARN (SFML shapes):         https://www.sfml-dev.org/tutorials/2.6/graphics-shape.php
#ifndef UI_HPP
#define UI_HPP
#include <SFML/Graphics.hpp>
#include <string>

//the colours we use in the whole game. sf::Color(red, green, blue) - each number is 0 to 255.
const sf::Color COLOR_CREAM(245, 230, 200);          //light cream color for normal text
const sf::Color COLOR_GOLD(230, 176, 96);            //gold color for titles and highlights
const sf::Color COLOR_GREEN(90, 210, 120);           //green for success and usage dots
const sf::Color COLOR_RED(230, 80, 80);              //red for lost crew members and fights
const sf::Color COLOR_PANEL(30, 21, 17, 225);        //the 4th number is transparency (almost solid dark brown)
const sf::Color COLOR_PANEL_SELECTED(90, 55, 30, 235); //lighter brown for the selected menu option

//where the text sits compared to the x position we give.
const int ALIGN_LEFT = 0;    //text starts at the x position
const int ALIGN_CENTER = 1;  //text is centered on the x position
const int ALIGN_RIGHT = 2;   //text ends at the x position

//gives back the one font used by the whole game (it is loaded only the first time).
sf::Font& getFont();

//draws a line of text with a small dark shadow so it is easy to read.
//you can pass a color and alignment, but if you don't, it uses cream and left by default.
void drawText(sf::RenderWindow& window, const std::string& words, int size, float x, float y,
              sf::Color color = COLOR_CREAM, int align = ALIGN_LEFT);

//draws a dark box with a golden border (used for menus, cards and the HUD).
//you can pass custom colors, but it uses the default dark colors if you don't.
void drawPanel(sf::RenderWindow& window, float x, float y, float width, float height,
               sf::Color fillColor = COLOR_PANEL, sf::Color borderColor = COLOR_GOLD);

#endif