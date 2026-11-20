#include "Artifact.hpp"
#include <iostream>

//constructor: name, floor number, picture file, description
Artifact::Artifact(std::string newName, int newFloorNumber, std::string iconFile, std::string newDescription){
    name = newName;
    floorNumber = newFloorNumber;
    description = newDescription;

    //loadFromFile returns false if the file could not be loaded
    if (icon.loadFromFile("assets/items/" + iconFile) == false){
        std::cout << "Could not load the picture " << iconFile << std::endl;
    }
}

std::string Artifact::getName() const{ //returns thename of the artifact
    return name;}

int Artifact::getFloorNumber() const{ //returnsthe floor number of the artifact
    return floorNumber;
}

std::string Artifact::getDescription() const{//returns the description of the artifact
    return description;
}

const sf::Texture& Artifact::getIcon() const{//returns the icon of the artifact
    return icon;
}
