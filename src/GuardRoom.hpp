//a room with 1 or 2 guards. The door stays locked until every guard is defeated.
//it inherits from Room and REPLACES (overrides) some of Room's virtual functions.

//COMPOSITION: the guards belong to this room only. The room creates them with 'new'
//and the destructor deletes them, so they disappear together with the room.

#ifndef GUARDROOM_HPP
#define GUARDROOM_HPP
#include "Room.hpp"

//a guard room never has more than 2 guards
const int MAX_GUARDS = 2;

class GuardRoom : public Room{ 
    protected:
        Guard* guards[MAX_GUARDS];//an array of pointers to the guards of this room
        int guardCount;//how many guards are really in the array

    public:
        GuardRoom(RoomInfo newInfo, int newRoomNumber, int floorNumber);
        virtual ~GuardRoom();

        virtual void onEnter();   //shows the strength of every guard
        virtual void draw(sf::RenderWindow& window);
        virtual int getGuardCount() const;
        virtual Guard* getGuard(int index);
        virtual Guard* getTouchedGuard(const sf::FloatRect& feetBox);

        //Runs one fight with the rules of the game:
        //crew > guard  : guard defeated, crew loses 1 usage
        //crew == guard : crew lost, guard strength is halved
        //crew < guard  : crew lost, guard unchanged
        virtual int fight(CrewMember* crew, Guard* guard);

        bool allGuardsDefeated() const; //used to update 'cleared' after each fight
    };

#endif
