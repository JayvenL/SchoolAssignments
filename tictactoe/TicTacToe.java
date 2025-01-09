import java.util.*;
import java.lang.StringBuilder;

/**
 * A class modelling a tic-tac-toe (noughts and crosses, Xs and Os) game.
 * 
 * @author Lynn Marshall
 * @version November 8, 2012
 *
 * @author Jayven Larsen
 * @version Assignment 3 v1.0
 */

public class TicTacToe {
   public static final int PLAYER_X = 2; // player using "X"
   public static final int PLAYER_O = 1; // player using "O"
   public static final int EMPTY = 0;  // empty cell
   public static final String TIE = "T"; // game ended in a tie

   protected int player;   // current player (PLAYER_X or PLAYER_O)

   protected int winner;   // winner: PLAYER_X, PLAYER_O, TIE, EMPTY = in progress

   protected int numFreeSquares; // number of squares still free

   protected int board[][];// 3x3 array representing the board

   protected int row, col; 

   /**
    * Constructs a new Tic-Tac-Toe board.
    */
   public TicTacToe() {
      board = new int[3][3];
      player = PLAYER_O;
      winner = EMPTY;
      for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 3; j++) {
            board[i][j] = 0;
         }
      }
   }

   /**
    * Sets everything up for a new game.  Marks all squares in the Tic Tac Toe board as empty,
    * and indicates no winner yet, 9 free squares and the current player is player X.
    */
   protected void clearBoard() {

      winner = EMPTY;
      numFreeSquares = 9;
      player = PLAYER_O;     // Player O always has the first turn.
   }


   /**
    * Plays one game of Tic Tac Toe.
    */

   public void playGame() {
      clearBoard(); // clear the board
   }


   /**
    * Returns true if filling the given square gives us a winner, and false
    * otherwise.
    *
    * @param int row of square just set
    * @param int col of square just set
    * @return true if we have a winner, false otherwise
    */
   protected boolean haveWinner(int row, int col) {
      // unless at least 5 squares have been filled, we don't need to go any further
      // (the earliest we can have a winner is after player X's 3rd move).

      if (numFreeSquares > 4) return false;

      // Note: We don't need to check all rows, columns, and diagonals, only those
      // that contain the latest filled square.  We know that we have a winner
      // if all 3 squares are the same, as they can't all be blank (as the latest
      // filled square is one of them).

      // check row "row"
      if (board[row][0] == (board[row][1]) &&
              board[row][0] == (board[row][2])) return true;

      // check column "col"
      if (board[0][col]==(board[1][col]) &&
              board[0][col]==(board[2][col])) return true;

      // if row=col check one diagonal
      if (row == col)
         if (board[0][0]==(board[1][1]) &&
                 board[0][0]==(board[2][2])) return true;

      // if row=2-col check other diagonal
      if (row == 2 - col)
         if (board[0][2]==(board[1][1]) &&
                 board[0][2]==(board[2][0])) return true;

      // no winner yet
      return false;
   }

}
  
    


