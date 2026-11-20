#include "RoomInfo.hpp"
#include <fstream>
#include <iostream>

//sets up a blank room with default values (0 size, no door, no fire).
RoomInfo::RoomInfo(){
    width = 0;
    height = 0;
    hasDoor = false;
    spawnX = 0;
    spawnY = 0;
    pedestalX = 0;
    pedestalY = 0;
    hasFire = false;
    wallCount = 0;//no walls yet
    guardSpotCount = 0;//no guard spots yet
}

//reads the rooms.txt file and turns it into a list of RoomInfo objects
int loadRoomInfos(std::string fileName, RoomInfo list[]){
    int listCount = 0;//how many rooms we have stored in the array so far
    std::ifstream file(fileName);
    
    //if the file didn't open, print an error and return an empty list
    if (file.is_open() == false){
        std::cout << "Could not open " << fileName << " (run the game from the project folder)" << std::endl;
        return listCount;
    }



    RoomInfo current;
    std::string word;

    //'file >> word' reads one word at a time. The first word of every line tells us what the line is
    // so we keep reading words until the file ends
    while (file >> word){
        float a, b, c, d; //these will hold the numbers (like x, y, width, height)

        if (word[0] == '#'){
            //a comment line: throw away the rest of the line
            std::getline(file, word);
        }
        else if (word == "room"){
            current = RoomInfo();     ///start again with an empty room
            file >> current.name >> current.pictureFile >> current.width >> current.height;
            
            //in the file we write _ instead of spaces. Change them back to spaces.
            for (int i = 0; i < (int)current.name.size(); i++){
                if (current.name[i] == '_'){
                    current.name[i] = ' ';
                }
            }
        }
        else if (word == "wall"){
            //read the wall's x, y, width, and height and add it to the room.
            file >> a >> b >> c >> d;
            if (current.wallCount < MAX_WALLS){//only add it if the array still has space
                current.walls[current.wallCount] = sf::FloatRect(a, b, c, d);
                current.wallCount++;
            }
        }
        else if (word == "door"){
            //read the door's x, y, width, and height. 
            file >> a >> b >> c >> d;
            current.door = sf::FloatRect(a, b, c, d);
            current.hasDoor = true; //mark that this room has a door
        }
        else if (word == "spawn"){
            //read the spawn point (where the player starts in the room)
            file >> current.spawnX >> current.spawnY;
        }
        else if (word == "guard"){
            //read the guard's x and y position and add it to the list of guard spots
            file >> a >> b;
            if (current.guardSpotCount < MAX_GUARD_SPOTS){//only add it if the array still has space
                current.guardSpots[current.guardSpotCount] = sf::Vector2f(a, b);
                current.guardSpotCount++;
            }
        }
        else if (word == "pedestal"){
            //read the pedestal position (where the artifact sits)
            file >> current.pedestalX >> current.pedestalY;
        }
        else if (word == "fire"){
            //read the fire's x, y, width, and height.
            file >> a >> b >> c >> d;
            current.fire = sf::FloatRect(a, b, c, d);
            current.hasFire = true; //mark that this room has a fire
        }
        else if (word == "end"){
            //"end" means the room is finished. Add it to the main list.
            if (listCount < MAX_ROOM_INFOS){//only add it if the array still has space
                list[listCount] = current;
                listCount++;
            }
        }
    }
    return listCount; //give back how many rooms we found
}

//searches the list of rooms to find one with a specific name.
RoomInfo findRoomInfo(const RoomInfo list[], int listCount, std::string name){
    for (int i = 0; i < listCount; i++){
        if (list[i].name == name){
            return list[i]; //found it, give it back
        }
    }
    
    //if we get here, the room wasn't in the list. print an error and give an empty room.
    std::cout << "Room not found in rooms.txt: " << name << std::endl;
    return RoomInfo();
}