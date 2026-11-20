// CrewMember.cpp
#include "CrewMember.hpp"
#include "Guard.hpp"
#include <cstdlib>

CrewMember::CrewMember(std::string newName, int newStrength, int newPuzzleTime, std::string newDescription,std::string newQuote)
:Character(newName, newStrength, newName){
    puzzleTime = newPuzzleTime;
    description = newDescription;
    quote = newQuote;
    usage = 0;
    lost = false;
}
int CrewMember::getUsage()const{
    return usage;
}

int CrewMember::getPuzzleTime() const{
    return puzzleTime;
}

std::string CrewMember::getDescription() const{
    return description;
}

std::string CrewMember::getQuote() const{
    return quote;
}


//Its called ONCE at the start of every mission
void CrewMember::rollUsage(){
    //rand() % 4 gives 0,1,2 or 3. Adding 2 gives 2,3,4 or 5.
    //Sets usage to a random value in 2–5
    //resets lost = false
    usage = 2 + rand() % 4;
    lost = false;
}
//spend one usage after fight they won
//writes
void CrewMember::loseUsage(){
    if (usage > 0){
        usage = usage - 1;
    }
}
//usage used up BUT not caught by a guard 
//reads
bool CrewMember::isExhausted() const{
    return usage <= 0;
}


//is this thief captured?
bool CrewMember::isLost() const{
    return lost;
}
//this thief got captured
void CrewMember::markLost(){
    lost = true;
}

//when member not lost and has usage remaining
bool CrewMember::canAct() const{
    return lost == false && usage > 0;
}



bool CrewMember::operator>(const Guard& guard) const{
    return strength > guard.getStrength();
}

bool CrewMember::operator==(const Guard& guard) const{
    return strength == guard.getStrength();
}
