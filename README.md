# Tetris

Board specific: LandTiger (LPC1768)


Also if I'm programming a game with static roles, I'm trying to write code that can work also with different roles.


---

### General structure

activePiece contains the current tetromino that is moving.
activePiece constain start_x, start_y, end_x, end_y. They delimit the tetromino's area (not perfectly but a good approximation), in particular they the top left angle and the bottom right angle of the tetronimo.

Once the tetromino is draw on the filed, those values become the values adapted to the field.

I use the polling technique to get the joystick value. But, maybe I can change it like the timer structure, with a variable that is modified while the interrupt occurs.

In the infinite loop in the main I check if:
- game is over
- game is in pause
- game is running
    - joystick pressed
    - shift down tetromino
    - tetromino placed


---

### During the development

- To include new files in the project you have to right click on project directory and:

    1. Select Manage Project Items, create the directory you want to add and, if you want to add some existing files, click Add Files...
    2. Got to Option for Target > C/C++ > Include paths, after that add the path of the directory you want to include


- I can't use RIT (Repetitive Interrupt Timer) to polling the joystick move and execute function to modify the LCD because in the ISR (Interrupt Service Routine) you can't use: printf, sprintf, LCD function or UART blocking; because they can cause internal interrupt, deadlock or HardFault.
Correction: actually only with the emulator this doesn't work, with the real board there shouldn't be problems.


- GLCD.h contains only struct and functions declarations. Instead, GLCD.c contains global variables and functions definitions.

- I declare global variables in the sample.c file, and in others files I use the "extern" keyword to use the global variables.


- To debug better, change the optimisation to see the value of the variable in the Watch 1 window, and to see the execution of all the line of code.
  Got to Option for Target > C/C++ > Optimisation, and select -O0. At the end of the debugging you can put the previous value of the optimisation.

- I tried to maintain a syntactic coherence while writing code. For example, I commented functions, I used spaces to make the code more readable, and things like this.

- I had to flag the "Use MicroLIB" option in Target, because my file size exceeded the maximun size for keil free version



