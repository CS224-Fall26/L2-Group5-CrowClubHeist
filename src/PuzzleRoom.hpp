// The vault: the last room of every floor. It is a GuardRoom (so it has guards) and it also
// holds an artifact on a pedestal. In the final version the artifact is protected by a timed
// puzzle. For now, the crew takes the artifact once all guards are defeated.

//INHERITANCE CHAIN: Room -> GuardRoom -> PuzzleRoom
//ASSOCIATION: the room only points to the Artifact. It does not own it (so no 'delete' here)

#ifndef PUZZLEROOM_HPP
#define PUZZLEROOM_HPP
#include "GuardRoom.hpp"

class PuzzleRoom : public GuardRoom{
    private:
        Artifact* artifact;

        //the box on the floor the artifact stands on (the crew cannot walk through it)
        sf::FloatRect getPedestalBox() const;

    public:
        PuzzleRoom(RoomInfo newInfo, int newRoomNumber, int floorNumber, Artifact* newArtifact);

        virtual void draw(sf::RenderWindow& window);
        virtual bool isVault() const;
        virtual bool isBlocked(const sf::FloatRect& feetBox) const;
        virtual bool isNearArtifact(float feetX, float feetY) const;

        Artifact* getArtifact();
};

#endif
