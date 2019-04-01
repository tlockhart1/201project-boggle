***BOGGLE***

- by Trevor Lockhart

***VIDEO DEMO***

- https://alabama.box.com/s/3i5mxp97q15l7gntivnj8h2c2dkzn5np

- dictionary setup-
	-login as admin with 'sudo -i' command
	-navigate to /usr/share/dict/
	-type 'cat american-english > words.txt'
	-use the 'mv' command to move 'words.txt' to the folder in which
	 the program is compiled

	-if you don't have the dictionary already installed, download it
	 via the command 'sudo apt-get install wamerican', then follow the 
	 above instructions

- Compiling

	-type 'make boggle' to compile and run the program

- menu options-

	-use arrows keys to select:
		-Person VS Person Start
		-Person VS Computer Start
		-Player Logs

- Person VS Person Options-
	-you must enter the dimenions of the game board
	-Then Player 1 will input the name by which his player logs 
	 will be saved
	-Then Player 2 will do the same
	
	-The game board will then displayed
	-To view all columns and rows, press 'w' to scroll up, 's' to
	 scroll down, 'a' to scroll left, and 'd' to scroll right
	-The top left most cell's position on the game board is displayed
	 so that you can determine the portion of the game board that is 
	 being displayed.

	-When you are ready to enter words, press 'r'. a prompt will be 
	 provided for Player 1 to enter his words. Seperate words by spaces.
	 After Player 1 enters his words, Player 2 will be prompted to do
	 the same.

	-After both players have entered their words, scores will be
	 calculated and each player's win or loss will be recorded for
	 later reference.	

	-Player 1's list can be scrolled through by pressing 'u' and 'j'
	-Player 2's list can be scrolled through by pressing 'i' and 'k'
	-The total word list can be scrolled though by pressing 'o' and 'l'

	-Press 'F1' to exit the game.

- Person VS Computer

	-Precedure is the same as Player VS Player minus input needed by
	 a second player.  

	-Press 'F1' to exit the game.

	-Computer difficulty is random. It can be beaten.

- Player Logs
	-User will be prompted for a name

	-If the log exists, total wins and losses will be displayed, as
	 well as the who they won or lost against

	-The scrollable by pressing 'u' or 'j'

	-If the user log does not exit, nothing will be displayed

	-To return to the start menu, press 'b'

	
