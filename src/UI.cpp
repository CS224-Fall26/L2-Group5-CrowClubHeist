#include "UI.hpp"
#include <iostream>

//returns the font used by the whole game. 
//we use 'static' so the font is only loaded once and stays in memory, making it fast.
sf::Font& getFont(){
    //'static' means these variables are created once and keep their value between calls.
    static sf::Font font;
    static bool loaded = false;

    //if we haven't loaded the font yet, load it now.
    if (loaded == false){
        // The font file comes with the project (DejaVu Sans Bold, free licence).
        if (font.loadFromFile("assets/fonts/DejaVuSans-Bold.ttf") == false){
            std::cout << "Could not load the font file." << std::endl;
        }
        loaded = true; //mark it as loaded so we don't load it again next time
    }
    return font;
}

//draws a line of text on the screen with a small drop shadow to make it easy to read.
void drawText(sf::RenderWindow& window, const std::string& words, int size, float x, float y,
              sf::Color color, int align){
    sf::Text text;
    text.setFont(getFont());//use our loaded font
    text.setString(words);//set the actual text words
    text.setCharacterSize(size); //set how big the text should be

    //to centre or right-align we need to know how wide the text is.
    //we measure the text box and shift the x position accordingly.
    sf::FloatRect box = text.getLocalBounds();
    if (align == ALIGN_CENTER){
        x = x - box.width / 2.0f;
    }
    else if (align == ALIGN_RIGHT){
        x = x - box.width;
    }

    //first draw the shadow (black, moved 2 pixels), then the real text on top.
    text.setPosition(x + 2.0f, y + 2.0f);
    text.setFillColor(sf::Color(0, 0, 0, 200)); //semi-transparent black shadow
    window.draw(text);

    //now draw the actual text at the original position with the real color.
    text.setPosition(x, y);
    text.setFillColor(color);
    window.draw(text);
}

//draws a fancy box with two borders (used for menus, HUD panels, and the fight box).
void drawPanel(sf::RenderWindow& window, float x, float y, float width, float height,
               sf::Color fillColor, sf::Color borderColor){
    //the outer rectangle: dark fill with a brown outline.
    sf::RectangleShape outer(sf::Vector2f(width, height));
    outer.setPosition(x, y);
    outer.setFillColor(fillColor);
    outer.setOutlineColor(sf::Color(74, 47, 31));
    outer.setOutlineThickness(5.0f); //thick border on the outside
    window.draw(outer);

    //a thin golden line a little inside the box.
    //we make it slightly smaller (width - 8) and move it in by 4 pixels.
    sf::RectangleShape inner(sf::Vector2f(width - 8.0f, height - 8.0f));
    inner.setPosition(x + 4.0f, y + 4.0f);
    inner.setFillColor(sf::Color::Transparent); //no fill, just a border
    inner.setOutlineColor(borderColor);
    inner.setOutlineThickness(2.0f); //thinner border on the inside
    window.draw(inner);
}