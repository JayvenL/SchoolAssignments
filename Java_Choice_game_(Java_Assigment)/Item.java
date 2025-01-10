
/**
 * This class represents an item which may be put
 * in a room in the game of Zuul.
 * 
 * @author Lynn Marshall 
 * @version A1 Solution
 *
 * @author Jayven Larsen 101260364
 * @version A2 v1.0
 */
public class Item
{
    // description of the item
    private String description;
    
    // weight of the item in kilgrams 
    private double weight; 

    private String name;

    /**
     * Constructor for objects of class Item.
     *
     *
     * @param description The description of the item
     * @param weight The weight of the item
     * @param name the name of the item
     */
    public Item(String description, double weight, String name)
    {
        this.description = description;
        this.name = name;
        this.weight = weight;
    }

    /**
     * Returns a description of the item, including its
     * description and weight.
     * 
     * @return  A description of the item
     */
    public String getDescription()
    {
        return name+ " : "+description + " that weighs " + weight + "kg.";
    }

    /**
     * Return the name of the item
     *
     * @return  the name of the item
     */
    public String getName()
    {
        return name;
    }
}
