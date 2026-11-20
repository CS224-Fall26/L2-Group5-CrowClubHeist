// RoomInfo holds the facts about ONE room picture: its size, where the walls and furniture are,
// where the door is, where guards may stand... We do not write these numbers inside the C++ code.
// They are read from the text file data/rooms.txt, so you can change a room without recompiling.

// The room pictures come from "Fantasyville  Cozy Interiors" by JPKG
//(https://aku-jpkg.itch.io/fantasyville-cozy-interiors), see CREDITS.md.

//LEARN (reading a text file with ifstream): https://www.learncpp.com  (lesson "Basic file I/O")

#ifndef ROOMINFO_HPP
#define ROOMINFO_HPP
#include <SFML/Graphics.hpp>
#include <string>

//the biggest sizes our plain arrays can hold
const int MAX_WALLS = 100;
const int MAX_GUARD_SPOTS = 20;
const int MAX_ROOM_INFOS = 30;

class RoomInfo{
    public:
        std::string name;
        std::string pictureFile;
        float width;
        float height;
        sf::FloatRect walls[MAX_WALLS];//places the crew cannot walk
        int wallCount;//how many walls are really used in the array
        bool hasDoor;
        sf::FloatRect door;
        float spawnX;
        float spawnY;
        sf::Vector2f guardSpots[MAX_GUARD_SPOTS];//possible guard positions
        int guardSpotCount;//how many guard spots are really used in the array
        float pedestalX;
        float pedestalY;
        bool hasFire;
        sf::FloatRect fire;

        RoomInfo();
};

//reads every room from the file and fills the list. returns how many rooms were read.
int loadRoomInfos(std::string fileName, RoomInfo list[]);

//looks for a room by its name in the list.
RoomInfo findRoomInfo(const RoomInfo list[], int listCount, std::string name);

#endif
