/*
        Program 2: Wumpus, version 2 [Dynamic sized arrays, bats, and arrows]
        CS 211, UIC, Fall 2022
        System: Replit
        Author: Aniket Jasani

        Hunt the Wumpus is a classic text-based adventure game by Gregory Yob
   from 1972. The Wumpus lives in a completely dark cave of 20 rooms.  Each room
   has 3 tunnels leading to other rooms. This program implements this game with
   superbats, arrows, and dynamic sized arrays.
*/

#include <ctype.h>   // for toupper()
#include <stdbool.h> // for the bool type in C
#include <stdio.h>
#include <stdlib.h> // for srand

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
  int moveNumber; // Counts up from 1, incrementing for each move
  int playerRoom; // Room 1..20 the player currently is in
  int wumpusRoom; // Room 1..20 the Wumpus is in
  int pitRoom1;   // Room 1..20 the first pit is in
  int pitRoom2;   // Room 1..20 the second pit is in
  int arrow;      // Room 1..20 the arrow is in
  int batRoom1;   // Room 1..20 with first pack of super-bats
  int batRoom2;   // Room 1..20 with second pack of super-bats
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *playerIsAlive,
                     int **pRooms);

//--------------------------------------------------------------------------------
void displayCave() {
  printf("\n"
         "       ______18______             \n"
         "      /      |       \\           \n"
         "     /      _9__      \\          \n"
         "    /      /    \\      \\        \n"
         "   /      /      \\      \\       \n"
         "  17     8        10     19       \n"
         "  | \\   / \\      /  \\   / |    \n"
         "  |  \\ /   \\    /    \\ /  |    \n"
         "  |   7     1---2     11  |       \n"
         "  |   |    /     \\    |   |      \n"
         "  |   6----5     3---12   |       \n"
         "  |   |     \\   /     |   |      \n"
         "  |   \\       4      /    |      \n"
         "  |    \\      |     /     |      \n"
         "  \\     15---14---13     /       \n"
         "   \\   /            \\   /       \n"
         "    \\ /              \\ /        \n"
         "    16---------------20           \n"
         "\n");
}

//--------------------------------------------------------------------------------
void displayInstructions() {
  printf("Hunt the Wumpus:                                             \n"
         "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
         "room has 3 tunnels leading to other rooms.                   \n"
         "                                                             \n"
         "Hazards:                                                     \n"
         "1. Two rooms have bottomless pits in them.  If you go there you fall "
         "and die.   \n"
         "2. Two other rooms have super-bats.  If you go there, the bats grab "
         "you and     \n"
         "   fly you to some random room, which could be troublesome.  Then "
         "those bats go \n"
         "   to a new room different from where they came from and from the "
         "other bats.   \n"
         "3. The Wumpus is not bothered by the pits or bats, as he has sucker "
         "feet and    \n"
         "   is too heavy for bats to lift.  Usually he is asleep.  Two things "
         "wake       \n"
         "    him up: Anytime you shoot an arrow, or you entering his room.  "
         "The Wumpus   \n"
         "    will move into the lowest-numbered adjacent room anytime you "
         "shoot an arrow.\n"
         "    When you move into the Wumpus' room, then he wakes and moves if "
         "he is in an \n"
         "    odd-numbered room, but stays still otherwise.  After that, if he "
         "is in your \n"
         "    room, he snaps your neck and you die!                            "
         "           \n"
         "                                                                     "
         "           \n"
         "Moves:                                                               "
         "           \n"
         "On each move you can do the following, where input can be upper or "
         "lower-case:  \n"
         "1. Move into an adjacent room.  To move enter 'M' followed by a "
         "space and       \n"
         "   then a room number.                                               "
         "           \n"
         "2. Shoot your guided arrow through a list of up to three adjacent "
         "rooms, which  \n"
         "   you specify.  Your arrow ends up in the final room.               "
         "           \n"
         "   To shoot enter 'S' followed by the number of rooms (1..3), and "
         "then the      \n"
         "   list of the desired number (up to 3) of adjacent room numbers, "
         "separated     \n"
         "   by spaces. If an arrow can't go a direction because there is no "
         "connecting   \n"
         "   tunnel, it ricochets and moves to the lowest-numbered adjacent "
         "room and      \n"
         "   continues doing this until it has traveled the designated number "
         "of rooms.   \n"
         "   If the arrow hits the Wumpus, you win! If the arrow hits you, you "
         "lose. You  \n"
         "   automatically pick up the arrow if you go through a room with the "
         "arrow in   \n"
         "   it.                                                               "
         "           \n"
         "3. Enter 'R' to reset the person and hazard locations, useful for "
         "testing.      \n"
         "4. Enter 'C' to cheat and display current board positions.           "
         "           \n"
         "5. Enter 'D' to display this set of instructions.                    "
         "           \n"
         "6. Enter 'P' to print the maze room layout.                          "
         "           \n"
         "7. Enter 'X' to exit the game.                                       "
         "           \n"
         "                                                                     "
         "           \n"
         "Good luck!                                                           "
         "           \n"
         " \n\n");
} // end displayInstructions()

//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,     // New number we're checking
                 int randomNumbers[], // Set of numbers previously found
                 int limit)           // How many numbers previously found
{
  int returnValue = false;

  // compare random value against all previously found values
  for (int i = 0; i < limit; i++) {
    if (randomValue == randomNumbers[i]) {
      returnValue = true; // It is already there
      break;
    }
  }

  return returnValue;
}

//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[], // Array of random numbers
                     int size)            // Size of random numbers array
{
  int randomValue = -1;

  for (int i = 0; i < size; i++) {
    if (randomNumbers[i] != 0) {
      continue;
    } else {
      do {
        randomValue = rand() % NUMBER_OF_ROOMS + 1; // random number 1..20
      } while (alreadyFound(randomValue, randomNumbers, i));
      randomNumbers[i] = randomValue;
    }
  }
}

//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo) // All game settings variables
{
  // Array of 7 unique values 1..20, to be used in initializing cave hazards
  // locations
  int *randomNumbers = malloc(7 * sizeof(int));

  // Set each element of the array to 0
  for (int i = 0; i < 7; i++) {
    randomNumbers[i] = 0;
  }

  // Initialize cave room connections
  //       ______18______
  //      /      |       \
    //     /      _9__      \
    //    /      /    \      \
    //   /      /      \      \
    //  17     8        10    19
  // |  \   / \      /  \   / |
  // |   \ /   \    /    \ /  |
  // |    7     1---2     11  |
  // |    |    /     \    |   |
  // |    6----5     3---12   |
  // |    |     \   /     |   |
  // |    \       4      /    |
  // |     \      |     /     |
  //  \     15---14---13     /
  //   \   /            \   /
  //    \ /              \ /
  //    16---------------20

  // Select some unique random values 1..20 to be used for 2 bats rooms, 2
  // pits rooms, Wumpus room, arrow room, and initial player room
  setUniqueValues(randomNumbers, 7);
  // Use the unique random numbers to set initial locations of hazards, which
  //    should be non-overlapping.
  gameInfo->playerRoom = randomNumbers[0];
  gameInfo->wumpusRoom = randomNumbers[1];
  gameInfo->pitRoom1 = randomNumbers[2];
  gameInfo->pitRoom2 = randomNumbers[3];
  gameInfo->batRoom1 = randomNumbers[4];
  gameInfo->batRoom2 = randomNumbers[5];
  gameInfo->arrow = randomNumbers[6];

  gameInfo->moveNumber = 1;
} // end initializeBoard(...)

//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent(int tunnels[3], // Array of adjacent tunnels
                   int nextRoom)   // Desired room to move to
{
  return (tunnels[0] == nextRoom || tunnels[1] == nextRoom ||
          tunnels[2] == nextRoom);
}

//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo) {
  printf("Cheating! Game elements are in the following rooms: \n"
         "Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
         "%4d %5d %6d %5d %5d %5d %5d \n\n",
         gameInfo.playerRoom, gameInfo.wumpusRoom, gameInfo.pitRoom1,
         gameInfo.pitRoom2, gameInfo.batRoom1, gameInfo.batRoom2,
         gameInfo.arrow);
} // end displayCheatInfo(...)

//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo(struct GameInfo gameInfo, // All game setting variables
                     int **pRooms)             // Cave rooms
{
  // Retrieve player's current room number and display it
  int currentRoom = gameInfo.playerRoom;
  printf("You are in room %d. ", currentRoom);

  // Retrieve index values of all 3 adjacent rooms
  int room1 = pRooms[currentRoom][0];
  int room2 = pRooms[currentRoom][1];
  int room3 = pRooms[currentRoom][2];

  // Display hazard detection message if Wumpus is in an adjacent room
  int wumpusRoom = gameInfo.wumpusRoom;
  if (room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
    printf("You smell a stench. ");
  }

  // Display hazard detection message if a pit is in an adjacent room
  int pit1Room = gameInfo.pitRoom1;
  int pit2Room = gameInfo.pitRoom2;
  if (room1 == pit1Room || room1 == pit2Room || room2 == pit1Room ||
      room2 == pit2Room || room3 == pit1Room || room3 == pit2Room) {
    printf("You feel a draft. ");
  }

  // Display hazard detection message if bats are in an adjacent room
  int bat1Room = gameInfo.batRoom1;
  int bat2Room = gameInfo.batRoom2;
  if (room1 == bat1Room || room1 == bat2Room || room2 == bat1Room ||
      room2 == bat2Room || room3 == bat1Room || room3 == bat2Room) {
    printf("You hear rustling of bat wings. ");
  }

  printf("\n\n");
} // end displayRoomInfo(...)

//--------------------------------------------------------------------------------
// When entering a room with bats, player will be transported to a random room.
// The bats will move to a location different from their previous location,
// new location of the player, and the location of other bats.
// If the player is transported into a room with the arrow, player picks it up
// Return the new postion of the bats
int encounterBats(struct GameInfo *gameInfo) {
  // Sets the new random player position and displays bat transportation message
  gameInfo->playerRoom = rand() % NUMBER_OF_ROOMS + 1;
  printf("Woah... you're flying! \n");
  printf("You've just been transported by bats to room %d.\n",
         gameInfo->playerRoom);

  // If player finds the arrow, player picks it up
  if (gameInfo->playerRoom == gameInfo->arrow) {
    gameInfo->arrow = -1;
    printf("Congratulations, you found the arrow and can once again shoot.\n");
  }

  // Finds a unique number different from all curent bat rooms and player room
  int *newBatRandPos = malloc(4 * sizeof(int));
  newBatRandPos[0] = gameInfo->batRoom1;
  newBatRandPos[1] = gameInfo->batRoom2;
  newBatRandPos[2] = gameInfo->playerRoom;

  setUniqueValues(newBatRandPos, 4);

  return newBatRandPos[3];
}

//--------------------------------------------------------------------------------
// If the player just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
// If the player just moved into a room with a pit, the player dies.
// If the player moves into a room with bats, they are transported to a random
// room
void checkForHazards(
    struct GameInfo *gameInfo, // Hazards location and game info
    bool *playerIsAlive, // Player is alive, but could die depending on the
                         // hazards
    int **pRooms)        // Cave rooms
{
  // Retrieve the room the player is in
  int playerPos = gameInfo->playerRoom;

  // Check for the Wumpus
  if (playerPos == gameInfo->wumpusRoom) {
    // The Wumpus always moves if it is currently in an odd-numbered room, and
    // it moves into the lowest-numbered adjacent room.
    if (gameInfo->wumpusRoom % 2 == 1) {
      // You got lucky and the Wumpus moves away
      printf("You hear a slithering sound, as the Wumpus slips away. \n"
             "Whew, that was close! \n");
      gameInfo->wumpusRoom =
          pRooms[playerPos][0]; // Choose the lowest-numbered adjacent room
    } else {
      // Wumpus kills you
      printf(
          "You briefly feel a slimy tentacled arm as your neck is snapped. \n"
          "It is over.\n");
      *playerIsAlive = false;
      return;
    }
  }

  // Check whether there is a pit
  if (playerPos == gameInfo->pitRoom1 || playerPos == gameInfo->pitRoom2) {
    // player falls into pit
    printf("Aaaaaaaaahhhhhh....   \n");
    printf("    You fall into a pit and die. \n");
    *playerIsAlive = false;
    return;
  }

  // Check if there are bats and check for hazards again in the new room after
  // being dropped by bats
  if (playerPos == gameInfo->batRoom1) {
    gameInfo->batRoom1 = encounterBats(gameInfo);
    checkForHazards(gameInfo, playerIsAlive, pRooms);
  }
  if (playerPos == gameInfo->batRoom2) {
    gameInfo->batRoom2 = encounterBats(gameInfo);
    checkForHazards(gameInfo, playerIsAlive, pRooms);
  }

  // If player finds the arrow, player picks it up
  if (playerPos == gameInfo->arrow) {
    gameInfo->arrow = -1;
    printf("Congratulations, you found the arrow and can once again shoot.\n");
  }
} // end checkForHazards(...)

//--------------------------------------------------------------------------------
// Takes in the number of rooms (up to 3, and then the list of the desired
// number of adjacent room numbers, separated by spaces. If an arrow can't go a
// direction because there is no connecting tunnel, it ricochets and moves to
// the lowest-numbered adjacent room and continues doing this until it has
// traveled the designated number of rooms.
// Process each room one by one. Check for player and Wumpus each time. If the
// arrow hits the Wumpus, you win! If the arrow hits you, you lose.
// If nothing happens, arrow is in the last room after ricochet(if any)
void guidedArrow(
    struct GameInfo *gameInfo, // All game setting variables
    int **pRooms,              // Cave rooms
    bool *wumpusIsAlive,       // Wumpus is alive but could die if hit by arrow
    bool *playerIsAlive) // Players is alive, but could die if hit by arrow
{
  int shootRooms;                      // Number of rooms to shoot through
  int room;                            // Player selected room to shoot at
  int arrowPos = gameInfo->playerRoom; // Set arrow position to current player
                                       // position, instead of -1

  // Prompt for rooms to shoot through
  printf("Enter the number of rooms (1..3) into which you want to shoot, "
         "followed by the rooms themselves: ");
  scanf(" %d", &shootRooms);

  // If more than 3 rooms selected, set to 3
  if (shootRooms > 3) {
    printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
    shootRooms = 3;
  }

  // Arrow movement for each room
  for (int i = 0; i < shootRooms; i++) {
    scanf(" %d", &room);
    if (roomIsAdjacent(pRooms[arrowPos], room)) {
      // Valid adjacent room, move the arrow position to the next room
      arrowPos = room;
      // Arrow killed Wumpus
      if (arrowPos == gameInfo->wumpusRoom) {
        printf("Wumpus has just been pierced by your deadly arrow! \n"
               "Congratulations on your victory, you awesome hunter you.\n");
        *wumpusIsAlive = false;
        return;
      }
      // Arrow killed player
      if (arrowPos == gameInfo->playerRoom) {
        printf("You just shot yourself.  \n"
               "Maybe Darwin was right.  You're dead.\n");
        *playerIsAlive = false;
        return;
      }
    }
    // Arrow Ricochet
    else {
      printf("Room %d is not adjacent.  Arrow ricochets...\n", room);
      // Set arrow position to the lowest adjacent room
      arrowPos = pRooms[arrowPos][0];
      // If the arrow ricochets, and kills the player
      if (arrowPos == gameInfo->playerRoom) {
        printf("You just shot yourself, and are dying.\n"
               "It didn't take long, you're dead.\n");
        *playerIsAlive = false;
        return;
      }
      // If the arrow ricochets, and kills Wumpus
      if (arrowPos == gameInfo->wumpusRoom) {
        printf("Your arrow ricochet killed the Wumpus, you lucky dog!\n"
               "Accidental victory, but still you win!\n");
        *wumpusIsAlive = false;
        return;
      }
    }
  }
  // Set arrow position to the last room it ends up in
  gameInfo->arrow = arrowPos;
  return;
} // end guidedArrow(...)

//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the player's
// location, useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo) {
  printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, "
         "bats1, bats2, and arrow: \n");
  // In the scanf below note that we put the space at the beginning of the scanf
  // so that any newline left over from a previous input is not read in and used
  // as the next move. Another option is having getchar() after the scanf()
  // statement.
  scanf(" %d %d %d %d %d %d %d", &theGameInfo->playerRoom,
        &theGameInfo->wumpusRoom, &theGameInfo->pitRoom1,
        &theGameInfo->pitRoom2, &theGameInfo->batRoom1, &theGameInfo->batRoom2,
        &theGameInfo->arrow);
  printf("\n");
}

//--------------------------------------------------------------------------------
int main(void) {
  struct GameInfo
      gameInfo; // Used to more easily pass game info variables around
  bool playerIsAlive = true; // Used in checking for end of game
  bool wumpusIsAlive = true; // Used in checking for end of game
  char typeOfMove;           // Used to handle user input letter
  int nextRoom; // User input of destination room number, used on a 'M' type
                // move

  // Pointer to an array of pointers that represents cave rooms
  int **pRooms = malloc(21 * sizeof(int *));

  // Dynamically allocate memory for 3 integers for each pointer in the array of
  // pointers, excluding pointer at index 0 to prevent off-by-one indexing
  // errors.
  for (int i = 1; i < 21; i++) {
    pRooms[i] = malloc(3 * sizeof(int));
  }

  // Array of integers pointed to by each pointer is initialized to appropriate
  // room connections
  pRooms[1][0] = 2, pRooms[1][1] = 5, pRooms[1][2] = 8;
  pRooms[2][0] = 1, pRooms[2][1] = 3, pRooms[2][2] = 10;
  pRooms[3][0] = 2, pRooms[3][1] = 4, pRooms[3][2] = 12;
  pRooms[4][0] = 3, pRooms[4][1] = 5, pRooms[4][2] = 14;
  pRooms[5][0] = 1, pRooms[5][1] = 4, pRooms[5][2] = 6;
  pRooms[6][0] = 5, pRooms[6][1] = 7, pRooms[6][2] = 15;
  pRooms[7][0] = 6, pRooms[7][1] = 8, pRooms[7][2] = 17;
  pRooms[8][0] = 1, pRooms[8][1] = 7, pRooms[8][2] = 9;
  pRooms[9][0] = 8, pRooms[9][1] = 10, pRooms[9][2] = 18;
  pRooms[10][0] = 2, pRooms[10][1] = 9, pRooms[10][2] = 11;
  pRooms[11][0] = 10, pRooms[11][1] = 12, pRooms[11][2] = 19;
  pRooms[12][0] = 3, pRooms[12][1] = 11, pRooms[12][2] = 13;
  pRooms[13][0] = 12, pRooms[13][1] = 14, pRooms[13][2] = 20;
  pRooms[14][0] = 4, pRooms[14][1] = 13, pRooms[14][2] = 15;
  pRooms[15][0] = 6, pRooms[15][1] = 14, pRooms[15][2] = 16;
  pRooms[16][0] = 15, pRooms[16][1] = 17, pRooms[16][2] = 20;
  pRooms[17][0] = 7, pRooms[17][1] = 16, pRooms[17][2] = 18;
  pRooms[18][0] = 9, pRooms[18][1] = 17, pRooms[18][2] = 19;
  pRooms[19][0] = 11, pRooms[19][1] = 18, pRooms[19][2] = 20;
  pRooms[20][0] = 13, pRooms[20][1] = 16, pRooms[20][2] = 19;

  // Seed the random number generator.  Change seed to time(0) to change output
  // each time. srand(time(0));
  srand(1);

  // Set random initial values for player, Wumpus, bats and pits
  initializeGame(&gameInfo);

  // Main playing loop.  Break when player dies, or player kills Wumpus
  while (playerIsAlive && wumpusIsAlive) {

    // Display current room information: Room number, hazards detected
    displayRoomInfo(gameInfo, pRooms);

    // Prompt for and handle move
    printf("%d. Enter your move (or 'D' for directions): ",
           gameInfo.moveNumber);
    // Note the extra space in the scanf below between the opening quote " and
    // the %c.
    //    This skips leading white space in the input so that the newline left
    //    over from a previous move is not read in and used as the current move.
    //    An alternative is using getchar() after the scanf() statement.
    scanf(" %c", &typeOfMove);

    typeOfMove = toupper(typeOfMove); // Make uppercase to facilitate checking

    // Check all types of user input and handle them.  This uses if-else-if code
    //   rather than switch-case, so that we can take advantage of break and
    //   continue.
    if (typeOfMove == 'D') {
      displayCave();
      displayInstructions();
      continue; // Loop back up to reprompt for the same move
    } else if (typeOfMove == 'P') {
      // To assist with play, display the cave layout
      displayCave();
      continue; // Loop back up to reprompt for the same move
    } else if (typeOfMove == 'M') {
      // Move to an adjacent room,
      // Note the extra space in the scanf below between the opening quote " and
      // the %c.
      //    This skips leading white space in the input so that the newline left
      //    over from a previous move is not read in and used as the current
      //    move.  An alternative is using getchar() after the scanf()
      //    statement.
      scanf(" %d", &nextRoom);

      if (roomIsAdjacent(pRooms[gameInfo.playerRoom], nextRoom)) {
        gameInfo.playerRoom = nextRoom; // move to a new room
        // Check if pit or wumpus is present in this new room
        checkForHazards(&gameInfo, &playerIsAlive, pRooms);
      } else {
        printf("Invalid move.  Please retry. \n");
        continue; // Doesn't count as a move, so retry same move.
      }
    } else if (typeOfMove == 'S') {
      // Check if arrow is with the player
      if (gameInfo.arrow == -1) {
        guidedArrow(&gameInfo, pRooms, &wumpusIsAlive, &playerIsAlive);
        // Wumpus moves to a random adjacent room if not hit by an arrow
        gameInfo.wumpusRoom = pRooms[gameInfo.wumpusRoom][0];
      }
      // Player does not have the arrow
      else {
        printf(
            "Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
      }
      // Increment the move number
      gameInfo.moveNumber = gameInfo.moveNumber + 1;
      continue;
    } else if (typeOfMove == 'C') {
      // Display Cheat information
      displayCheatInfo(gameInfo);
      continue; // Doesn't count as a move, so retry same move.
    } else if (typeOfMove == 'R') {
      // Specify resetting the hazards and player positions, useful for testing
      resetPositions(&gameInfo);
      continue; // Doesn't count as a move, so retry same move.
    } else if (typeOfMove == 'G') {
      // Prompt for room number guess and see if user has found the Wumpus
      int wumpusRoomGuess = 0;
      printf("Enter room (1..20) you think Wumpus is in: ");
      scanf(" %d", &wumpusRoomGuess);
      // See if userguess was correct, for a win or loss.
      if (gameInfo.wumpusRoom == wumpusRoomGuess) {
        printf("You won!\n");
      } else {
        printf("You lost.\n");
      }
      break;
    } else if (typeOfMove == 'X') {
      // Exit program
      playerIsAlive =
          false; // Indicate player is dead as a way to exit playing loop
      break;
    }

    // Increment the move number
    gameInfo.moveNumber = gameInfo.moveNumber + 1;

  } // end while(playerIsAlive && wumpusIsAlive)

  printf("\nExiting Program ...\n");

  return 0;
}