# The Crow Club Heist (C++ / SFML 2.6)

## Folder layout
```
CrowClubHeist/
  src/            all .hpp and .cpp files
  data/rooms.txt  walls, doors and guard spots of every room (read by the game)
  assets/         the pictures and the font
  CMakeLists.txt
```

## Build and run (macOS)
```
brew install cmake sfml@2
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix sfml@2)"
cmake --build build
./build/CrowClubHeist
```
Run it from the project folder (the one that contains `assets` and `data`).
Linux: `sudo apt install libsfml-dev cmake g++`, then the same cmake commands without the PREFIX part.

## Controls
- Menus: Up/Down/Left/Right, Enter, Esc.
- In a room: arrows or WASD to walk, TAB or 1-6 to switch crew, E to take the artifact, Enter to close the fight box.

## Rules in short
Walk into a guard to fight. C = crew strength, G = guard strength.
- C > G: guard defeated, crew loses 1 usage.
- C = G: crew member lost, guard strength halved.
- C < G: crew member lost, guard unchanged.

Defeat all guards to unlock the door. In the vault, stand next to the artifact and press E.
You lose when nobody with usage left can act.

## Features (same list as the proposal)
| # | Feature | Classes (from the proposal) | Member | Status |
|---|---|---|---|---|
| 1 | Welcome menu and screen navigation | Game, Screen, MenuScreen | Zunaira | done (menu: New Mission, View Crew, Kaz's Intel, How to Play, Exit) |
| 2 | Choose a crew member | CrewSelectScreen, CrewMember, Character | Ayna | done |
| 3 | Kaz's intel and artifact choice | IntelScreen, Artifact, Mission | Ayna | done |
| 4 | Random mansion generation | Mansion, Floor (linked list of rooms), Room, GuardRoom, Guard | Zunaira | done |
| 5 | Top-down movement and doors | PlayScreen, Character (move, position) | Zunaira | done |
| 6 | Guard fights | CrewMember (>, ==, loseUsage), Guard (weaken), GuardRoom (runs the fight) | Ayna | done |
| 7 | Timed puzzles | Puzzle (abstract), Riddle, MissingLetter, Multiplication, PuzzleFactory, PuzzleRoom | Ayna (puzzles), Zunaira (PuzzleRoom) | **not yet.** PuzzleRoom exists (vault + artifact) but has no puzzle or timer |
| 8 | Dialogue | Dialogue, DialogueLine (linked list), Character::speak | Zunaira | **not yet** |
| 9 | Win, lose and results | Mission (isWon, isLost), ResultScreen | Ayna | done |
| 10 | Sound | AudioManager | Zunaira | **not yet** |
| 11 | Play again and final testing | all classes | both | the menu lets you start a new mission again; full testing later |

Other things not in the game yet: climbing from floor 1 up to the target floor (the crew starts on the target floor).

Helper classes that are not in the proposal list: `RoomInfo` (reads data/rooms.txt) and `UI` (draws text and boxes), plus `HowToPlayScreen` (screen 2 of the screen table).

## UML check
| UML class | Attributes in the UML | In the code |
|---|---|
CrewMember | name, description, strength, usage, puzzleTime | all present (name and strength come from Character) |
Mission | status, failureReason | `status`, `failureReason`, `isWon()`, `isLost()` |
Mansion | name | `name` |
Artifact | name, floorNumber | `name`, `floorNumber` |
Floor | floorNumber | `floorNumber` |
Room | roomNumber, isCleared | `roomNumber`, `cleared` with `isCleared()` |
Guard | name, strength | `name` (from Character), `strength` |
Puzzle | question, answer, timeLimit | not yet (feature 7) |


| UML relationship | Where it is in the code |
|---|---|
| Mission uses 6 CrewMember (aggregation) | `Mission::crew` (pointers; the crew is owned and deleted by Game) |
| Mission takes place in 1 Mansion (composition) | `Mission::mansion` (created with `new`, deleted in `~Mission`) |
| Mission targets 1 Artifact (association) | `Mission::target` |
| Mansion contains 3 Artifacts (aggregation) | `Mansion::artifacts` (pointers; not deleted by the mansion) |
| Mansion has 3 Floors (composition) | `Mansion::floors` (deleted in `~Mansion`) |
| Floor has 3 Rooms (composition) | `Floor::head`, a linked list through `Room::nextRoom` (deleted in `~Floor`) |
| Room guarded by 1..2 Guards (composition) | `GuardRoom::guards` (deleted in `~GuardRoom`) |
| Room holds 0..1 Artifact (association) | `PuzzleRoom::artifact` (only the vault holds one) |
| Room locked by 0..1 Puzzle (composition) | not yet (feature 7) |

## Classes
| Class | Job |
|---|---|
| Game | window, main loop, owns crew, artifacts, mission, current screen |
| Screen (abstract) + MenuScreen, HowToPlayScreen, IntelScreen, CrewSelectScreen, PlayScreen, ResultScreen | the screens |
| Character -> CrewMember, Guard | people in the rooms |
| Artifact | the treasures |
| Room -> GuardRoom -> PuzzleRoom | the rooms (PuzzleRoom = vault) |
| Floor, Mansion | 3 rooms per floor (linked list), 3 floors |
| Mission | one run: crew, mansion, target, status, active crew member, current room |
| RoomInfo | facts about a room, read from data/rooms.txt |
| UI | helper functions that draw text and boxes |

## OOP map
- **Inheritance + virtual:** `Character` <- `CrewMember`, `Guard` (`draw` is overridden in `Guard`); `Room` <- `GuardRoom` <- `PuzzleRoom`; `Screen` <- 6 screens.
- **Abstract class:** `Screen` has pure virtual functions (`= 0`).
- **Polymorphism:** `Game` calls `Screen::handleEvent / update / draw` through one `Screen*`. `PlayScreen` calls `Room::getTouchedGuard / fight / draw` through a `Room*` without knowing the room type.
- **Operator overloading:** `CrewMember::operator>` and `operator==` compare strength with a `Guard` (used in `GuardRoom::fight`).
- **Linked list:** `Floor` keeps the first room, each `Room` points to the next one. `Mission` moves along it with `goToNextRoom()`.
- **Memory:** plain pointers with `new` and `delete`. Each class that creates objects deletes them in its destructor (Mission -> Mansion -> Floor -> Room -> Guard).

## Work split
- Zunaira: Game + Screen + MenuScreen, Mansion / Floor / Room / RoomInfo, PlayScreen walking (`moveActive`) and `Character::move`, PuzzleRoom.
- Ayna: CrewMember + CrewSelectScreen, Artifact / Mission + IntelScreen, Guard + `GuardRoom::fight`, ResultScreen.

More: `LEARNING_RESOURCES.md` (where to learn SFML and each C++ idea) and `CREDITS.md`.
