#include "Mansion.hpp"
#include "GuardRoom.hpp"
#include "PuzzleRoom.hpp"
#include <cstdlib>

//the vault room of floor 1, 2 and 3.
const std::string VAULT_NAMES[3] = {"Study Vault", "Lobby Vault", "Bedroom"};

//the rooms that can appear as the first two rooms of a floor.
const int NORMAL_ROOM_COUNT = 4;
const std::string NORMAL_NAMES[NORMAL_ROOM_COUNT] = {"Kitchen", "Parlour", "Great Hall", "Bedroom"};

//builds the whole mansion. creates 3 floors, with 2 guard rooms and 1 vault room on each.
Mansion::Mansion(RoomInfo roomInfos[], int roomInfoCount, Artifact* newArtifacts[], int newArtifactCount){
    name = "The Crow Club Mansion";
    floorCount = 0;//no floors yet
    artifactCount = newArtifactCount;
    for (int i = 0; i < artifactCount; i++){//copy the pointers one by one (not the artifacts themselves)
        artifacts[i] = newArtifacts[i];
    }

    //loop through the 3 floors (floor 1, 2, and 3)
    for (int floorNumber = 1; floorNumber <= 3; floorNumber++){
        Floor* floor = new Floor(floorNumber);
        std::string vaultName = VAULT_NAMES[floorNumber - 1];//get the name of the vault for this floor

        // Make a list of the normal rooms, without the one used as this floor's vault.
        // this prevents having a vault room and a normal room with the same name on the same floor.
        std::string pool[NORMAL_ROOM_COUNT];
        int poolCount = 0;//how many names are really in the pool
        for (int i = 0; i < NORMAL_ROOM_COUNT; i++){
            if (NORMAL_NAMES[i] != vaultName){
                pool[poolCount] = NORMAL_NAMES[i];
                poolCount++;
            }
        }

        // Pick two different rooms from the pool at random.
        // 'first' is just a random index.
        int first = rand() % poolCount;
        
        // 'second' is a random index, but we skip the one we already picked.
        // if second is the same or higher than first, we add 1 to shift it over.
        int second = rand() % (poolCount - 1);
        if (second >= first){
            second = second + 1;
        }

        //create the first two rooms as GuardRooms, using the randomly picked names.
        floor->addRoom(new GuardRoom(findRoomInfo(roomInfos, roomInfoCount, pool[first]), 1, floorNumber));
        floor->addRoom(new GuardRoom(findRoomInfo(roomInfos, roomInfoCount, pool[second]), 2, floorNumber));

        // The third room is the vault. It holds the artifact that belongs to this floor.
        // we use PuzzleRoom instead of GuardRoom because the vault needs a puzzle to unlock.
        floor->addRoom(new PuzzleRoom(findRoomInfo(roomInfos, roomInfoCount, vaultName), 3, floorNumber, artifacts[floorNumber - 1]));

        floors[floorCount] = floor;//store the floor in the array
        floorCount++;
    }
}

//delete the floors when the mansion is destroyed so we don't leak memory
Mansion::~Mansion(){
    for (int i = 0; i < floorCount; i++){
        delete floors[i];
    }
}

//returns the name of the mansion
std::string Mansion::getName() const{
    return name;
}

//returns the floor at the given number (1, 2, or 3)
Floor* Mansion::getFloor(int number){
    return floors[number - 1];//subtract 1 because arrays start at 0
}

//returns how many floors the mansion has (3)
int Mansion::getFloorCount() const{
    return floorCount;
}

//returns how many artifacts there are in the mansion (3)
int Mansion::getArtifactCount() const{
    return artifactCount;
}

//returns the artifact at the given index (0, 1, or 2)
Artifact* Mansion::getArtifact(int index){
    return artifacts[index];
}