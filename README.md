# Tetris





### During the development

To include new files in the project you have to right click on project directory and:

1. Select Manage Project Items, create the directory you want to add and, if you want to add some existing files, click Add Files...
2. Got to Option for Target > C/C++ > Include paths, after that add the path of the directory you want to include


I can't use RIT (Repetitive Interrupt Timer) to polling the joystick move and execute function to modify the LCD because in the ISR (Interrupt Service Routine) you can't use: printf, sprintf, LCD function or UART blocking; because they can cause internal interrupt, deadlock or HardFault.


