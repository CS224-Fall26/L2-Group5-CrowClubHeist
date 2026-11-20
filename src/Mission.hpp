//one run of the game. It

//   - uses the 6 crew members          (AGGREGATION: Game owns them, Mission only points to them)
//   - owns the mansion it happens in   (COMPOSITION: deleted together with the mission)
//   - targets one artifact             (ASSOCIATION)
// It also keeps the progress: status, failure reason, the active crew member and the current room.

//UML attributes: status, failureReason

#ifndef MISSION_HPP
#define MISSION_HPP
#include "CrewMember.hpp"
#include "Mansion.hpp"
#include <string>

class Mission{
    private:
        CrewMember* crew[MAX_CREW_MEMBERS];
        int crewCount;//how many crew members are really in the array
        Artifact* target;
        Mansion* mansion;
        int activeIndex;// which crew member is walking around
        Room* currentRoom;// the room the crew is in (moves along the floor's linked list)
        std::string status;// "In progress", "Won" or "Lost"
        std::string failureReason;// why the mission was lost (empty while not lost)

    public:
        Mission(CrewMember* newCrew[], int newCrewCount, Artifact* newTarget, RoomInfo roomInfos[], int roomInfoCount,
                Artifact* artifacts[], int artifactCount, int starterIndex);
        ~Mission();

        Artifact* getTarget();
        Mansion* getMansion();
        int getFloorNumber() const;
        Room* getCurrentRoom();
        bool goToNextRoom();//false if we are already in the vault
        int getRoomsCleared(); //cleared rooms on the target floor

        //progress of the mission.
        void setWon();
        void setLost(std::string reason);
        bool isWon() const;
        bool isLost() const;
        std::string getStatus() const;
        std::string getFailureReason() const;

        int getCrewCount() const;
        CrewMember* getCrewMember(int index);
        CrewMember* getActive();
        int getActiveIndex() const;
        bool selectCrew(int index);  //only works if that member can act
        bool switchToNextCrew();//the next member who can act (false if there is none)
        bool makeSureActiveCanAct();//switches if needed (false = nobody can act: mission failed)
};

#endif
