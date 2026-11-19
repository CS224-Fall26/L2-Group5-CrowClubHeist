# Where to Learn 3what This Project Uses

For code understanding, every `.hpp`/`.cpp` file also has a `LEARN` line in its top comment!!

## SFML(Graphics Library / GUI)

Official SFML 2.6 tutorials, in the order that matches our code:

- **Tutorial Index:** https://www.sfml-dev.org/tutorials/2.6/
- **Class Reference:** https://www.sfml-dev.org/documentation/2.6.1/

| Topic | Tutorial | Where Used |
| :--- | :--- | :--- |
| Opening a window, main loop | https://www.sfml-dev.org/tutorials/2.6/window-window.php | `Game::Game`, `Game::run` |
| Events (key pressed, window closed) | https://www.sfml-dev.org/tutorials/2.6/window-events.php | `Game::run`, `Screen::handleEvent` |
| Keyboard input (holding a key) | https://www.sfml-dev.org/tutorials/2.6/window-inputs.php | `PlayScreen::moveActive` |
| Drawing 2D objects (`clear` / `draw` / `display`) | https://www.sfml-dev.org/tutorials/2.6/graphics-draw.php | `Game::run`, all `draw` methods |
| Sprites and textures | https://www.sfml-dev.org/tutorials/2.6/graphics-sprite.php | `Character`, `Room`, `Artifact` |
| Text and fonts | https://www.sfml-dev.org/tutorials/2.6/graphics-text.php | `drawText` (`UI.cpp`) |
| Shapes (rectangles, circles) | https://www.sfml-dev.org/tutorials/2.6/graphics-shape.php | `drawPanel`, door, pedestal, badges |
| Views (room camera/zoom) | https://www.sfml-dev.org/tutorials/2.6/graphics-view.php | `PlayScreen::draw` |
| Time (clock and delta time) | https://www.sfml-dev.org/tutorials/2.6/system-time.php | `Game::run` |

## C++ Concepts Used

| Concept | Where Used |
| :--- | :--- |
| Classes, constructors, getters, access specifiers | Every class |
| Inheritance (`class Guard : public Character`) | `Character` -> `CrewMember` / `Guard`, `Room` -> `GuardRoom` -> `PuzzleRoom`, `Screen` -> 6 screens |
| Virtual functions & overriding | `Room::draw`, `Character::draw`, `Room::isDoorOpen`, etc. |
| Abstract classes & pure virtual methods (`= 0`) | `Screen` |
| Polymorphism (`Screen*` / `Room*` dispatch) | `Game::run`, `PlayScreen` |
| Operator overloading (`operator>`, `operator==`) | `CrewMember` (used in `GuardRoom::fight`) |
| Raw pointers, `new`/`delete`, destructors | `GuardRoom`, `Floor`, `Mansion`, `Mission`, `Game` |
| `std::vector` | Guards, floors, crew, artifacts |
| Linked list | `Floor` -> `Room::nextRoom`; `Mission::goToNextRoom` |
| `rand()` and `srand()` | `GuardRoom`, `Mansion`, `CrewMember::rollUsage` |
| File I/O (`std::ifstream`) | `loadRoomInfos` (`RoomInfo.cpp`) |

## OOP Relationships

- **Inheritance:** Child class "is a" parent (`Guard` is a `Character`).
- **Composition (solid diamond):** Owner creates and destroys parts (`Mansion` -> `Floor` -> `Room` -> `Guard`).
- **Aggregation (hollow diamond):** Owner references independent objects (`Mission` uses the 6 `CrewMember` instances).
- **Association (arrow):** Class references/interacts with another (`Mission` -> `Artifact`).


