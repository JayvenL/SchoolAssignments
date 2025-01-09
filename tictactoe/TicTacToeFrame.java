
import java.applet.Applet;
import java.applet.AudioClip;
import java.awt.print.PrinterException;
import java.net.URL;
import java.util.*;
import java.awt.*; 
import java.awt.event.*;
import javax.swing.*;

/**
 * A class modelling a tic-tac-toe (noughts and crosses, Xs and Os) game in a very
 * simple GUI window.
 * 
 * @author Lynn Marshall
 * @version November 8, 2012
 *
 * @author Jayven Larsen 101260364
 * @version Assignment 3 V1.0
 */

public class TicTacToeFrame extends TicTacToe implements ActionListener
{

    private final JLabel labelStatus;
    private final JLabel winStatus;

    private final JButton topLeft;

    private final JButton topMiddle;

    private final JButton topRight;

    private final JButton midLeft;
    private final JButton midMiddle;
    private final JButton midRight;

    private final JButton bottomLeft;
    private final JButton bottomMiddle;
    private final JButton bottomRight;
    private final JMenuItem resetGame;

    /* The quit menu item */
    private final JMenuItem quitGame;

    private final ArrayList<JButton> buttons;
    private int XPlayerWins;
    private int OPLayerWins;
    AudioClip click;





    /**
    * Constructs a new Tic-Tac-Toe board and sets up the basic
    * JFrame containing a JTextArea in a JScrollPane GUI.
     * Constructs the 3x3 grid of button
    */
   public TicTacToeFrame() throws PrinterException {
       buttons = new ArrayList<JButton>();
       labelStatus = new JLabel();
       winStatus = new JLabel();



       JFrame frame = new JFrame("TicTacToe");



       JPanel buttonPanel = new JPanel();
       buttonPanel.setLayout(new GridLayout(3, 3));

       Container contentPane = frame.getContentPane();
       contentPane.setLayout(new GridLayout(3, 1));


       JMenuBar menubar = new JMenuBar();
       frame.setJMenuBar(menubar); // add menu bar to our frame

       JMenu fileMenu = new JMenu("Game"); // create a menu
       menubar.add(fileMenu); // and add to our menu bar

       resetGame = new JMenuItem("Reset"); // create a menu item called "Reset"
       fileMenu.add(resetGame); // and add to our menu

       quitGame = new JMenuItem("Quit"); // create a menu item called "Quit"
       fileMenu.add(quitGame); // and add to our menu

       // TicTacToe buttons
       topLeft = new JButton("");
       buttons.add(topLeft);
       buttonPanel.add(topLeft);

       topMiddle = new JButton("");
       buttons.add(topMiddle);
       buttonPanel.add(topMiddle);

       topRight = new JButton("");
       buttons.add(topRight);
       buttonPanel.add(topRight);

       midLeft = new JButton ("");
       buttons.add(midLeft);
       buttonPanel.add(midLeft);

       midMiddle = new JButton ("");
       buttons.add(midMiddle);
       buttonPanel.add(midMiddle);

       midRight = new JButton ("");
       buttons.add(midRight);
       buttonPanel.add(midRight);

       bottomLeft = new JButton ("");
       buttons.add(bottomLeft);
       buttonPanel.add(bottomLeft);
       bottomMiddle = new JButton ("");
       buttons.add(bottomMiddle);
       buttonPanel.add(bottomMiddle);
       bottomRight = new JButton ("");
       buttons.add(bottomRight);
       buttonPanel.add(bottomRight);


       labelStatus.setFont(new Font("Arial", Font.PLAIN, 30));
       winStatus.setFont(new Font("Arial", Font.PLAIN, 30));
       if (winner==EMPTY ){
           labelStatus.setText("Game is progress, current turn: "+checkPlayer(player));
           winStatus.setText("O has won: "+OPLayerWins+", X has won: "+XPlayerWins);
       }

       for(JButton button: buttons){
           button.addActionListener(this);
       }



      resetGame.addActionListener(this);
      quitGame.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event)
          {
              System.exit(0); // quit
          }
      }
      );





       contentPane.add(buttonPanel);
       contentPane.add(labelStatus);
       contentPane.add(winStatus);
       frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
       frame.pack();
       frame.setResizable(true);
       frame.setVisible(true);




   }

    /**
     * The function is to register the press of each button
     * After every button press the board is updated by calling the function
     * @param e the event to be processed
     */
   public void actionPerformed(ActionEvent e) {
       Object o = e.getSource();
       if (o instanceof JButton button) {
           if (button == topLeft) {
               board[0][0] = player;
               row=0;
               col=0;
               updateGame(row,col,topLeft);
           } else if (button == topMiddle) {
               board[0][1] = player;
               row=0;
               col=1;
               updateGame(row,col,topMiddle);
           } else if (button == topRight) {
               board[0][2] = player;
               row=0;
               col=2;
               updateGame(row,col,topRight);
           } else if (button == midLeft) {
               board[1][0] = player;
               row=1;
               col=0;
               updateGame(row,col,midLeft);
           } else if (button == midMiddle) {
               board[1][1] = player;
               row=1;
               col=1;
               updateGame(row,col,midMiddle);
           } else if (button == midRight) {
               board[1][2] = player;
               row=1;
               col=2;
               updateGame(row,col,midRight);
           } else if (button == bottomLeft) {
               board[2][0] = player;
               row=2;
               col=0;
               updateGame(row,col,bottomLeft);
           } else if (button == bottomMiddle) {
               board[2][1] = player;
               row=2;
               col=1;
               updateGame(row,col,bottomMiddle);
           } else if (button == bottomRight) {
               board[2][2] = player;
               row=2;
               col=2;
               updateGame(row,col,bottomRight);
           }

       }
       else{
           JMenuItem item = (JMenuItem)o;

           if(item == resetGame){
               URL urlClick = PlayAlarm.class.getResource("boing_poing.wav"); // train sound
               click = Applet.newAudioClip(urlClick);
               click.play();
               resetGame();
           }
       }
   }

    /**
     * The function updates the game after every button press
     * @param latest_row
     * @param latest_col
     * @param buttonPressed
     */
   private void updateGame(int latest_row,int latest_col, JButton buttonPressed){

       buttonPressed.setEnabled(false);
       URL urlClick = PlayAlarm.class.getResource("click.wav"); // train sound
       click = Applet.newAudioClip(urlClick);
       click.play();

       buttonPressed.setText(checkPlayer(player));
       numFreeSquares--;

       if(haveWinner(latest_row,latest_col)){
            winner=player;
           if (Objects.equals(winner, PLAYER_X)){
               XPlayerWins++;
           }
           else{
               OPLayerWins++;
           }
           for(JButton button:buttons){
               button.setEnabled(false);
           }
           labelStatus.setText("Winner: "+checkPlayer(winner));
           winStatus.setText("O has won: "+OPLayerWins+", X has won: "+XPlayerWins);

       }
       else{
           if(numFreeSquares < 1){
               labelStatus.setText("TIE game");
           }else {
               if (player == PLAYER_X)
                   player = PLAYER_O;
               else
                   player = PLAYER_X;
               labelStatus.setText("Game is progress, current turn: "+checkPlayer(player));
               winStatus.setText("O has won: "+OPLayerWins+", X has won: "+XPlayerWins);
           }
       }

   }

    /**
     *
     * This is a function to reset the game after the reset button in the menu is pressed
     */
   private void resetGame()
    {
        clearBoard();
        for (JButton button : buttons) {
            button.setEnabled(true);
            button.setText("");
            labelStatus.setText("Game is progress, current turn: "+checkPlayer(player));
            winStatus.setText("O has won: "+OPLayerWins+", X has won: "+XPlayerWins);
        }

    }

    /**
     * Helper function to find the player in an int form and change it to either X or O
     * @param player
     * @return String X or O
     */
    private String checkPlayer(int player){
       if(player == 1){
           return "O";
       }
       if (player == 2){
           return "X";
       }
       return "";
    }


    /**
     * Used for hover events.
     *
     * @param e The mouse event triggered when the mouse was moved out of the component
     */
    private void mouseEntered(MouseEvent e){
       JMenu Item = (JMenu) e.getSource();
       Item.setSelected(true);
    }

    /**
     * Used for hover events.
     *
     * @param e The mouse event triggered when the mouse was moved out of the component
     */
    private void mouseExited(MouseEvent e){
        JMenu Item = (JMenu) e.getSource();
        Item.setSelected(false);
    }
    /**
     * Not used.
     *
     * @param e The mouse event triggered when the mouse was moved out of the component
     */
    private void mouseClicked(MouseEvent e) {

    }

    /**
     * Not used.
     *
     * @param e The mouse event triggered when the mouse was moved out of the component
     */
    private void mousePressed(MouseEvent e) {

    }

    /**
     * Not used.
     *
     * @param e The mouse event triggered when the mouse was moved out of the component
     */
    private void mouseReleased(MouseEvent e) {

    }




}