
/**
 * The beamer will hold the current room with a charge command, and then when fired
 * it will bring you to the room it was charged in
 *
 * @author Jayven Larsen 101260364
 * @version A2 v1.0
 */

public class Beamer extends Item

{
    // instance variables - replace the example below with your own
    private boolean charged;
    private Room storedRoom;
    
    

    /**
     * Constructor for objects of class Beamer
     */
    public Beamer()
    {
        // initialise instance variables

        super("a beamer",10,"beamer");
        charged = false;
    }

    /**
     * This method will charge the beamer item, the beamer will hold the current
     * room location the player is in
     *
     * @param cur_room the current room the player is in to hold in storedRoom
     * @return boolean True is the beamer it got charged, false if it is charged
     */
    public boolean charge(Room cur_room)
    {
        if(isCharged()){
            System.out.println("The beamer is already charged");
            return false;
        }
        storedRoom = cur_room;
        charged = true;
        return true;


    }

    /**
     * Fire function which will bring the player to the room stored in storedRoom
     *
     * @return  Room the room to be teleported too
     */

    public Room fire(){
        if(!isCharged()){
            System.out.println("The beamer is not charged");
        }
        charged = false;
        return storedRoom;


    }

    /**
     * Function to check if the beamer is currently charged or not
     *
     *
     */

    public boolean isCharged(){

        return charged;
    }


    /**
     * Getter function for the name of the beamer item
     *
     * @return String the name
     */
    public String getName(){
        return "beamer";
    }
}
