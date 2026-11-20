#include "GuardRoom.hpp"
#include <cstdlib>
//guard strength range for floor 1, 2 and 3 (index 0 = floor 1).
const int MIN_STRENGTH[3] = {2, 4, 7};
const int MAX_STRENGTH[3] = {5, 8, 10};

//set the room up. creates 1 or 2 guards and places them on the guard spots.
GuardRoom::GuardRoom(RoomInfo newInfo, int newRoomNumber, int floorNumber) : Room(newInfo, newRoomNumber){
    cleared = false;// the room is cleared only after every guard is defeated
    guardCount = 0;//no guards yet

    //1 or 2 guards: rand() % 2 is 0 or 1, plus 1.
    int count = 1 + rand() % 2;
    int spotCount = info.guardSpotCount;
    int firstSpot = rand() % spotCount;

    //get the min and max strength for this floor from the arrays above
    int minStrength = MIN_STRENGTH[floorNumber - 1];
    int maxStrength = MAX_STRENGTH[floorNumber - 1];

    for (int i = 0; i < count; i++){
        //a random strength between min and max (both included).
        int strength = minStrength + rand() % (maxStrength - minStrength + 1);

        //'new' creates the guard and gives back a pointer
        Guard* guard = new Guard(strength);  

        //two guards never get the same spot
        int spot = (firstSpot + i) % spotCount;  
        
        guard->setPosition(info.guardSpots[spot].x, info.guardSpots[spot].y);
        guards[guardCount] = guard;//store the guard in the array
        guardCount++;//one more guard
    }
}

//delete the guards when the room is destroyed so we don't leak memory
GuardRoom::~GuardRoom(){
    //everything created with 'new' must be removed with 'delete'.
    for (int i = 0; i < guardCount; i++){
        delete guards[i];
    }
}

//called when the crew walks into the room. reveals all guards so their strength is shown.
void GuardRoom::onEnter(){
    for (int i = 0; i < guardCount; i++){
        guards[i]->reveal();
    }
}

//draws the room and then all the guards inside it.
void GuardRoom::draw(sf::RenderWindow& window){
    Room::draw(window);     //first the normal room (picture, fire, door)

    //defeated guards first, so the living ones are drawn on top of them.
    for (int i = 0; i < guardCount; i++){
        if (guards[i]->isDefeated() == true){
            guards[i]->draw(window);
        }
    }
    for (int i = 0; i < guardCount; i++){
        if (guards[i]->isDefeated() == false){
            guards[i]->draw(window);
        }
    }
}

//returns true only if every guard in the room is defeated
bool GuardRoom::allGuardsDefeated() const{
    for (int i = 0; i < guardCount; i++){
        if (guards[i]->isDefeated() == false){
            return false;//found a living guard, so the room is not cleared
        }
    }
    return true;
}

//returns how many guards are in this room
int GuardRoom::getGuardCount() const{
    return guardCount;
}

//returns the guard at the given index
Guard* GuardRoom::getGuard(int index){
    return guards[index];
}

//checks if the crew member's feet box touches any living guard.
//returns that guard if found, otherwise returns nullptr.
Guard* GuardRoom::getTouchedGuard(const sf::FloatRect& feetBox){
    for (int i = 0; i < guardCount; i++){
        //only check living guards
        if (guards[i]->isDefeated() == false && guards[i]->getTouchBox().intersects(feetBox) == true){
            return guards[i];
        }
    }
    return nullptr;
}

//runs a fight between a crew member and a guard.
//uses our overloaded operators to compare strength.
int GuardRoom::fight(CrewMember* crew, Guard* guard){
    //'*crew' means "the crew member itself", so *crew > *guard uses our overloaded operator>.
    if (*crew > *guard){
        guard->defeat();//crew won
        crew->loseUsage();//winning costs one usage point
        cleared = allGuardsDefeated();//maybe that was the last guard
        return FIGHT_WIN;
    }
    if (*crew == *guard){
        crew->markLost();//tie means the crew member is lost
        guard->weaken();//but the guard gets weaker for next time
        return FIGHT_TIE;
    }
    crew->markLost();//crew lost
    return FIGHT_LOSS;
}