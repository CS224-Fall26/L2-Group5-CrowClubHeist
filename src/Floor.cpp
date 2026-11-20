#include "Floor.hpp"

//floorclass manages a LINKED LIST  of Room objects
//representing the rooms on a specific floor of the mansion. 
//it provides methods to add rooms, retrieve the first room, count the totalno of rooms, count how many rooms have been cleared
Floor::Floor(int newFloorNumber){
    floorNumber = newFloorNumber;
    head = nullptr;
}

Floor::~Floor(){ //walks along the list and deletes every room
    //visit every room in the list. remember the next room BEFORE deleting the current one.
    //why remember the next room? because once we delete the current room, we cannot ask it for its next room anymore.
    Room* current = head;
    while (current != nullptr){
        Room* next = current->getNextRoom();
        delete current;
        current = next;
    }
}

void Floor::addRoom(Room* room){ //puts the room at the END of the list
    if (head == nullptr){   //the list is empty, so this is the first room
        head = room;   //the head of the list points to the first room
        return;
    }
    //otherwise walk to the last room and link the new room after it.
    Room* last = head;
    while (last->getNextRoom() != nullptr){ //while there is a next room, keep walking
        last = last->getNextRoom(); //move to the next room
    }
    last->setNextRoom(room);
}

Room* Floor::getFirstRoom(){ //returns the first room of the list (nullptr = no rooms yet)
    return head;
}

int Floor::getRoomCount() const{    //how many rooms are in the list
    int count = 0;
    Room* current = head;
    
    while(current != nullptr){ //while there is a room, count it and move to the next one
        count = count + 1;
        current = current->getNextRoom();
    }
    return count;
}

int Floor::getFloorNumber() const{
    return floorNumber;
}

int Floor::countClearedRooms() const{ //how many rooms have isCleared() == true
    int count = 0;
    Room* current = head;
    while (current != nullptr){ //while there is a room, check if it's cleared and move to the next one
        if (current->isCleared() == true){ //if the room is cleared, count it
            count = count + 1;
        }
        current = current->getNextRoom(); //move to the next room
    }
    return count;
}
