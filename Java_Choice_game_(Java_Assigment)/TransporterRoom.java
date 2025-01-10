import java.util.Random;

/**
 * This class creates a transporter class
 * When a player leaves they will be transported randomly to one of the others rooms
 *
 * @author Jayven Larsen 101260364
 * @version v1.0 March 2024
 */
public class TransporterRoom extends Room
{
    // instance variables - replace the example below with your own

    private Random rangomGen;

    /**
     * Constructor for objects of class TransporterRoom
     */
    public TransporterRoom()
    {
        super("Transporter Room");

    }

    /**
     * Get exit function to find a random room in the game
     * And transport the player to one of the randomly selected rooms
     *
     * @param direction direction of exit (not used for the transporterRoom)
     * @return the randomly selected room from the find random room function
     */
    public Room getExit(String direction)
    {
        return findRandomRoom();
    }

    /**
     * This functions will return a random room from the static Arraylist of rooms
     *
     *
     * @return Room The randomly chosen room
     */
    private Room findRandomRoom(){
        rangomGen = new Random();
        int index =  rangomGen.nextInt(getRooms().size());
        return getRooms().get(index);
    }
}
