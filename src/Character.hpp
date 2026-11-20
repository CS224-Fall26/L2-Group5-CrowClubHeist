//Character is the BASE CLASS for everything that stands in a room and has a picture:
//the crew members (CrewMember) and the guards (Guard) both inherit from it.
//It stores what they have in common: name, strength, position and the sprite sheet.

//The sprite sheet is one picture with 3 columns x 4 rows of small frames (12 x 16 pixels):
//   columns: standing, step 1, step 2        rows: down, left, right, up

//LEARN (SFML sprites and textures): https://www.sfml-dev.org/tutorials/2.6/graphics-sprite.php
#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <SFML/Graphics.hpp>
#include <string>

//numbers for the 4 directions, so we can write DIR_LEFT instead of 1.
const int DIR_DOWN = 0; //the first row of the sprite sheet is facing down
const int DIR_LEFT = 1;//the second row of the sprite sheet is facing left
const int DIR_RIGHT = 2;
const int DIR_UP = 3;

    class Character{
    protected:
        //protected= the child classes (CrewMember, Guard) can use these
        std::string name;
        int strength;
        float x;        //position inside the room (in pixels of the room picture)
        float y;
        float speed;              //pixels per second
        int direction;            //one of DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_UP
        float animationTimer;     //counts seconds while walking, used to pick the walking frame
        bool isMoving; //whether the character is moving or standing still (used to pick the walking frame)

        //the sprite sheet picture
        sf::Texture texture;      
        sf::Sprite sprite;//what we actually draw

    public:
        static const int FRAME_WIDTH = 12;
        static const int FRAME_HEIGHT = 16;



        Character(std::string newName, int newStrength, std::string sheetName);

        //'virtual' lets a child class replace this function (Guard does that for draw)
        virtual ~Character();

        std::string getName() const;
        int getStrength() const;
        float getX() const;
        float getY() const;
        void setPosition(float newX, float newY);
        void setDirection(int newDirection);

        //moves the character by dx and dy pixels. (It does NOT check walls: PlayScreen asks the room first
        void move(float dx, float dy);

        //The small box at the character's feet. Walls are checked against this box,
        //so the head can overlap furniture and it looks like a top-down game.
        sf::FloatRect getFeetBox() const;
        //the same box, but as if the character stood at another position (used to test a move before making it).
        sf::FloatRect getFeetBoxAt(float atX, float atY) const;
        float getSpeed() const;
        float getFeetCenterX() const;
        float getFeetCenterY() const;

        //turns the character to face the way it is moving.
        void faceTowards(float dx, float dy);
        //moves the walking animation forward.
        void updateAnimation(float dt, bool moving);

        //draws the character at its position inside the room.
        virtual void draw(sf::RenderWindow& window);
        //draws one chosen frame anywhere on the screen (used by menus and the HUD).
        void drawAt(sf::RenderWindow& window, float screenX, float screenY, float scale, int row, int column);
};

#endif


//drawAt() is used by the menu and the HUD to draw the character at any position on the screen, with any scale, and any frame from the sprite sheet.
//draw() is used by the PlayScreen to draw the character at its position inside the room, with scale = 1.0f, and the correct frame from the sprite sheet (standing or walking, facing the right direction)

//draw with the animation and DRAWAT x/y coordinates of the screen