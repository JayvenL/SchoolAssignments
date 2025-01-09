
import java.applet.Applet;
import java.applet.AudioClip;
import java.awt.FlowLayout;
import java.awt.event.*;
import java.net.URL;
import javax.swing.*;

/**
 * Borrowed from: http://www.javaknowledge.info/play-sound-on-button-click-in-java/, except
 * that I couldn't find the wav file, so I picked a couple from the internet.
 * One is a train, the other is a beep.  I also added some more functionality.
 * 
 * Select main to run this (not the constructor).
 * 
 * @author javaknowledge.info
 * @version May, 2013
 * 
 * @author Lynn Marshall
 * @version November, 2017
 */
 
public class PlayAlarm extends JFrame implements ActionListener, MouseListener {
 
   JButton btn, btn2, btn3, btn4;
   AudioClip click;
 
   public PlayAlarm() {
        setSize(400, 300);
        btn = new JButton("Play");
        btn2 = new JButton("Stop");
        
        btn3 = new JButton("Beep");
        btn4 = new JButton("Hover Beep");
        
        setTitle("Play Sounds");
        setLayout(new FlowLayout());
        getContentPane().add(btn);
        getContentPane().add(btn2);
        getContentPane().add(btn3);
        getContentPane().add(btn4);
 
        btn.addActionListener(PlayAlarm.this);
        btn2.addActionListener(PlayAlarm.this);
        btn3.addActionListener(PlayAlarm.this);
        //btn4.addActionListener(PlayAlarm.this);
        btn4.addMouseListener(this);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
   }
 
   @Override
   public void actionPerformed(ActionEvent e) {
        if (e.getSource() == btn) {
            URL urlClick = PlayAlarm.class.getResource("beep-08b.wav"); // train sound
            click = Applet.newAudioClip(urlClick);
            click.loop(); // clip loops until stopped (by clicking stop button)
        }
 
        if (e.getSource() == btn2) {
            click.stop(); // clip stops playing
        }
        
        if (e.getSource() == btn3) {
            URL urlClick = PlayAlarm.class.getResource("beep-08b.wav"); // beep
            click = Applet.newAudioClip(urlClick);
            click.play(); // just plays clip once
        }
   }
    
   /**
    * Detects when the mouse enters the component btn4.  We are only "listening" to the
    * btn4.  We play a beep when the mouse goes into that component.
    * 
    * @param e The mouse event triggered when the mouse was moved into the component
    */
   public void mouseEntered(MouseEvent e) {
        //JButton item = (JButton) e.getSource();
        URL urlClick = PlayAlarm.class.getResource("beep-08b.wav"); // beep
        click = Applet.newAudioClip(urlClick);
        click.loop(); // clip plays until mouse moves outside component
   }

   /**
    * Detects when the mouse exits the component btn4.  We are only "listening" to the
    * btn4.  We stop playing the beep when the mouse exits that component.
    * 
    * @param e The mouse event triggered when the mouse was moved out of the component
    */
   public void mouseExited(MouseEvent e) {
        click.stop(); // stops clip
   }
    
   /**
    * Not used.
    * 
    * @param e The mouse event triggered when the mouse was moved out of the component
    */
   public void mouseClicked(MouseEvent e) {
        
   }
   
   /**
    * Not used.
    * 
    * @param e The mouse event triggered when the mouse was moved out of the component
    */
   public void mousePressed(MouseEvent e) {
        
   }
   
   /**
    * Not used.
    * 
    * @param e The mouse event triggered when the mouse was moved out of the component
    */
   public void mouseReleased(MouseEvent e) {
        
   }
 
   public static void main(String[] args) {
        PlayAlarm p = new PlayAlarm();
        p.setVisible(true);
   }
}