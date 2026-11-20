#include "Character.hpp"
#include <cmath>
#include <iostream>
//the constructor sets the name, strength, and loads the sprite sheet picture.
Character::Character(std::string newName, int newStrength, std::string sheetName){
    name = newName;
    strength = newStrength;
    x = 0.0f;
    y = 0.0f;
    speed = 70.0f;
    direction = DIR_DOWN;
    animationTimer = 0.0f;
    isMoving = false;

    //all character pictures live in assets/characters and are called <name>_sheet.png
    std::string path = "assets/characters/" + sheetName + "_sheet.png";
    if (texture.loadFromFile(path) == false){ //loadFromFile returns false if the file could not be loaded
        std::cout << "Could not load " << path << std::endl;
    }
}

//virtual destructor so child classes can clean up after themselves
Character::~Character(){}

std::string Character::getName() const{
    return name;}

int Character::getStrength() const{
    return strength;
}

float Character::getX() const{//returns the x position of the character in pixels inside the room
    return x;
}

float Character::getY() const{//returns the y position of the character in pixels inside the room
    return y;
}

void Character::setPosition(float newX, float newY){//sets the x and y position of the character in pixels inside the room
    x = newX;
    y = newY;
}

void Character::move(float dx, float dy){//moves the character by dx and dy pixels. (It does NOT check walls: PlayScreen asks the room first)
    x = x + dx;
    y = y + dy;
}

void Character::setDirection(int newDirection){//sets the direction the character is facing. One of DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_UP
    direction = newDirection;
}

//returns the small box at the character's feet. 
//walls are checked against this box, so the head can overlap furniture and it looks like a topdown game
//When the player tries to walk, the game checks if this tiny box hits a wall. 
//If it doesn't, the player moves. 
//It also provides getFeetCenterX() and getFeetCenterY() 
//so the game knows exactly where the center of the feet are 
sf::FloatRect Character::getFeetBox() const{
    return getFeetBoxAt(x, y);
}

//returns the same box, but as if the character stood at another position (used to test a move before making it)
sf::FloatRect Character::getFeetBoxAt(float atX, float atY) const{
    //left, top, width, height of the box around the feet
    return sf::FloatRect(atX + 2.0f, atY + 10.0f, 8.0f, 6.0f);
    //the box is 8 pixels wide and 6 pixels tall, 
    //and is 2 pixels in from the left and 10 pixels down from the top of the character
}

float Character::getSpeed() const{return speed;}

float Character::getFeetCenterX() const{
    return x + FRAME_WIDTH / 2.0f; 
    //the center of the feet box is 6 pixels in from the left of the character 
    //so we can use that to aim at doors and guards
}

float Character::getFeetCenterY() const{
    return y + FRAME_HEIGHT - 3.0f;//the center of the feet box is 3 pixels up from the bottom of the character
}

void Character::faceTowards(float dx, float dy){
    //if we move more sideways than up/down, face left or right. Otherwise face up or down.
    if (std::fabs(dx) > std::fabs(dy)){
        if (dx < 0){//if we move left, face left. Otherwise face right.
            direction = DIR_LEFT;
        }
        else{
            direction = DIR_RIGHT;
        }
    }
    else{
        if (dy < 0){
            direction = DIR_UP;
        }
        else{
            direction = DIR_DOWN;
        }
    }
}

//from moving to standing frame
//calls every frame to update the walking animation
//dt = how many seconds the last frame took. moving = whether the character is moving or not
void Character::updateAnimation(float dt, bool moving){
    isMoving = moving;
    if (moving == true){
        animationTimer = animationTimer + dt;//we only count time while moving, so the walking animation stops when we stop
    }
    else{
        animationTimer = 0.0f;//if stops walking reset back to 0
    }
}

//Sprite Sheet (a big grid containing all the different poses and directions)
//draws one chosen frame anywhere on the screen (used by menu)
void Character::drawAt(sf::RenderWindow& window, float screenX, float screenY, float scale, int row, int column){
    sprite.setTexture(texture);
    //cut one small frame out of the big sheet: left, top, width, height.
    sprite.setTextureRect(sf::IntRect(column * FRAME_WIDTH, row * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
    sprite.setScale(scale, scale);//scale the sprite up or down
    sprite.setPosition(screenX, screenY); //set the position of the sprite on the screen
    window.draw(sprite);//draw the sprite on the screen
}

//draws the character at its position inside the room
void Character::draw(sf::RenderWindow& window){
    //column 0 = standing. While walking we switch between column 1 and 2, 8 times a second.
    int column = 0;
    if (isMoving == true){
        column = 1 + (int)(animationTimer * 8.0f) % 2;
    }
    drawAt(window, x, y, 1.0f, direction, column); //draw the character at its position inside the room, scale = 1.0f, row = direction, column = column
}
