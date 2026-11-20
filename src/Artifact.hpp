//A treasure the crew can be sent to steal. The floor it sits on decides how hard the mission is.
#ifndef ARTIFACT_HPP
#define ARTIFACT_HPP
#include <SFML/Graphics.hpp>
#include <string>

//how many artifacts the game has
const int MAX_ARTIFACTS = 3;

class Artifact{
    private:
        std::string name;
        int floorNumber;
        std::string description;
        sf::Texture icon; //the small picture of the artifact

    public:
        //constructor: name, floor number, picture file, description
        Artifact(std::string newName, int newFloorNumber, std::string iconFile, std::string newDescription);

        //getters
        std::string getName() const;
        int getFloorNumber() const;
        std::string getDescription() const;
        const sf::Texture& getIcon() const;
};

#endif
