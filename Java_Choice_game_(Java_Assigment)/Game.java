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
 * @author Lynn Marshall
 * @version A1 Solution
 *
 * @author Jayven 101260364
 * @version A2 v1.0
 */

public class Game
{
    private Parser parser;
    private Room currentRoom;
    private Room previousRoom;
    private Stack<Room> previousRoomStack;
    private Item[] curItem;
    private boolean hungry;
    private int items_held=0; //Holds the number of items that can be held

    /**
     * Create the game and initialise its internal map, as well
     * as the previous room (none) and previous room stack (empty).
     */
    public Game()
    {
        createRooms();

        curItem=  new Item[1];
        hungry= true;
        parser = new Parser();
        previousRoom = null;
        previousRoomStack = new Stack<Room>();


    }

    /**
     * Create all the rooms and link their exits together.
     */
    private void createRooms()
    {
        Room outside, theatre, pub, lab, office, transporter_room;
        Item chair1, chair2, chair3, chair4, bar, computer1, computer2, computer3, tree1, tree2, cookie1,cookie2, beamer1,
        beamer2;


        // create some items
        chair1 = new Item("a wooden chair",5,"chair");
        chair2 = new Item("a wooden chair",5,"chair");
        chair3 = new Item("a wooden chair",5,"chair");
        chair4 = new Item("a wooden chair",5,"chair");
        bar = new Item("a long bar with stools",95.67," bar with stools");
        computer1 = new Item("a PC",10,"PC");
        computer2 = new Item("a Mac",5,"mac");
        computer3 = new Item("a PC",10,"PC");
        tree1 = new Item("a fir tree",500.5,"tree");
        tree2 = new Item("a fir tree",500.5,"tree");
        cookie1 = new Item("a cookie", 1,"cookie");
        cookie2 =  new Item("a cookie", 1,"cookie");
        beamer1= new Beamer();
        beamer2 = new Beamer();



        // create the rooms
        outside = new Room("outside the main entrance of the university");
        theatre = new Room("in a lecture theatre");
        pub = new Room("in the campus pub");
        lab = new Room("in a computing lab");
        office = new Room("in the computing admin office");
        transporter_room = new TransporterRoom();


        // put items in the rooms
        outside.addItem(tree1);
        outside.addItem(tree2);
        theatre.addItem(chair1);
        pub.addItem(bar);
        lab.addItem(chair2);
        lab.addItem(computer1);
        lab.addItem(chair3);
        lab.addItem(computer2);
        office.addItem(chair4);
        office.addItem(computer3);
        outside.addItem(beamer1);
        office.addItem(beamer2);

        outside.addItem(cookie1);
        lab.addItem(cookie2);


        // initialise room exits
        outside.setExit("east", theatre);
        outside.setExit("south", lab);
        outside.setExit("west", pub);
        outside.setExit("north",transporter_room);

        theatre.setExit("west", outside);

        pub.setExit("east", outside);

        lab.setExit("north", outside);
        lab.setExit("east", office);

        office.setExit("west", lab);
        transporter_room.setExit("anywhere",null);



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
        else if (commandWord.equals("look")) {
            look(command);
        }
        else if (commandWord.equals("eat")) {
            eat(command);
        }
        else if (commandWord.equals("back")) {
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
        else if(commandWord.equals("fire")){
            fire(command);
        }
        else if(commandWord.equals("charge")){
            charge(command);
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
     * If we go to a new room, update previous room and previous room stack.
     * If the player is in a transporter room the second word does not matter
     *
     * @param command The command to be processed
     */
    private void goRoom(Command command)
    {
        // If the current you is a transporter room then the second word doesnt matter
        if (currentRoom instanceof TransporterRoom){
            previousRoom = currentRoom; // store the previous room
            previousRoomStack.push(currentRoom); // and add to previous room stack
            currentRoom= currentRoom.getExit("");
            printRoomAndCarry();
            return;
        }

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
            previousRoom = currentRoom; // store the previous room
            previousRoomStack.push(currentRoom); // and add to previous room stack
            currentRoom = nextRoom;
            printRoomAndCarry();
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

    private void printRoomAndCarry(){
        System.out.println(currentRoom.getLongDescription());
        if (curItem[0] == null) {
            System.out.println("Not holding anything");
        }
        else {
            System.out.println("You are currently holding: "+ curItem[0].getName());
        }
    }

    /**
     * "Look" was entered. Check the rest of the command to see
     * whether we really want to look.
     *
     * @param command The command to be processed
     */
    private void look(Command command)
    {
        if(command.hasSecondWord()) {
            System.out.println("Look what?");
        }
        else {
            // output the long description of this room
            printRoomAndCarry();
        }
    }

    /**
     * "Eat" was entered. Check the rest of the command to see
     * whether we really want to eat.
     *
     * @param command The command to be processed
     */
    private void eat(Command command){
        if(command.hasSecondWord()) {
            System.out.println("eat what?");
            return;
        }
        if(curItem[0] == null){
            System.out.println("You have no food");
            return;
        }
        if("cookie".equals(curItem[0].getName())){
            System.out.println("You've eaten a cookie");
            hungry = false;
            items_held += 5;
            curItem[0]=null;

        }
        else{
            System.out.println("You have no food");
        }
    }

    /**
     * "Back" was entered. Check the rest of the command to see
     * whether we really quit the game.
     *
     * @param command The command to be processed
     */
    private void back(Command command)
    {
        if(command.hasSecondWord()) {
            System.out.println("Back what?");
        }
        else {
            // go back to the previous room, if possible
            if (previousRoom==null) {
                System.out.println("No room to go back to.");
            } else {
                // go back and swap previous and current rooms,
                // and put current room on previous room stack
                Room temp = currentRoom;
                currentRoom = previousRoom;
                previousRoom = temp;
                previousRoomStack.push(temp);
                // and print description
                printRoomAndCarry();
            }
        }
    }

    /**
     * "StackBack" was entered. Check the rest of the command to see
     * whether we really want to stackBack.
     *
     * @param command The command to be processed
     */
    private void stackBack(Command command)
    {
        if(command.hasSecondWord()) {
            System.out.println("StackBack what?");
        }
        else {
            // step back one room in our stack of rooms history, if possible
            if (previousRoomStack.isEmpty()) {
                System.out.println("No room to go stack back to.");
            } else {
                // current room becomes previous room, and
                // current room is taken from the top of the stack
                previousRoom = currentRoom;
                currentRoom = previousRoomStack.pop();
                // and print description
                printRoomAndCarry();
            }
        }
    }
    /**
     * "take" was entered. Check the rest of the command to see
     * the item we want to pick up.
     * Holds the item in a 1 slot item array
     *
     * @param command The command to be processed
     */
    private void take(Command command) {

        String item_to_pick = command.getSecondWord();
        if(!command.hasSecondWord()){
            System.out.println("Take what?");
            return;
        }

        if (item_to_pick.equals("cookie")) {
            for (Item cookie : currentRoom.items) {
                if (cookie.getName().equals("cookie")) {
                    curItem[0] = cookie;
                    currentRoom.removeItem(cookie);
                    System.out.println("You've taken a cookie");
                    return;
                }
            }
        }
        else {

            if (checkHungry()) {
                System.out.println("You are hungry (look for a cookie)");
                return;
            }

            if (curItem[0] != null) {
                System.out.println("You are already holding something");
                return;
            }
            if (!currentRoom.items.isEmpty()) {
                for (Item item : currentRoom.items) {
                    if (item.getName().equals(item_to_pick)) {
                        items_held --;
                        curItem[0] = item;
                        currentRoom.removeItem(item);
                        System.out.println("You have picked up a " + item_to_pick);
                        return;
                    }
                }

            } else {
                System.out.println("That item is not in the room");
            }

        }
    }

    /**
     * "drop" was entered. Check the rest of the command to see
     * whether we really want to drop anything.
     *This will drop the item currently being held
     *
     * @param command The command to be processed
     */
    private void drop(Command command){
        if(command.hasSecondWord()){
            System.out.println("drop what?");
            return;
        }
        if(curItem[0]==null){
            System.out.println("There is nothing to drop");
        }
        else{
            System.out.println("You have dropped a "+ curItem[0].getName());
            currentRoom.items.add((curItem[0]));
            curItem[0]=null;
        }
    }

    /**
     * "charge" was entered. Check the rest of the command to see
     * whether we really want to charge.
     * While a beamer in being held, this function will charge the beamer
     * which will hold the current room in the beamer
     *
     * @param command The command to be processed
     */
    private void charge(Command command){
        if (command.hasSecondWord()){
            System.out.println("Charge what?");
        }
        if(curItem[0] == null||!Objects.equals(curItem[0].getName(), "beamer") ){
            System.out.println("You are not holding a beamer");

        }else{

            Beamer beamer = (Beamer) curItem[0];
            if(beamer.charge(currentRoom)) {
                System.out.println("The beamer is charged " + currentRoom.getShortDescription());
            }

        }
    }

    /**
     * "fire" was entered. Check the rest of the command to see
     * whether we really want to fire
     * While the beamer is being held and the beamer is charged with a room
     * It will bring the player to the room stored in the charged beamer.
     *
     * @param command The command to be processed
     */
    private void fire(Command command){
        if (command.hasSecondWord()){
            System.out.println("fire what?");
        }
        if(curItem[0] == null||!Objects.equals(curItem[0].getName(), "beamer") ){
            System.out.println("You are not holding a beamer");
        }
        else {
            Beamer beamer = (Beamer) curItem[0];
            currentRoom = beamer.fire();
            printRoomAndCarry();
        }

    }

    /**
     * Checks if the player is hungry or not
     *
     * @return boolean true or false if the player is hungry
     */
    private boolean checkHungry(){
        if (items_held <= 0) {
            hungry = true;
            return true;

        }
        return false;
    }


}


