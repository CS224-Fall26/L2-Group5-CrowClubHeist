// CrewMember.hpp
//one of the six thieves. A CrewMember is a Character (inheritance) that also keeps
// usage (how many actions are left), puzzle time and whether it was lost.

// OPERATOR OVERLOADING: we make rules for what ">" and "==" mean between a CrewMember and a Guard,
//so in the fight code we can simply write:  if (crew > guard)

#ifndef CREWMEMBER_HPP
#define CREWMEMBER_HPP
#include "Character.hpp"

//how many crew members the game has
const int MAX_CREW_MEMBERS = 6;

class Guard;//we only need the name here; the full class is included in the .cpp file

class CrewMember:public Character{
    private:
        //seconds the member gets for a puzzle (used by the puzzle feature later)
        int puzzleTime;   

        //actions left (2 to 5), goes down after every fight won AND ARE RANDOMLY ROLLED AT THE START OF EVERY MISSION. 
        int usage;  

        bool lost;          

        std::string description; //who this person is (shown on the crew select screen)
        std::string quote;       //a short line they say (shown on the crew select screen)

    public:
        CrewMember(std::string newName, int newStrength, int newPuzzleTime, std::string newDescription,
                std::string newQuote);

        //getters
        int getUsage() const;
        int getPuzzleTime() const;
        std::string getDescription() const;
        std::string getQuote() const;

        void rollUsage();        //picks a random usage from 2 to 5 (called at the start of every mission)
        void loseUsage();        //usage goes down by 1
        bool isExhausted() const;//usage is 0
        bool isLost() const;
        void markLost();
        bool canAct() const;//not lost and still has usage left

        bool operator>(const Guard& guard) const;//my strength is bigger than the guard's
        bool operator==(const Guard& guard) const;//my strength is equal to the guard's
};

#endif
