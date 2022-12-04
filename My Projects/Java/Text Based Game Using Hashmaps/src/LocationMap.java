import java.util.*;
import java.io.*;

//class that behaves like a map
public class LocationMap implements Map<Integer, Location> {

    private static final String LOCATIONS_FILE_NAME = "locations.txt";
    private static final String DIRECTIONS_FILE_NAME = "directions.txt";

    /**
     * TODO
     * create a static locations HashMap
     */
    static HashMap<Integer, Location> locations = new HashMap<>();

    static {
        /** TODO
         * create a FileLogger object
         */
        FileLogger fileLogger = new FileLogger();
        /** TODO
         * create a ConsoleLogger object
         */
        ConsoleLogger consoleLogger = new ConsoleLogger();
        /** TODO
         * Read from LOCATIONS_FILE_NAME so that a user can navigate from one location to another
         * use try-with-resources/catch block for the FileReader
         * extract the location and the description on each line
         * print all locations and descriptions to both console and file
         * check the ExpectedOutput files
         * put each location in the locations HashMap using temporary empty hashmaps for exits
         */

        try {
            String[] array;
            String delimiter = ":";
            FileReader fileReader = new FileReader(LOCATIONS_FILE_NAME);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String s = "Available locations:";
            int n = 0;
            while (s != null) {
                consoleLogger.log(s.replaceFirst(",", ": "));
                fileLogger.log(s.replaceFirst(",", ": "));
                s = bufferedReader.readLine();
                if (s != null) {
                    array = s.replaceFirst(",", ":").split(delimiter);
                    Map<String, Integer> exits = new HashMap<>();
                    Location location = new Location(n, array[1], exits);
                    locations.put(n, location);
                    n = n + 1;
                }
            }
        } catch (IOException e) {
            System.out.println("File not found");
        }
        /**TODO
         * Read from DIRECTIONS_FILE_NAME so that a user can move from A to B, i.e. current location to next location
         * use try-with-resources/catch block for the FileReader
         * extract the 3 elements  on each line: location, direction, destination
         * print all locations, directions and destinations to both console and file
         * check the ExpectedOutput files
         * for each location, create a new location object and add its exit
         */
        String[] array;
        String delimiter = ",";
        try {
            String s = "Available directions:";
            FileReader fileReader = new FileReader(DIRECTIONS_FILE_NAME);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            while (s != null) {
                consoleLogger.log(s.replace(",", ": "));
                fileLogger.log(s.replace(",", ": "));
                s = bufferedReader.readLine();
                if (s != null) {
                    array = s.split(delimiter);
                    Location location = locations.get(Integer.parseInt(array[0]));
                    location.addExit(array[1], Integer.parseInt(array[2]));
                }
            }
        } catch (IOException e) {
            System.out.println("File not found");
        }
    }

    /**
     * TODO
     * implement all methods for Map
     *
     * @return
     */
    @Override
    public int size() {
        //TODO
        return locations.size();
    }

    @Override
    public boolean isEmpty() {
        //TODO
        return locations.isEmpty();
    }

    @Override
    public boolean containsKey(Object key) {
        //TODO
        return locations.containsKey(key);
    }

    @Override
    public boolean containsValue(Object value) {
        //TODO
        return locations.containsValue(value);
    }

    @Override
    public Location get(Object key) {
        //TODO
        return locations.get(key);
    }

    @Override
    public Location put(Integer key, Location value) {
        //TODO
        return locations.put(key, value);
    }

    @Override
    public Location remove(Object key) {
        //TODO
        return locations.remove(key);
    }

    @Override
    public void putAll(Map<? extends Integer, ? extends Location> m) {
        //TODO
        locations.putAll(m);
    }

    @Override
    public void clear() {
        //TODO
        locations.clear();
    }

    @Override
    public Set<Integer> keySet() {
        //TODO
        return locations.keySet();
    }

    @Override
    public Collection<Location> values() {
        //TODO
        return locations.values();
    }

    @Override
    public Set<Entry<Integer, Location>> entrySet() {
        //TODO
        return locations.entrySet();
    }
}
