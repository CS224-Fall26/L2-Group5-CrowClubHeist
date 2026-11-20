//Room is the BASE CLASS of all rooms. It draws the room picture and the animated fire,
//knows where the walls are and where the door is.
//GuardRoom (a room with guards) inherits from Room, and PuzzleRoom (the vault) inherits from GuardRoom:
//        Room  ->  GuardRoom  ->  PuzzleRoom

//The functions marked 'virtual' do nothing special here. Child classes REPLACE them (override).
//That is POLYMORPHISM: the PlayScreen only holds a Room* pointer, but when it calls
//room->draw() the correct version (Room, GuardRoom or PuzzleRoom) runs automatically.

#ifndef ROOM_HPP
#define ROOM_HPP

#include "RoomInfo.hpp"
#include "CrewMember.hpp"
#include "Guard.hpp"
#include "Artifact.hpp"
#include <SFML/Graphics.hpp>

//thethree possible results of a fight
const int FIGHT_WIN = 0;
const int FIGHT_TIE = 1;
const int FIGHT_LOSS = 2;

class Room{
    protected:
        RoomInfo info;// size, walls, door... read from data/rooms.txt
        sf::Texture background;// the room picture
        sf::Sprite backgroundSprite;
        sf::Texture fireTexture;// 4 fire pictures side by side
        sf::Sprite fireSprite;
        float timer;// seconds since the room was created (drives the animations)
        int roomNumber;// 1, 2 or 3 inside its floor (3 = the vault)
        bool cleared;// true when there is nothing left to defeat (door open)
        Room* nextRoom;// the next room of the floor's linked list (nullptr = last room)

    public:
        Room(RoomInfo newInfo, int newRoomNumber);
        virtual ~Room();

        std::string getName() const;
        float getWidth() const;
        float getHeight() const;
        float getSpawnX() const;
        float getSpawnY() const;
        int getRoomNumber() const;
        bool isCleared() const;//the door is open when the room is cleared

        //linked list: every room points to the next room of its floor.
        Room* getNextRoom() const;
        void setNextRoom(Room* room);

        //true if the box touches a wall, furniture or goes outside the room.
        // 'virtual' because the vault also blocks the pedestal.
        virtual bool isBlocked(const sf::FloatRect& feetBox) const;
        //true if the box touches the door.
        bool isAtDoor(const sf::FloatRect& feetBox) const;

        virtual void onEnter();//called when the crew enters the room
        virtual void update(float dt);//called every frame
        virtual void draw(sf::RenderWindow& window);

        //the functions below have a "nothing happens" version here. GuardRoom / PuzzleRoom replace them.
        virtual bool isVault() const;
        virtual int getGuardCount() const;
        virtual Guard* getGuard(int index);
        virtual Guard* getTouchedGuard(const sf::FloatRect& feetBox);//guard the crew is touching, or nullptr
        virtual int fight(CrewMember* crew, Guard* guard);
        virtual bool isNearArtifact(float feetX, float feetY) const;
};

#endif
