#include "Guard.hpp"
#include <iostream>
//set the guard up. calls the parent (Character) constructor first, then loads the special defeated picture
Guard::Guard(int newStrength) : Character("Guard", newStrength, "Guard"){ 
    //the part after the colon above calls the Character constructor first
    defeated = false;
    revealed = false;
    if (defeatedTexture.loadFromFile("assets/characters/Guard_defeated.png") == false){
        std::cout << "Could not load Guard_defeated.png" << std::endl;
    }
}

//halves the guard's strength
void Guard::weaken(){
    //whole-number division, so 5 becomes 2
    strength = strength / 2; 

    //make sure the guard never goes below 1 strength
    if (strength < 1){ 
        strength = 1;
    }
}

//marking the guard as defeated so we draw the defeated picture
void Guard::defeat(){
    defeated = true;
}

bool Guard::isDefeated() const{
    return defeated;
}

void Guard::reveal(){
    revealed = true;
}

//marks the guard as revealed (used for stealth/vision)
bool Guard::isRevealed() const{
    return revealed;
}

//returns a larger box around the guard to check if the player touches them
sf::FloatRect Guard::getTouchBox() const{
    sf::FloatRect feet = getFeetBox();
    //make the feet box a little bigger on every side.
    //left and top are shifted back, width and height are increased
    
    //the feet box a little bigger on every side
    return sf::FloatRect(feet.left - 3.0f, feet.top - 4.0f, feet.width + 6.0f, feet.height + 8.0f);
}

void Guard::draw(sf::RenderWindow& window){ 
    if (defeated == true){
        defeatedSprite.setTexture(defeatedTexture);
        defeatedSprite.setPosition(x - 2.0f, y + 4.0f);
        window.draw(defeatedSprite);
    }
    else{
        //not defeated: draw it the normal way, using the parent's draw function
        Character::draw(window);
    }
}
