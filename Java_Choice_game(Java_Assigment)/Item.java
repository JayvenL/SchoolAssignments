
/**
 * This class will create an item with a weight and a description
 * The description gives a representation of the item in String from
 * The weight will be a double holding the weight of the item in kg's
 * These items can be added to diffrent rooms
 *
 * @author Jayven Larsen
 * @version Feb 12 2024
 */
public class Item
{
    // instance variables - replace the example below with your own
    private String description_of_item;
    private double weight_of_item;
    private String name;

    /**
     * Constructor for objects of class Items
     * @param descrip is the description of the item
     * @param weight will be the weight of the item
     */
    public Item(String descrip, double weight)
    {
        // initialise instance variables
        description_of_item = descrip;
        name = descrip;
        weight_of_item = weight;
    }

    /**
     * Returns a printed string of the description of the item (the weight and what the item is)
     *
     * No return, prints out the information of the item
     */
    public void getItemInfo()
    {
        System.out.println( "a " + description_of_item + " that weighs " + weight_of_item +"kg");
        
    }

    public String getName()
    {
     return name;
    }
}
