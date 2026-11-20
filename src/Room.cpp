// The room pictures are from "Fantasyville - Cozy Interiors" by JPKG (see CREDITS.md).
// The fire pictures were cut from the animated fireplace of the same pack.

// LEARN (drawing a sprite): https://www.sfml-dev.org/tutorials/2.6/graphics-sprite.php
#include "Room.hpp"
#include <iostream>
#include <cmath>

Room::Room(RoomInfo newInfo, int newRoomNumber){
    info = newInfo;
    timer = 0.0f;
    roomNumber = newRoomNumber;
    cleared = true;          // a plain room has nothing to defeat. GuardRoom sets this to false.
    nextRoom = nullptr;

    if (background.loadFromFile("assets/" + info.pictureFile) == false){
        std::cout << "Could not load the room picture " << info.pictureFile << std::endl;
    }
    if (fireTexture.loadFromFile("assets/fx/fire_strip.png") == false){
        std::cout << "Could not load the fire picture" << std::endl;
    }
}

Room::~Room(){}

std::string Room::getName() const{
    return info.name;
}

float Room::getWidth() const{
    return info.width;
}

float Room::getHeight() const{
    return info.height;
}

float Room::getSpawnX() const{
    return info.spawnX;
}

float Room::getSpawnY() const{
    return info.spawnY;
}

int Room::getRoomNumber() const{
    return roomNumber;
}

bool Room::isCleared() const{
    return cleared;
}

Room* Room::getNextRoom() const{
    return nextRoom;
}

void Room::setNextRoom(Room* room){
    nextRoom = room;
}

bool Room::isBlocked(const sf::FloatRect& feetBox) const{
    //1) Outside the room picture?
    if (feetBox.left < 0 || feetBox.top < 0){
        return true;
    }
    if (feetBox.left + feetBox.width > info.width || feetBox.top + feetBox.height > info.height){
        return true;
    }

    //2) Touching any wall or furniture rectangle? (loop over the list from rooms.txt)
    for (int i = 0; i < info.wallCount; i++){
        if (info.walls[i].intersects(feetBox) == true)
        {
            return true;
        }
    }
    return false;
}

bool Room::isAtDoor(const sf::FloatRect& feetBox) const{
    if (info.hasDoor == false){
        return false;
    }
    return info.door.intersects(feetBox);
}

void Room::onEnter(){}

void Room::update(float dt){
    timer = timer + dt;
}

void Room::draw(sf::RenderWindow& window){
    //1) The room picture.
    backgroundSprite.setTexture(background);
    window.draw(backgroundSprite);

    //2)The moving fire. The fire picture has 4 frames (35 x 22 pixels each).
    //We show a different frame 7 times per second, which makes it flicker.
    if (info.hasFire == true){
        int frame = (int)(timer * 7.0f) % 4;
        fireSprite.setTexture(fireTexture);
        fireSprite.setTextureRect(sf::IntRect(frame * 35, 0, 35, 22));
        fireSprite.setPosition(info.fire.left, info.fire.top);
        window.draw(fireSprite);
    }

    //3)The door: a coloured box. Red = locked, green = open (it slowly pulses).
    if (info.hasDoor == true){
        float pulse = 0.5f + 0.5f * std::sin(timer * 4.0f);
        sf::Color color;
        if (isCleared() == true){
            color = sf::Color(90, 230, 130);
        }
        else{
            color = sf::Color(230, 80, 80);
        }
        sf::RectangleShape doorBox(sf::Vector2f(info.door.width, info.door.height));
        doorBox.setPosition(info.door.left, info.door.top);
        doorBox.setFillColor(sf::Color(color.r, color.g, color.b, (int)(70 + 80 * pulse)));
        doorBox.setOutlineColor(color);
        doorBox.setOutlineThickness(1.0f);
        window.draw(doorBox);
    }
}

bool Room::isVault() const{
    return false;
}

int Room::getGuardCount() const{
    return 0;
}

Guard* Room::getGuard(int index){
    return nullptr;
}

Guard* Room::getTouchedGuard(const sf::FloatRect& feetBox){
    return nullptr;
}

int Room::fight(CrewMember* crew, Guard* guard){
    //a plain room has no guards, so this is never used. GuardRoom has the real version.
    return FIGHT_LOSS;
}

bool Room::isNearArtifact(float feetX, float feetY) const{
    return false;
}
