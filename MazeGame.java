import java.io.IOException;
import java.util.*;
import java.io.*;


/**
 * Maze Game
 * INFO1103 Assignment 2
 * 2017 Semester 1
 *
 * The Maze Game.
 * The player can step left, right, up or down.
 * you need to complete the maze within a given number of steps.
 *
 * As in any maze, there are walls that you cannot move through. If you try to
 * move through a wall, you lose a life. You have a limited number of lives.
 * There is also gold on the board that you can collect if you move ontop of it.
 * @author Monica
 * @date April, 2017
 *
 */
public class MazeGame {
 
static int lives;
static int steps;
static int gold;
static int rows;
static String[] board;
static int XPosition;
static int YPosition;
static char landon;
static boolean isGameEnd;

    // A sample variable to show you can put variables here.
    // You would initialise it in initialiseGame method.
    // e.g. Have the following line in the initialiseGame method.
    // sampleVariable = 1;

     /**
     * If the configuration file name is "DEFAULT", load the default
     * game configuration.
     *
     * NOTE: Please also initialise all of your class variables.
     *
     * @args configFileName The name of the game configuration file to read from.
     * @throws IOException If there was an error reading in the game.
     *         For example, if the input file could not be found.
     */
    public static void initialiseGame(String configFileName) throws IOException {
     File infile = new File(configFileName);
     Scanner scan = new Scanner(infile);
		  lives = Integer.parseInt(scan.next());
		  steps = Integer.parseInt(scan.next());
		  gold = Integer.parseInt(scan.next());
		  rows = Integer.parseInt(scan.next());
		  board = new String[rows];
		  scan.nextLine();
			for(int i = 0; i < board.length; i++){
			  board[i] = scan.nextLine();
            }
		
		 for(int i = 0; i < board.length; i++){
          for(int j = 0; j < board[i].length(); j++){
            char ch = board[i].charAt(j);

              if(ch == '&'){
                XPosition = j;
				YPosition = i;
               }
  		   }
  		}


      scan.close();
    }

    /**
     * @args toFileName The name of the file to save the game configuration to.
     * @throws IOException If there was an error writing the game to the file.
     */
    public static void saveGame(String toFileName) throws IOException {
      File outfile = new File(toFileName);
      PrintWriter output = new PrintWriter(outfile);
				output.print(lives + " ");
				output.print(steps + " ");
				output.print(gold + " ");
				output.print(rows + "\n");
                for(int i = 0; i < board.length; i++){
					 	 output.println(board[i]);
						}

			  output.close();
		System.out.printf("Successfully saved the current game configuration to '%s'.\n", toFileName);



    }

    
     // @return The players current x position.
     
    public static int getCurrentXPosition() {
     
        return XPosition;
    }

    /**
     * @return The players current y position.
     */
    public static int getCurrentYPosition() {
    
        return YPosition;
    }

    /**
     * @return The number of lives the player currently has.
     */
    public static int numberOfLives() {
        return lives;
    }

    /**
     * @return The number of steps remaining in the game.
     */
    public static int numberOfStepsRemaining() {
        return steps;
    }

    /**
     * @return The amount of gold the player has collected so far.
     */
    public static int amountOfGold() {
        return gold;
    }


    /**
     * Checks to see if the player has completed the maze.
     * The player has completed the maze if they have reached the destination.
     *
     * @return True if the player has completed the maze.
     */
    public static boolean isMazeCompleted() {
	  int j = 0;
      for( int i = 0; i < board.length; i++){
		  if(!board[i].contains("@")){ j += 1;}
	  }
      if(j == board.length ){return true;}  
      
        return false;
	
    }

    /**
     * Checks to see if it is the end of the game.
     * It is the end of the game if one of the following conditions is true:
     *  - There are no remaining steps.
     *  - The player has no lives.
     *  - The player has completed the maze.
     *
     * @return True if any one of the conditions that end the game is true.
     */
    public static boolean isGameEnd() {

        if(steps <= 0 || lives <= 0 || isMazeCompleted()){return true;}
 

        else{return false;}

        
    }

    /**
     * @args x The x coordinate.
     * @args y The y coordinate.
     */
    public static boolean isValidCoordinates(int x, int y) {
        if( x < board[0].length() && y < board.length && x >= 0 && y >= 0){
          return true;
        }
        else{ return false;
        }
    }

    /**
     * Checks if a move to the given coordinates is valid.
     * A move is invalid if:
     *  - It is move to a coordinate off the board.
     *  - There is a wall at that coordinate.
     *  - The game is ended.
     *
     * @args x The x coordinate to move to.
     * @args y The y coordinate to move to.
     * @return True if the move is valid, otherwise false.
     */
    public static boolean canMoveTo(int x, int y) {
        if(isValidCoordinates(x,y) == true && board[y].charAt(x) != '#'  && isGameEnd() == false ){
          return true;
        }
        else {

          return false; }
    }

    /**
     * Move the player to the given coordinates on the board.
     * @args x The x coordinate to move to.
     * @args y The y coordinate to move to.
     */
    public static void moveTo(int x, int y) {
        if(canMoveTo(x,y) == true){
          XPosition = x;
          YPosition = y;
          
          for(int i = 0; i < board.length; i++){  
                  board[i] = board[i].replace('&','.'); 
          }

	
			
		  char landon = board[y].charAt(x);
  
	      board[y] = board[y].substring(0, x) + '&' + board[y].substring(x + 1);
          System.out.printf("Moved to (%d, %d).\n", x, y);
          steps -= 1;
			
			if(Character.isDigit(landon)){
            gold += Character.getNumericValue(landon);
            System.out.printf("Plus %c gold.\n", landon);
          }
			
        }
        else if(canMoveTo(x,y) == false){
          lives -= 1;
          System.out.println("Invalid move. One life lost.");
          steps -= 1;
        }
		
		     
    }


    public static void printHelp() {
          System.out.println("Usage: You can type one of the following commands.");
          System.out.println("help         Print this help message.");
          System.out.println("board        Print the current board.");
          System.out.println("status       Print the current status.");
          System.out.println("left         Move the player 1 square to the left.");
          System.out.println("right        Move the player 1 square to the right.");
          System.out.println("up           Move the player 1 square up.");
          System.out.println("down         Move the player 1 square down.");
          System.out.println("save <file>  Save the current game configuration to the given file.");
   return;
   }

    public static void printStatus() {
      System.out.printf("Number of live(s): %d\n", lives);
      System.out.printf("Number of step(s) remaining: %d\n", steps);
      System.out.printf("Amount of gold: %d\n", gold);
    return;
    }


    public static void printBoard() {
      for(int i = 0; i < board.length; i++){System.out.println(board[i]);}
    }

    /**
     * Performs the given action by calling the appropriate helper methods.
     * [For example, calling the printHelp() method if the action is "help".]
     *
     * If it is not a valid action, an IllegalArgumentException should be thrown.
     *
     * @args action The action we are performing.
     * @throws IllegalArgumentException If the action given isn't one of the
     *         allowed actions.
     */
    public static void performAction(String action) throws IllegalArgumentException {
     String [] saving = action.split(" ");
	 
        if(action.equalsIgnoreCase("help")){ printHelp(); }
        else if(action.equalsIgnoreCase("board")){ printBoard(); }
        else if(action.equalsIgnoreCase("status")){ printStatus(); }
        else if(action.equalsIgnoreCase("left")){ moveTo(getCurrentXPosition() - 1, getCurrentYPosition()); }
        else if(action.equalsIgnoreCase("right")){ moveTo(getCurrentXPosition() + 1, getCurrentYPosition()); }
        else if(action.equalsIgnoreCase("up")){ moveTo(XPosition, getCurrentYPosition() - 1); }
        else if(action.equalsIgnoreCase("down")){ moveTo(XPosition, getCurrentYPosition() + 1); }
          else if(saving.length > 1 && saving[0].equalsIgnoreCase("save")){
			        if (saving.length > 2) {
						        throw new IllegalArgumentException();
					    }
			        try {saveGame( saving[1] );} 
			        catch(IOException e){System.out.printf("Error: Could not save the current game configuration to '%s'.\n", saving[1]);}
	
		}
		else {
				throw new IllegalArgumentException();
			}
	
	
      


    }

    /**
     * The main method of your program.
     *
     * @args args[0] The game configuration file from which to initialise the
     *       maze game. If it is DEFAULT, load the default configuration.
     */
    public static void main(String[] args) {
      if(args.length == 0){
        System.out.println("Error: Too few arguments given. Expected 1 argument, found 0.");
        System.out.println("Usage: MazeGame [<game configuration file>|DEFAULT]");
        return;
      }

      else if(args.length > 1){
        System.out.printf("Error: Too many arguments given. Expected 1 argument, found %d.\n", args.length);
        System.out.println("Usage: MazeGame [<game configuration file>|DEFAULT]");
        return;
      }

      try{initialiseGame(args[0]);
		
		 }
		catch(IOException e){ System.out.printf("Error: Could not load the game configuration from '%s'.",args[0]); 
							return;
							}

    
		Scanner keyboard = new Scanner(System.in);
		

      while(isGameEnd() == false){

        try{
          String input = keyboard.nextLine().trim();
		  int i = 0;
				while (i < input.length()) {
					if (input.charAt(i) == ' ') {
						i++;
					}
					else {
						break;
					}
				}
				if (i == input.length()) {
					continue;
				}
		   else{
			  try{
			  performAction(input);
			  }
			  catch (IllegalArgumentException e) {
					System.out.printf("Error: Could not find command '%s'.\nTo find the list of valid commands, please type 'help'.\n", input);    
			  }
			  
		   }
		}
	    catch (NoSuchElementException e) {
				System.out.println("You did not complete the game.");
				return;
		}
		
      }
	    
			  		
		if (isMazeCompleted() == true) {
			System.out.println("Congratulations! You completed the maze!");
			System.out.println("Your final status is:");
			printStatus();
			return;
		}
		else if (lives <= 0 && steps <= 0) {
			System.out.println("Oh no! You have no lives and no steps left.");
			System.out.println("Better luck next time!");
			return;
		}
		else if (steps <= 0) {
			System.out.println("Oh no! You have no steps left.");
			System.out.println("Better luck next time!");
			return;
		}
		else if (lives <= 0) {
			System.out.println("Oh no! You have no lives left.");
			System.out.println("Better luck next time!");
			return;
		}
			
    }

}

