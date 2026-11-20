#include "PuzzleRoom.hpp"
#include <cmath>

PuzzleRoom::PuzzleRoom(RoomInfo newInfo, int newRoomNumber, int floorNumber, Artifact* newArtifact)
    : GuardRoom(newInfo, newRoomNumber, floorNumber){
    artifact = newArtifact;
}

sf::FloatRect PuzzleRoom::getPedestalBox() const{
    return sf::FloatRect(info.pedestalX - 9.0f, info.pedestalY - 3.0f, 18.0f, 8.0f);
}

bool PuzzleRoom::isVault() const{
    return true;
}

Artifact* PuzzleRoom::getArtifact(){
    return artifact;
}

bool PuzzleRoom::isBlocked(const sf::FloatRect& feetBox) const{
    //first ask the normal room (walls). Then also block the pedestal.
    if (GuardRoom::isBlocked(feetBox) == true){
        return true;
    }
    return getPedestalBox().intersects(feetBox);
}

bool PuzzleRoom::isNearArtifact(float feetX, float feetY) const{
    //Distance between two points (Pythagoras): sqrt(dx*dx + dy*dy)
    float dx = feetX - info.pedestalX;
    float dy = feetY - info.pedestalY;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < 22.0f;
}

void PuzzleRoom::draw(sf::RenderWindow& window){
    GuardRoom::draw(window);//the room and its guards

    //the pedestal: a small brown box.
    sf::FloatRect box = getPedestalBox();
    sf::RectangleShape pedestal(sf::Vector2f(box.width, box.height));
    pedestal.setPosition(box.left, box.top);
    pedestal.setFillColor(sf::Color(96, 62, 40));
    pedestal.setOutlineColor(sf::Color(230, 176, 96));
    pedestal.setOutlineThickness(1.0f);
    window.draw(pedestal);

    //the artifact floats up and down a little (sin makes a smooth wave).
    float bob = std::sin(timer * 3.0f) * 1.5f;
    sf::Sprite icon(artifact->getIcon());
    icon.setPosition(info.pedestalX - 6.0f, info.pedestalY - 14.0f + bob);
    window.draw(icon);

    //when the guards are gone, a golden ring tells the player "take it".
    if (isCleared() == true){
        sf::CircleShape ring(14.0f);
        ring.setOrigin(14.0f, 14.0f);
        ring.setPosition(info.pedestalX, info.pedestalY - 8.0f);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(1.0f);
        ring.setOutlineColor(sf::Color(255, 230, 120, (int)(150 + 100 * std::sin(timer * 5.0f))));
        window.draw(ring);
    }
}
