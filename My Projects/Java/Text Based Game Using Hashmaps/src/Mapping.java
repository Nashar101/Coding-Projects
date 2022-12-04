import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Mapping {

    public static final int INITIAL_LOCATION = 95;

    /**
     * TODO
     * create a static LocationMap object
     */
    static LocationMap locationMap = new LocationMap();
    /**
     * TODO
     * create a vocabulary HashMap to store all directions a user can go
     */
    HashMap vocabulary = new HashMap();
    /**
     * TODO
     * create a FileLogger object
     */
    FileLogger fileLogger = new FileLogger();
    /**
     * TODO
     * create a ConsoleLogger object
     */
    ConsoleLogger consoleLogger = new ConsoleLogger();

    public Mapping() {
        //vocabulary.put("QUIT", "Q"); //example
        /** TODO
         * complete the vocabulary HashMap <Key, Value> with all directions.
         * use the directions.txt file and crosscheck with the ExpectedInput and ExpectedOutput files to find the keys and the values
         */
        vocabulary.put("W", "W");
        vocabulary.put("D", "D");
        vocabulary.put("SW", "SW");
        vocabulary.put("U", "U");
        vocabulary.put("SE", "SE");
        vocabulary.put("N", "N");
        vocabulary.put("Q", "Q");
        vocabulary.put("SOUTH", "S");
        vocabulary.put("WEST", "W");
        vocabulary.put("SOUTHEAST", "SE");
        vocabulary.put("SOUTHWEST", "SW");
        vocabulary.put("NORTHEAST", "NE");
        vocabulary.put("NORTHWEST", "NW");
        vocabulary.put("UP", "U");
        vocabulary.put("DOWN", "D");
        vocabulary.put("NORTH", "N");
        vocabulary.put("EAST", "E");
        vocabulary.put("QUIT", "Q");
        vocabulary.put("S", "S");
        vocabulary.put("NE", "NE");
        vocabulary.put("NW", "NW");
        vocabulary.put("E", "E");
    }

    public void mapping() {

        /** TODO
         * create a Scanner object
         */
        Scanner inp = new Scanner(System.in);
        /**
         * initialise a location variable with the INITIAL_LOCATION
         */
        int location = INITIAL_LOCATION;
        while (true) {

            /** TODO
             * get the location and print its description to both console and file
             * use the FileLogger and ConsoleLogger objects
             */
            consoleLogger.log(locationMap.get(location).getDescription());
            fileLogger.log(locationMap.get(location).getDescription());
            /** TODO
             * verify if the location is exit
             */
            if (location == 0) {
                break;
            }
            /** TODO
             * get a map of the exits for the location
             */
            /** TODO
             * print the available exits (to both console and file)
             * crosscheck with the ExpectedOutput files
             * Hint: you can use a StringBuilder to append the exits
             */
            StringBuilder s = new StringBuilder();
            int i;
            if (location != 0) {
                s.append(locationMap.get(location).getExits().keySet());
                s.replace(0, 1, "");
                s.replace(s.length() - 1, s.length(), ", ");
                consoleLogger.log("Available exits are " + s);
                fileLogger.log("Available exits are " + s);
            }
            /** TODO
             * input a direction
             * ensure that the input is converted to uppercase
             */
            String input = inp.nextLine().toUpperCase();
            /** TODO
             * are we dealing with a letter / word for the direction to go to?
             * available inputs are: a letter(the HashMap value), a word (the HashMap key), a string of words that contains the key
             * crosscheck with the ExpectedInput and ExpectedOutput files for examples of inputs
             * if the input contains multiple words, extract each word
             * find the direction to go to using the vocabulary mapping
             * if multiple viable directions are specified in the input, choose the last one
             */
            String[] array;
            array = input.split(" ");
            int j = 0;
            for (j = array.length - 1; j >= 0; --j) {
                if (vocabulary.containsKey(array[j])) {
                    if (locationMap.get(location).getExits().containsKey(vocabulary.get(array[j]))) {
                        if ((array[j].toUpperCase().equals("UP"))) {
                            location = locationMap.get(location).getExits().get(vocabulary.get(array[j]));
                        } else if ((array[j].length() <= 2) & (array.length == 1)) {
                            location = locationMap.get(location).getExits().get(vocabulary.get(array[j]));
                        } else if (array[j].length() > 2) {
                            location = locationMap.get(location).getExits().get(vocabulary.get(array[j]));
                        } else {
                            consoleLogger.log("You cannot go in that direction");
                            fileLogger.log("You cannot go in that direction");
                        }
                        break;
                    }
                } else if (j == 0) {
                    consoleLogger.log("You cannot go in that direction");
                    fileLogger.log("You cannot go in that direction");
                    break;
                }
            }
            /** TODO
             * if user can go in that direction, then set the location to that direction
             * otherwise print an error message (to both console and file)
             * check the ExpectedOutput files
             */
        }
    }

    public static void main(String[] args) {
        /**TODO
         * run the program from here
         * create a Mapping object
         * start the game
         */
        Mapping mapping = new Mapping();
        mapping.mapping();
    }

}
