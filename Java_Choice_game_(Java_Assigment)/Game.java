import java.util.ArrayList;
import java.util.Objects;
import java.util.Stack;

/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a very simple, text based adventure game.  Users 
 *  can walk around some scenery. That's all. It should really be extended 
 *  to make it more interesting!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kolling and David J. Barnes
 * @version 2006.03.30
 * 
 * @author Jayven Larsen
 * @version Jan 30th 2024
 */

public class Game 
{
    private Parser parser;
    private Room currentRoom;

    Room previousRoom = null;
    Room stackCurrRoom = null;
    private Stack<Room> roomStack = new Stack<Room>();
    private Item[] held_items = new Item[1];
    boolean eaten_cookie=false;
    int items_held=5; //Holds the number of items that can be held

        
    /**
     * Create the game and initialise its internal map.
     */
    public Game() 
    {
        createRooms();
        parser = new Parser();
        held_items[0]=null;


        
    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office;
      
        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        
        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);

        theatre.setExit("west", outside);

        pub.setExit("east", outside);

        lab.setExit("north", outside);
        lab.setExit("east", office);

        office.setExit("west", lab);

        add_item(1,"cookie",pub);
        add_item(1,"log",outside);
        add_item(1,"cookie",office);
        add_item(1,"cookie",outside);






        currentRoom = outside;  // start game outside


    }

    /**
     *  Main play routine.  Loops until end of play.
     */
    public void play() 
    {            
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
                
        boolean finished = false;
        while (! finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
        }
        System.out.println("Thank you for playing.  Good bye.");
    }

    /**
     * Print out the opening message for the player.
     */
    private void printWelcome()
    {
        System.out.println();
        System.out.println("Welcome to the World of Zuul!");
        System.out.println("World of Zuul is a new, incredibly boring adventure game.");
        System.out.println("Type 'help' if you need help.");
        System.out.println();
        System.out.println(currentRoom.getLongDescription());
    }

    /**
     * Given a command, process (that is: execute) the command.
     * 
     * @param command The command to be processed
     * @return true If the command ends the game, false otherwise
     */
    private boolean processCommand(Command command) 
    {
        boolean wantToQuit = false;

        if(command.isUnknown()) {
            System.out.println("I don't know what you mean...");
            return false;
        }

        String commandWord = command.getCommandWord();
        if (commandWord.equals("help")) {
            printHelp();
        }
        else if (commandWord.equals("go")) {
            
            goRoom(command);
        }
        else if (commandWord.equals("quit")) {
            wantToQuit = quit(command);
        }
        else if(commandWord.equals("look")){
            look(command);
        }
        else if(commandWord.equals("eat")){
            eat(command);
        }
        else if (commandWord.equals("back")){
            back(command);
        }
        else if (commandWord.equals("stackBack")) {
            stackBack(command);
        }
        else if(commandWord.equals("take")){
            take(command);
        }
        else if(commandWord.equals("drop")){
            drop(command);
        }
        // else command not recognised.
        return wantToQuit;
    }

    // implementations of user commands:

    /**
     * Print out some help information.
     * Here we print a cryptic message and a list of the 
     * command words.
     */
    private void printHelp() 
    {
        System.out.println("You are lost. You are alone. You wander");
        System.out.println("around at the university.");
        System.out.println();
        System.out.println("Your command words are:");
        System.out.println(parser.getCommands());
    }

    /** 
     * Try to go to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     * 
     * @param command The command to be processed
     */
    private void goRoom(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("Go where?");
            return;
        }

        String direction = command.getSecondWord();

        // Try to leave current room.
        Room nextRoom = currentRoom.getExit(direction);

        if (nextRoom == null) {
            System.out.println("There is no door!");
        }
        else {
            roomStack.push(currentRoom);
            previousRoom = currentRoom;
            currentRoom = nextRoom;
            System.out.println(currentRoom.getLongDescription());
        }
    }

    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * 
     * @param command The command to be processed
     * @return true, if this command quits the game, false otherwise
     */
    private boolean quit(Command command) 
    {
        if(command.hasSecondWord()) {
            System.out.println("Quit what?");
            return false;
        }
        else {
            return true;  // signal that we want to quit
        }
    }
    
    /** 
     * "look" was entered. Check the rest of the command to see
     * whether we really want to look .
     * 
     * @param command The command to be processed
     * No return, prints out the current room description
     */
    private void look(Command command)
    {
        if(command.hasSecondWord()) {
            System.out.println("look where?");
        }
        else{
            System.out.println(currentRoom.getLongDescription());
        }   
    }
    /** 
     * "eat" was entered. Check the rest of the command to see
     * whether we really want to eat.
     * 
     * @param command The command to be processed
     * No return, prints out that you've eaten already
     */
    private void eat(Command command){
        if(command.hasSecondWord()) {
            System.out.println("eat what?");
            return;
        }
        if(held_items[0] == null){
            System.out.println("You have no food");
            return;
        }
        if("cookie".equals(held_items[0].getName())){
            System.out.println("You've eaten a cookie");
            eaten_cookie = true;
            items_held += 5;
            held_items[0]=null;

        }
        else{
            System.out.println("You have no food");
        }   
    }
    /**
     * Adds an item to a room
     *
     *
     * @param weight is the weight of the item (int)
     * @param description is the description of the item
     * @param room is the room in which the item is placed
     */
    private void add_item(double weight, String description, Room room){

        Item item =  new Item(description, weight);
        room.item_list.add(item);


    }

    /**
     * Goes to the previous room the player was in
     *
     *
     * @param command is the command
     * No return, brings player back to previous room
     */
    private void back(Command command){
        if(command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("back where?");
            return;
        }
        if(previousRoom == null){
            System.out.println("You cant go back");

        }
        else{
            Room temp_room = currentRoom;
            currentRoom = previousRoom;
            previousRoom = temp_room;

            System.out.println(currentRoom.getLongDescription());
        }
    }

    /**
     * Holds a stack of all the rooms the player has been in
     * When called, the player will return to the room at the 
     * top of the stack, this can be done until the player has 
     * reached the starting position/room
     *
     *
     * @param command is the command
     * No return, brings the player back to the room at top of stack
     */
    private void stackBack(Command command) {
        if (command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            System.out.println("stackBack where?");
            return;
        }
        else{
            if (roomStack.isEmpty()){
                System.out.println("You can go back");
            }else{
                currentRoom = roomStack.peek();
                roomStack.pop();
            System.out.println(currentRoom.getLongDescription());

            }
        }

    }

    private void take(Command command) {

        String item_to_pick = command.getSecondWord();
        if(!command.hasSecondWord()){
            System.out.println("Take what?");
            return;
        }

        if (item_to_pick.equals("cookie") && held_items[0] == null) {
            for (Item cookie : currentRoom.item_list) {
                if (cookie.getName().equals("cookie")) {
                    held_items[0] = cookie;
                    eaten_cookie = true;
                    System.out.println("You've taken a cookie");
                    return;
                }
            }
        }
        else {
            if (eaten_cookie == false) {
                System.out.println("You have not eaten a cookie");
                return;
            }
            if (items_held == 0) {
                System.out.println("You're hungry you cant pick anything up");
            }


            if (held_items[0] != null) {
                System.out.println("You are already holding something");
                return;
            }
            if (!currentRoom.item_list.isEmpty()) {
                for (Item item : currentRoom.item_list) {
                    if (item.getName().equals(item_to_pick)) {
                        held_items[0] = item;
                        System.out.println("You have picked up a " + item_to_pick);
                        items_held--;
                        return;
                    } else {
                        System.out.println("That item is not in the room");
                    }
                }

            } else {
                System.out.println("That item is not in the room");
            }

        }
    }

    private void drop(Command command){
        if(command.hasSecondWord()){
            System.out.println("drop what?");
        }
        if(held_items[0]==null){
            System.out.println("There is nothing to drop");
            return;
        }
        else{
            System.out.println("You have dropped a "+ held_items[0].getName());
            held_items[0]=null;
        }
    }

}



