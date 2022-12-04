import java.sql.*;

public class Database {
    //This method executes a query and returns the number of albums for the artist with name artistName
    public int getTitles(String artistName) {
        int titleNum = 0;
        //SELECT count(album.artistid) from album INNER JOIN artist p ON album.artistid = p.artistid WHERE p.name = 'David Bowie';
        //String sql = "SELECT count(?) from album INNER JOIN artist p ON album.artistid = p.artistid WHERE p.name ='?'"
        //Implement this method
        String sql = "SELECT count(?) from album INNER JOIN artist p ON album.artistid = p.artistid WHERE p.name = '" + artistName + "'";
        try {
            Connection connection = DriverManager.getConnection(Credentials.URL, Credentials.USERNAME, Credentials.PASSWORD);
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            preparedStatement.clearParameters();
            preparedStatement.setString(1, "album");
            ResultSet resultSet = preparedStatement.executeQuery();
            resultSet.next();
            titleNum = resultSet.getInt("count");
            preparedStatement.close();
            connection.close();
            resultSet.close();
        } catch (SQLException exception) {
            System.out.println(exception);
        }
        //Prevent SQL injections!
        return titleNum;
    }

    // This method establishes a DB connection & returns a boolean status
    public boolean establishDBConnection() {
        try {
            Class.forName("org.postgresql.Driver");
            Connection connection = DriverManager.getConnection(Credentials.URL, Credentials.USERNAME, Credentials.PASSWORD);
            return (connection.isValid(5));
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        //Implement this method
        //5 sec timeout
        return false;
    }
}