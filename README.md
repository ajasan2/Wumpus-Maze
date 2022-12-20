# Wumpus-Maze
Class Project

Credit: UIC Fall 2022
        Professor Dale Reed
        
Welcome to Hunt the Wumpus, a classic text-based adventure game by Gregory Yob from 1972.  
(See a copy of his original write-up, and the Wikipedia page.  Play a text-based copy on-line. See what one version of this program looked like in BASIC.)

The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms, as indicated in the diagram shown below:

![image](https://user-images.githubusercontent.com/105524167/208590853-d49e851d-5653-4cdb-b15a-df7d673f8ebc.png)

Hunt the Wumpus:                                             
The Wumpus lives in a completely dark cave of 20 rooms. Each 
room has 3 tunnels leading to other rooms.                   
                                                             
Hazards:                                                     
1. Two rooms have bottomless pits in them.  If you go there you fall and die.   
2. Two other rooms have super-bats.  If you go there, the bats grab you and     
   fly you to some random room, which could be troublesome.  Then those bats go 
   to a new room different from where they came from and from the other bats.   
3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    
   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       
    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   
    will move into the lowest-numbered adjacent room anytime you shoot an arrow.
    When you move into the Wumpus' room, then he wakes and moves if he is in an 
    odd-numbered room, but stays still otherwise.  After that, if he is in your 
    room, he snaps your neck and you die!                                       
                                                                                
Moves:                                                                          
On each move you can do the following, where input can be upper or lower-case:  
1. Move into an adjacent room.  To move enter 'M' followed by a space and       
   then a room number.                                                          
2. Shoot your guided arrow through a list of up to three adjacent rooms, which  
   you specify.  Your arrow ends up in the final room.                          
   To shoot enter 'S' followed by the number of rooms (1..3), and then the      
   list of the desired number (up to 3) of adjacent room numbers, separated     
   by spaces. If an arrow can't go a direction because there is no connecting   
   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      
   continues doing this until it has traveled the designated number of rooms.   
   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  
   automatically pick up the arrow if you go through a room with the arrow in   
   it.                                                                          
3. Enter 'R' to reset the person and hazard locations, useful for testing.      
4. Enter 'C' to cheat and display current board positions.                      
5. Enter 'D' to display this set of instructions.                               
6. Enter 'P' to print the maze room layout.                                     
7. Enter 'X' to exit the game.                                                  
                                                                                
Good luck!
