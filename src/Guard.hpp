//aGuard is a Character that stands still in a room. If a crew member walks into it, a fight starts.
// Its strength is shown above its head once the crew has entered the room.

// INHERITANCE: "class Guard : public Character" means Guard gets everything Character has
// (name, strength, position, draw...) and adds its own things on top.

#ifndef GUARD_HPP
#define GUARD_HPP
#include "Character.hpp"

class Guard : public Character{
    private:
        bool defeated;
        bool revealed;//true after the crew entered the room (strength becomes visible)
        sf::Texture defeatedTexture;//picture of the guard lying down
        sf::Sprite defeatedSprite;

    public:
        Guard(int newStrength);

        void weaken();//strength is halved (happens when a crew member ties with it)
        void defeat();
        bool isDefeated() const;
        void reveal();
        bool isRevealed() const;

        //the area around the guard that starts a fight when the crew walks into it
        sf::FloatRect getTouchBox() const;


        //this REPLACES Character::draw (that is called overriding): a defeated guard is drawn lying down
        ////virtual makes sure the game uses the Guard's draw function instead of the Character's
        virtual void draw(sf::RenderWindow& window); 
};

#endif
