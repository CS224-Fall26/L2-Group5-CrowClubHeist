//one floor of the mansion. It owns its 3 rooms (composition). The rooms are kept in a
//LINKED LIST: the floor only remembers the first room (head), and every Room points to the next one
//The last room of the floor is the vault.

//head -> [Room 1] -> [Room 2] -> [Room 3 (vault)] -> nullptr

#ifndef FLOOR_HPP
#define FLOOR_HPP
#include "Room.hpp"

class Floor{
    private:
        int floorNumber;
        Room* head; //the first room of the list (nullptr = no rooms yet)

    public:
        Floor(int newFloorNumber);

        //walks along the list and deletes every room
        ~Floor();

        //puts the room at the END of the list
        void addRoom(Room* room);   

        Room* getFirstRoom();
        int getRoomCount() const;
        int getFloorNumber() const;

        // how many rooms have isCleared() == true
        int countClearedRooms() const; 
};

#endif
