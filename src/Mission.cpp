#include "Mission.hpp"

Mission::Mission(CrewMember* newCrew[], int newCrewCount, Artifact* newTarget, RoomInfo roomInfos[], int roomInfoCount,
                 Artifact* artifacts[], int artifactCount, int starterIndex){
    crewCount = newCrewCount;
    for (int i = 0; i < crewCount; i++){//copy the pointers one by one (not the crew members themselves)
        crew[i] = newCrew[i];
    }
    target = newTarget;
    mansion = new Mansion(roomInfos, roomInfoCount, artifacts, artifactCount);
    activeIndex = starterIndex;
    status = "In progress";
    failureReason = "";
    //the crew starts in the first room of the target floor.
    currentRoom = mansion->getFloor(target->getFloorNumber())->getFirstRoom();
}

Mission::~Mission(){
    delete mansion;//the crew and the artifacts are NOT deleted here, Game owns them
}

Artifact* Mission::getTarget(){
    return target;
}

int Mission::getFloorNumber() const{
    //the crew plays the floor where the target artifact is.
    return target->getFloorNumber();
}

Mansion* Mission::getMansion(){
    return mansion;
}

Room* Mission::getCurrentRoom(){
    return currentRoom;
}

bool Mission::goToNextRoom(){
    //follow the link to the next room of the floor.
    Room* next = currentRoom->getNextRoom();
    if (next == nullptr){
        return false;   //this was the last room (the vault)
    }
    currentRoom = next;
    return true;
}

int Mission::getRoomsCleared(){
    return mansion->getFloor(getFloorNumber())->countClearedRooms();
}

void Mission::setWon(){
    status = "Won";
}

void Mission::setLost(std::string reason){
    status = "Lost";
    failureReason = reason;
}

bool Mission::isWon() const{
    return status == "Won";
}

bool Mission::isLost() const{
    return status == "Lost";
}

std::string Mission::getStatus() const{//returns "In progress", "Won" or "Lost"
    return status;
}

std::string Mission::getFailureReason() const{
    return failureReason;
}

int Mission::getCrewCount() const{
    return crewCount;
}

CrewMember* Mission::getCrewMember(int index){
    return crew[index];
}

CrewMember* Mission::getActive(){
    return crew[activeIndex];
}

int Mission::getActiveIndex() const{
    return activeIndex;
}

bool Mission::selectCrew(int index){ 
    if (index < 0 || index >= crewCount){ //invalid index, so we cannot switch to that crew member
        return false;
    }
    if (crew[index]->canAct() == false){ //this crew member is exhausted or lost, so we cannot switch to them
        return false;
    }
    activeIndex = index;
    return true;
}

bool Mission::switchToNextCrew(){ //switches to the next crew member who can act. Returns false if there is none.
    int count = crewCount;  //how many crew members there are
    for (int step = 1; step < count; step++){ //try the next crew member, then the next, etc. until we have checked all of them
        int index = (activeIndex + step) % count; //% makes the counting wrap around to 0
        if (crew[index]->canAct() == true){   //this crew member can act, so we switch to them
            activeIndex = index;
            return true;
        }
    }
    return false;
}

bool Mission::makeSureActiveCanAct(){//if the active crew member is exhausted or lost, switch to the next one who can act. If there is none, the mission is lost.
    if (crew[activeIndex]->canAct() == true){//the active crew member can act, so we are fine
        return true;
    }
    return switchToNextCrew();//switch to the next crew member who can act. if there is none, the mission is lost.
}
