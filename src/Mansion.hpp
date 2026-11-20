// The whole building: 3 floors with 3 rooms each. A new random mansion is built for every mission
//cOMPOSITION: the Mansion owns its Floors (it deletes them in its destructor
//AGGREGATION: it contains the 3 Artifacts but does not own them (they are owned by Game)
//it puts one artifact in each floor's vault

#ifndef MANSION_HPP
#define MANSION_HPP
#include "Floor.hpp"
#include <string>

//how many floors the mansion has
const int MAX_FLOORS = 3;

class Mansion{
    private:
        std::string name;   //the name of the mansion ("The Crow Club Mansion")
        Floor* floors[MAX_FLOORS];      //pointers to the 3 floors (we create them with 'new', so we must delete them later)
        int floorCount;                 //how many floors are really in the array
        Artifact* artifacts[MAX_ARTIFACTS]; // only pointers: the artifacts are not deleted here (they are owned by Game)
        int artifactCount;              //how many artifacts are really in the array

    public:
        // Builds the mansion using the room facts from rooms.txt and the artifact list
        Mansion(RoomInfo roomInfos[], int roomInfoCount, Artifact* newArtifacts[], int newArtifactCount);
        
        //the destructor cleans up the floors when the mansion is destroyed
        ~Mansion();

        //returns the mansion's name
        std::string getName() const;

        //number is 1, 2 or 3 (returns the floor at that number)
        Floor* getFloor(int number);      
        //returns how many floors there are (3)
        int getFloorCount() const;        
        
        //returns how many artifacts there are (3)
        int getArtifactCount() const;     
        
        //returns the artifact at that index (0, 1 or 2)
        Artifact* getArtifact(int index); 
};

#endif