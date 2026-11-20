#include "PlayScreen.hpp"
#include "Game.hpp"
#include "UI.hpp"

//The panel on the left side of the window is this wide.
//this is a constant, so it never changes. We subtract it when calculating the room size.
const float HUD_WIDTH = 270.0f;

//sets up the play screen when the mission starts. 
//sets all the starting values (no fights yet, timer at 0, zoom at 1) and puts the player in the first room.
PlayScreen::PlayScreen(Game* newGame) : Screen(newGame){
    mission = game->getMission();
    state = STATE_WALKING;
    toastTimer = 0.0f;
    scale = 1.0f;
    originX = 0.0f;
    originY = 0.0f;
    fightResult = FIGHT_WIN;
    fightCrewStrength = 0;
    fightGuardStrength = 0;
    fightLineCount = 0;
    enterRoom(); //moves the player into the starting room
}

//shows a small message at the top of the screen (like "Guards still block the door!").
//the message stays visible for 2.2 seconds.
void PlayScreen::showToast(std::string text){
    toastText = text;
    toastTimer = 2.2f;
}

//figures out how big the room should be drawn on the screen.
//it zooms in as much as possible so the room fills the space on the right side (leaving the left panel alone).
//it only uses steps of 0.5 (like 1.0, 1.5, 2.0) so the pixel art doesn't look blurry.
void PlayScreen::setupView(){
    Room* room = mission->getCurrentRoom();

    // The free space for the room is the window without the left panel.
    float freeWidth = WINDOW_WIDTH - HUD_WIDTH - 20.0f;
    float freeHeight = WINDOW_HEIGHT - 20.0f;

    // Zoom as much as fits, but only in steps of 0.5 (1.0, 1.5, 2.0 ...) so the pixel art stays sharp.
    float zoomX = freeWidth / room->getWidth();
    float zoomY = freeHeight / room->getHeight();
    float zoom = zoomX;
    if (zoomY < zoomX) {
        zoom = zoomY;
    }
    int halfSteps = (int)(zoom * 2.0f);
    scale = halfSteps / 2.0f;
    if (scale < 1.0f) {
        scale = 1.0f;
    }

    // Put the room in the middle of the free space.
    // originX and originY are the exact top-left corner of where the room should be drawn on the window.
    originX = (float)(int)(HUD_WIDTH + 10.0f + (freeWidth - room->getWidth() * scale) / 2.0f);
    originY = (float)(int)((WINDOW_HEIGHT - room->getHeight() * scale) / 2.0f);
}

//called every time the player walks through a door into a new room.
void PlayScreen::enterRoom() {
    Room* room = mission->getCurrentRoom();
    CrewMember* member = mission->getActive();

    room->onEnter();                                      // guards show their strength
    member->setPosition(room->getSpawnX(), room->getSpawnY()); // put the crew member at the door
    member->setDirection(DIR_DOWN);                       // make them face down
    setupView();                                          // recalculate the zoom for the new room
    
    // In the first room we also say the name of the mansion.
    // this shows a small message at the top of the screen so the player knows where they are.
    std::string message = "Floor " + std::to_string(mission->getFloorNumber()) + " - " + room->getName();
    if (room->getRoomNumber() == 1){
        message = mission->getMansion()->getName() + " - " + message;
    }
    showToast(message);
}

//handles all the key presses while the player is walking around or fighting.
void PlayScreen::handleEvent(const sf::Event& event){
    if (event.type != sf::Event::KeyPressed){ // ignore everything except key presses
        return;
    }

    // While the fight box is open, only Enter does something (it closes the box).
    // so if we are fighting, we stop right here and wait for the player to press Enter.
    if (state == STATE_FIGHT){
        if (event.key.code == sf::Keyboard::Enter){
            closeFight();
        }
        return;
    }

    CrewMember* member = mission->getActive(); // get the character the player is currently controlling
    Room* room = mission->getCurrentRoom();

    if (event.key.code == sf::Keyboard::Escape){
        game->changeScreen(SCREEN_MENU); // go back to the main menu
    }
    else if (event.key.code == sf::Keyboard::Tab) {
        // Change to the next crew member who can still act. He or she stands where the old one stood.
        float oldX = member->getX();
        float oldY = member->getY();
        if (mission->switchToNextCrew() == true) {
            mission->getActive()->setPosition(oldX, oldY); // teleport the new member to the old spot
            showToast(mission->getActive()->getName() + " takes over");
        }
        else {
            showToast("Nobody else can act"); // all other crew members are lost or exhausted
        }
    }
    else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num6) {
        // Keys 1 to 6 choose a crew member directly.
        int index = event.key.code - sf::Keyboard::Num1; // convert the number key to a 0-based index
        float oldX = member->getX();
        float oldY = member->getY();
        if (mission->selectCrew(index) == true) {
            mission->getActive()->setPosition(oldX, oldY); // teleport the new member to the old spot
            showToast(mission->getActive()->getName() + " takes over");
        }
        else {
            showToast("That crew member can't act"); // this specific member is lost or exhausted
        }
    }
    else if (event.key.code == sf::Keyboard::E) {
        // E only does something in the vault, next to the artifact.
        // If the player presses E somewhere else, nothing happens.
        if (room->isVault() == true) {
            if (room->isNearArtifact(member->getFeetCenterX(), member->getFeetCenterY()) == false) {
                showToast("Walk up to the artifact, then press E"); // they are too far away
            }
            else if (room->isCleared() == false) {
                showToast("Defeat the guards first!"); // guards are still alive in the vault
            }
            else {
                mission->setWon(); // the player won the whole game
                game->changeScreen(SCREEN_WIN);
            }
        }
    }
}

//moves the active crew member based on which arrow keys are currently held down.
void PlayScreen::moveActive(float dt) {
    // Read the keys. Each pressed key adds or removes 1 from dx (sideways) or dy (up/down).
    // dx = 0 means not moving left or right. dy = 0 means not moving up or down.
    float dx = 0.0f;
    float dy = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        dx = dx - 1.0f; // moving left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx = dx + 1.0f; // moving right
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        dy = dy - 1.0f; // moving up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        dy = dy + 1.0f; // moving down
    }

    CrewMember* member = mission->getActive();
    Room* room = mission->getCurrentRoom();

    // if dx and dy are both 0, the player is standing still
    bool moving = (dx != 0.0f || dy != 0.0f);
    member->updateAnimation(dt, moving); // make the walking animation play if moving
    if (moving == false){
        return; // stop here if the player is not pressing any keys
    }
    member->faceTowards(dx, dy); // make the character face the direction they are walking

    // Walking diagonally would be faster, so we shorten the step a bit.
    // 0.7071 is roughly the square root of 0.5. This keeps diagonal speed the same as straight speed.
    if (dx != 0.0f && dy != 0.0f){
        dx = dx * 0.7071f;
        dy = dy * 0.7071f;
    }

    //move on the x axis first. We only really move if the new place is not blocked.
    // we check the feet box to see if the new position hits a wall.
    float stepX = dx * member->getSpeed() * dt;
    if (room->isBlocked(member->getFeetBoxAt(member->getX() + stepX, member->getY())) == false){
        member->move(stepX, 0.0f);
    }
    //then the y axis. Doing them one after the other lets the character slide along walls.
    // this is called "separating the axes". It prevents the character from getting stuck on corners.
    float stepY = dy * member->getSpeed() * dt;
    if (room->isBlocked(member->getFeetBoxAt(member->getX(), member->getY() + stepY)) == false){
        member->move(0.0f, stepY);
    }
}

//called every single frame. updates the game logic.
void PlayScreen::update(float dt){
    Room* room = mission->getCurrentRoom();
    room->update(dt); // update the room (move fires, animate puzzles, etc)
    if (toastTimer > 0.0f){
        toastTimer = toastTimer - dt; // count down the message timer
    }
    if (state != STATE_WALKING){ // if we are in a fight, don't move the player
        return;
    }

    moveActive(dt); // move the player based on keyboard input
    CrewMember* member = mission->getActive();

    // Did the crew member walk into a guard? Then a fight starts.
    Guard* guard = room->getTouchedGuard(member->getFeetBox());
    if (guard != nullptr){
        startFight(guard); // starts the fight and opens the fight box
        return;
    }

    // Did the crew member walk into the door?
    // if they did, we check if the room is cleared. If yes, go to the next room.
    if (room->isAtDoor(member->getFeetBox()) == true){
        if (room->isCleared() == true){
            if (mission->goToNextRoom() == true){
                enterRoom(); // moved to the next room successfully
            }
        }
        else if (toastTimer <= 0.0f) { // only show the message if we are not already showing one
            showToast("Guards still block the door!");
        }
    }
}

//sets up a fight between the active crew member and a guard.
void PlayScreen::startFight(Guard* guard){

    Room* room = mission->getCurrentRoom();
    CrewMember* member = mission->getActive();

    // Remember the numbers BEFORE the fight, because the fight changes them.
    // we save these so we can display the original strengths in the fight box.
    fightName = member->getName();
    fightCrewStrength = member->getStrength();
    fightGuardStrength = guard->getStrength();

    // The room runs the fight (the rules are in GuardRoom::fight).
    // this function will return FIGHT_WIN, FIGHT_TIE, or FIGHT_LOSS.
    fightResult = room->fight(member, guard);

    // Prepare the lines shown in the fight box.
    fightLineCount = 0; // clear any old text from the previous fight
    if (fightResult == FIGHT_WIN){
        fightLines[fightLineCount] = "Guard defeated!";
        fightLineCount++;
        fightLines[fightLineCount] = fightName + " loses 1 usage (" + std::to_string(member->getUsage()) + " left).";
        fightLineCount++;
        if (member->isExhausted() == true)  {
            fightLines[fightLineCount] = fightName + " is exhausted and can't act any more.";
            fightLineCount++;
        }
    }
    else if (fightResult == FIGHT_TIE)  {
        fightLines[fightLineCount] = "Equal strength! " + fightName + " is lost.";
        fightLineCount++;
        fightLines[fightLineCount] = "The guard is weakened to " + std::to_string(guard->getStrength()) + ".";
        fightLineCount++;
    }
    else {
        fightLines[fightLineCount] = fightName + " was overpowered and is lost.";
        fightLineCount++;
        fightLines[fightLineCount] = "The guard is unchanged.";
        fightLineCount++;
    }
    state = STATE_FIGHT; // change the state so the game stops moving and waits for the player
}

//called when the player presses Enter to close the fight box.
//checks if the crew member is lost or exhausted, and switches to the next one if needed.
void PlayScreen::closeFight(){
    Room* room = mission->getCurrentRoom();
    CrewMember* member = mission->getActive();
    state = STATE_WALKING; // go back to walking mode

    // if the result was not a win, the crew member was lost in the fight
    bool crewWasLost = (fightResult != FIGHT_WIN);
    float oldX = member->getX();
    float oldY = member->getY();

    // If the active member is lost or exhausted, the next one who can act takes over.
    // If nobody can act, the mission is over.
    if (mission->makeSureActiveCanAct() == false){
        mission->setLost("Crew lost: no crew member with usage left can continue.");
        game->changeScreen(SCREEN_LOSE); // go to the lose screen
        return;
    }

    CrewMember* newMember = mission->getActive();
    if (crewWasLost == true){
        // A lost member is gone, so the next one starts again at the door of the room.
        newMember->setPosition(room->getSpawnX(), room->getSpawnY());
    }
    else{
        newMember->setPosition(oldX, oldY); // the member won, so they stay where they are
    }
    newMember->updateAnimation(0.0f, false); // make them stand still

    // show a small message if the active member changed, or if the room is cleared
    if (newMember->getName() != fightName)  {
        showToast(newMember->getName() + " takes over");
    }
    else if (room->isCleared() == true && room->isVault() == false)  {
        showToast("Room cleared - the door is open!");
    }
}

//draws the strength of each living guard in a small red circle above their head.
void PlayScreen::drawGuardNumbers(sf::RenderWindow& window){
    // Draws the strength of each living guard in a red circle above its head.
    // We draw this with the normal window view, so we convert room position -> screen position.
    Room* room = mission->getCurrentRoom();
    for (int i = 0; i < room->getGuardCount(); i++)  {
        Guard* guard = room->getGuard(i);
        // skip if the guard is hidden or already defeated
        if (guard->isRevealed() == false || guard->isDefeated() == true) {
            continue;      // 'continue' skips to the next guard
        }
        
        // convert the guard's room coordinates to actual screen pixel coordinates
        float screenX = originX + (guard->getX() + 6.0f) * scale;
        float screenY = originY + (guard->getY() - 6.0f) * scale;

        // draw the red circle background
        sf::CircleShape badge(10.0f);
        badge.setOrigin(10.0f, 10.0f);
        badge.setPosition(screenX, screenY);
        badge.setFillColor(sf::Color(140, 30, 30));
        badge.setOutlineColor(COLOR_CREAM);
        badge.setOutlineThickness(2.0f);
        window.draw(badge);
        
        // draw the strength number on top of the circle
        drawText(window, std::to_string(guard->getStrength()), 14, screenX, screenY - 10.0f, sf::Color::White, ALIGN_CENTER);
    }
}

//draws the left panel showing the crew and their stats (HUD).
void PlayScreen::drawHud(sf::RenderWindow& window){
    Room* room = mission->getCurrentRoom();

    //draw the background panel for the HUD
    drawPanel(window, 10, 10, HUD_WIDTH - 20, WINDOW_HEIGHT - 20);
    
    //draw the floor and room number (e.g. "FLOOR 1 - ROOM 2/3")
    drawText(window, "FLOOR " + std::to_string(mission->getFloorNumber()) + " - ROOM " +
             std::to_string(room->getRoomNumber()) + "/3", 17, HUD_WIDTH / 2.0f, 24, COLOR_GOLD, ALIGN_CENTER);

    //draw the name of the room. if it's a vault, add "(vault)" to the end.
    std::string roomName = room->getName();
    if (room->isVault() == true){
        roomName = roomName + " (vault)";
    }
    drawText(window, roomName, 16, HUD_WIDTH / 2.0f, 54, COLOR_CREAM, ALIGN_CENTER);
    
    //draw the name of the artifact we are trying to steal
    drawText(window, "Target: " + mission->getTarget()->getName(), 16, HUD_WIDTH / 2.0f, 78, COLOR_CREAM, ALIGN_CENTER);

    //One row for each crew member.
    for (int i = 0; i < mission->getCrewCount(); i++){
        CrewMember* member = mission->getCrewMember(i);
        float y = 120.0f + i * 84.0f;
        bool isActive = (i == mission->getActiveIndex()); // is this the member we are controlling?

        // draw the background box for this crew member's row
        sf::RectangleShape row(sf::Vector2f(HUD_WIDTH - 50.0f, 76.0f));
        row.setPosition(25, y);
        if (isActive == true){
            row.setFillColor(sf::Color(110, 70, 36, 220)); // gold background for the active member
            row.setOutlineColor(COLOR_GOLD);
            row.setOutlineThickness(3.0f);
        }
        else {
            row.setFillColor(sf::Color(50, 36, 30, 200)); // dark background for the others
            row.setOutlineColor(sf::Color(90, 66, 50));
            row.setOutlineThickness(1.0f);
        }
        window.draw(row);

        //draw a tiny picture of the crew member inside the box
        member->drawAt(window, 32, y + 6, 4.0f, DIR_DOWN, 0);

        //Name colour: red-ish if lost, grey if exhausted, normal otherwise.
        sf::Color nameColor = COLOR_CREAM;
        std::string status = "ready";
        if (member->isLost() == true){
            nameColor = sf::Color(150, 90, 90);
            status = "LOST";
        }
        else if (member->isExhausted() == true){
            nameColor = sf::Color(150, 150, 150);
            status = "EXHAUSTED";
        }
        else if (isActive == true){
            status = "ACTIVE";
        }

        // draw the crew member's name, strength, and current status
        drawText(window, std::to_string(i + 1) + " " + member->getName(), 18, 90, y + 4, nameColor);
        drawText(window, "STR " + std::to_string(member->getStrength()), 14, 90, y + 28, nameColor);
        drawText(window, status, 13, 90, y + 52, nameColor);

        //One green dot for every usage the member has left.
        //draw one small green circle for each usage point.
        for (int dot = 0; dot < member->getUsage(); dot++){
            sf::CircleShape pip(4.5f);
            pip.setFillColor(COLOR_GREEN);
            pip.setPosition(172.0f + dot * 13.0f, y + 31.0f);
            window.draw(pip);
        }
    }

    //draw the control instructions at the bottom of the HUD
    drawText(window, "Move: arrows / WASD", 13, HUD_WIDTH / 2.0f, 632, COLOR_GOLD, ALIGN_CENTER);
    drawText(window, "TAB / 1-6: switch crew", 13, HUD_WIDTH / 2.0f, 652, COLOR_GOLD, ALIGN_CENTER);
    drawText(window, "E: take artifact   Esc: menu", 13, HUD_WIDTH / 2.0f, 672, COLOR_GOLD, ALIGN_CENTER);
}

//draws the big popup box that appears in the middle of the screen during a fight.
void PlayScreen::drawFightBox(sf::RenderWindow& window){
    float boxX = HUD_WIDTH + 120.0f;
    float boxY = 220.0f;
    float centerX = boxX + 380.0f;

    //draw the box background
    drawPanel(window, boxX, boxY, 760, 280, sf::Color(24, 16, 14, 245));
    drawText(window, "FIGHT!", 34, centerX, boxY + 16, COLOR_GOLD, ALIGN_CENTER);
    
    //show the crew member's strength (C) and the guard's strength (G)
    drawText(window, fightName + " (C = " + std::to_string(fightCrewStrength) + ")   vs   Guard (G = " +
            std::to_string(fightGuardStrength) + ")", 24, centerX, boxY + 70, COLOR_CREAM, ALIGN_CENTER);

    //show if the player won, tied, or lost based on the math
    if (fightResult == FIGHT_WIN){
        drawText(window, "C > G : WIN", 30, centerX, boxY + 112, COLOR_GREEN, ALIGN_CENTER);
    }
    else if (fightResult == FIGHT_TIE){
        drawText(window, "C = G : TIE", 30, centerX, boxY + 112, COLOR_GOLD, ALIGN_CENTER);
    }
    else{
        drawText(window, "C < G : LOSS", 30, centerX, boxY + 112, COLOR_RED, ALIGN_CENTER);
    }

    //print the explanation lines we prepared in startFight()
    for (int i = 0; i < fightLineCount; i++){
        drawText(window, fightLines[i], 20, centerX, boxY + 165.0f + i * 30.0f, COLOR_CREAM, ALIGN_CENTER);
    }
    drawText(window, "Press Enter to continue", 16, centerX, boxY + 246, COLOR_GOLD, ALIGN_CENTER);
}

//the main draw function for the play screen.
void PlayScreen::draw(sf::RenderWindow& window){
    Room* room = mission->getCurrentRoom();
    CrewMember* member = mission->getActive();

    //Draw the room and the characters through a View, so we can use room pixel positions.
    //setViewport says WHERE on the window the view appears (as fractions from 0 to 1).
    //this is how we zoom the room in without stretching the HUD.
    sf::View roomView(sf::FloatRect(0, 0, room->getWidth(), room->getHeight()));
    roomView.setViewport(sf::FloatRect(originX / WINDOW_WIDTH, originY / WINDOW_HEIGHT,
    room->getWidth() * scale / WINDOW_WIDTH,
    room->getHeight() * scale / WINDOW_HEIGHT));


    window.setView(roomView);
    room->draw(window); //draw the room floor, walls, doors
    member->draw(window); //draw the crew member standing in the room

    //Go back to the normal view for the HUD and texts.
    //if we didn't do this, the HUD would be zoomed in with the room!
    window.setView(window.getDefaultView());
    drawGuardNumbers(window);
    drawHud(window);

    //draw the small toast message at the top if the timer is still going
    if (toastTimer > 0.0f){
        float centerX = HUD_WIDTH + (WINDOW_WIDTH - HUD_WIDTH) / 2.0f;
        drawPanel(window, centerX - 260, 14, 520, 44, sf::Color(24, 16, 14, 230));
        drawText(window, toastText, 20, centerX, 22, COLOR_CREAM, ALIGN_CENTER);
    }
    
    //draw the fight box on top of everything else if a fight is happening
    if (state == STATE_FIGHT){
        drawFightBox(window);
    }
}