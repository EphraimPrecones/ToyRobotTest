#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> // For std::transform
#include <cctype>    // For std::toupper

using namespace std;

bool bdebug = false; //This is for debug purposes only! set to false before release

//Max Unit
const int MAX_X = 5;
const int MAX_Y = 5;

// Enum for front of robot
enum ePointing { NORTH, EAST, SOUTH, WEST, INVALID_or_COUNT };

class ToyRobot {
private:
    int x_coor, y_coor;
    ePointing towards;
    bool bPlaced;
    
    bool bCheckValidPosition(int x_coor, int y_coor, ePointing towards) {
        int i_valid_direction;
        if (towards >= ePointing::NORTH && towards < ePointing::INVALID_or_COUNT) {
            i_valid_direction = 1;
        }
        else{
            i_valid_direction = 0;
        }
        return (x_coor >= 0 && x_coor < MAX_X && y_coor >= 0 && y_coor < MAX_Y && i_valid_direction == 1);
    }

    // Function to convert string directionStr to ePointing enum
    string ePointingToString(const ePointing& towards) {
        if (towards == ePointing::NORTH) {
            return "NORTH";
        } else if (towards == ePointing::SOUTH) {
            return "SOUTH";
        } else if (towards == ePointing::EAST) {
            return "EAST";
        } else if (towards == ePointing::WEST) {
            return "WEST";
        } else {
            // Default to INVALID if direction is invalid
            return "INVALID DIRECTION";
        }
    }

public:
    ToyRobot(){
        bPlaced = false;
    }

    // Method to place the robot on the table
    void place(int new_x, int new_y, ePointing new_towards) {
        if (bCheckValidPosition(new_x, new_y, new_towards)) {
            x_coor = new_x;
            y_coor = new_y;
            towards = new_towards;
            bPlaced = true;  

            //Debug
            if(bdebug == true){log_current_location();}     
        }
        else{      
            cout << "PLACE Command IGNORED! You Try to Input:"<<new_x<<","<<new_y
            <<","<< ePointingToString(new_towards)<<endl;            
        }
    }

    void move() {
        if (!bPlaced){cout<<"Please use PLACE Command First!\n";return;}

        int new_x = x_coor, new_y = y_coor;

        switch (towards) {
            case ePointing::NORTH:
                new_y++;
                break;
            case ePointing::EAST:
                new_x++;
                break;
            case ePointing::SOUTH:
                new_y--;
                break;
            case ePointing::WEST:
                new_x--;
                break;
        }

        if (bCheckValidPosition(new_x, new_y, towards)) { //towards direction will not change on this function
            x_coor = new_x;
            y_coor = new_y;
            //Debug
            if(bdebug == true){log_current_location();}
        }
        else{
            cout << "MOVE Command IGNORED! You Try to Input:"<<new_x<<","<<new_y
            <<","<< ePointingToString(towards)<<endl;
        }
    }

    void left() {
        if (!bPlaced){cout<<"Please use PLACE Command First!\n";return;}
        towards = static_cast<ePointing>((towards - 1 + 4) % 4);
        //Debug
        if(bdebug == true){log_current_location();}
    }

    // Method to rotate the robot 90 degrees to the right
    void right() {
        if (!bPlaced){cout<<"Please use PLACE Command First!\n";return;}
        towards = static_cast<ePointing>((towards + 1) % 4);
        //Debug
        if(bdebug == true){log_current_location();}
    }

    void report() {
        if (!bPlaced){cout<<"Please use PLACE Command First!\n";return;}
        cout << "Output:" << endl;
        cout << x_coor << "," << y_coor << "," << ePointingToString(towards) << endl;
    }

    //For Debug purposes only
    void log_current_location(){
        cout << "TEST output: " << x_coor <<","<<y_coor<<","<< ePointingToString(towards) << endl;
    }

    //for Test only
    void test(){
        ToyRobot robot;
        ePointing towards;

        bdebug = true; // to enable logging of current direction

        cout <<"TEST PLACE VALID COMMAND"<<endl;
        for (int i_x = 0; i_x<MAX_X; i_x++){
            for(int i_y = 0; i_y<MAX_Y; i_y++){
                for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
                    towards = static_cast<ePointing>(i_direct);
                    robot.place(i_x,i_y,towards);
                }
            }
        }

        //Test outside the max table units
        cout <<"\nTEST PLACE COMMAND both X & Y outside the table"<<endl;
        for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
            towards = static_cast<ePointing>(i_direct);
            robot.place(MAX_X,MAX_Y,towards); //testing above step (max unit -1)
            robot.place(MAX_X-(MAX_X+1),MAX_Y-(MAX_Y+1),towards); //testing below minimum unit (0)
        }

        cout <<"\nTEST PLACE COMMAND with X within and Y outside"<<endl;
        for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
            towards = static_cast<ePointing>(i_direct);
            robot.place(MAX_X-1,MAX_Y,towards); //testing above step (max unit -1)
            robot.place(0,MAX_Y-(MAX_Y+1),towards); //testing below minimum unit (0)
        }
        cout <<"\nTEST PLACE COMMAND with X outside and Y within"<<endl;
        for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
            towards = static_cast<ePointing>(i_direct);
            robot.place(MAX_X,MAX_Y-1,towards); //testing above step (max unit -1)
            robot.place(MAX_X-(MAX_X+1),0,towards); //testing below minimum unit (0)
         }

        cout <<"\nTEST MOVE COMMAND in all directions from the Center Table: "<<(MAX_X-1)/2 <<","<<(MAX_Y-1)/2<<endl;
        for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
            towards = static_cast<ePointing>(i_direct);
            robot.place((MAX_X-1)/2, (MAX_Y-1)/2,towards);
            if(towards == ePointing::NORTH || towards == ePointing::SOUTH){
                for(int i = 0; i < ((MAX_Y-1)/2)+1;i++){
                    robot.move();
                }
            }else if(towards == ePointing::EAST || towards == ePointing::WEST){
                for(int i = 0; i < ((MAX_X-1)/2)+1;i++){
                    robot.move();
                } 
            }
        }
    
        cout <<"\nTEST RIGHT & LEFT COMMAND start from North Clockwise and Counter-Clockwise Direction"<<endl;
        cout<<"\nInitial Direction"<<endl;
        robot.place(0,0,ePointing::NORTH);
        cout <<"\nTEST RIGHT COMMAND: Clockwise"<<endl;
        for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
            robot.right();
        }
        cout <<"\nTEST LEFT COMMAND: Counter-Clockwise"<<endl;
            for(int i_direct = 0; i_direct<static_cast<int>(ePointing::INVALID_or_COUNT); i_direct++){
                robot.left();
            }
        bdebug = false; //disable again the logs of movement
    }

};



// Function to convert string directionStr to ePointing enum
ePointing stringToDirection(const string& directionStr) {
    if (directionStr == "NORTH") {
        return ePointing::NORTH;
    } else if (directionStr == "SOUTH") {
        return ePointing::SOUTH;
    } else if (directionStr == "EAST") {
        return ePointing::EAST;
    } else if (directionStr == "WEST") {
        return ePointing::WEST;
    } else {
        // Default to INVALID if direction is invalid
        return ePointing::INVALID_or_COUNT;
    }
}

bool parsePlaceCommand(const string& command, int& x, int& y, ePointing& towards) {
    // Create a string stream to parse the command
    stringstream ss(command);
    string placeCmd;

    ss >> placeCmd; // Read the command "PLACE"

    // Check if the command starts with "PLACE"
    if (placeCmd != "PLACE") {
        return false; // Invalid command format
    }
 
    char comma; // To store the comma after X and Y values

    // Read X, Y, and Direction from the command
    ss >> x >> comma >> y >> comma;
    
    // Read Direction as string
    string directionStr;
    ss >> directionStr;

    // Convert direction string to Direction enum
    towards = stringToDirection(directionStr);
    return true; // Successfully parsed
}

//void RunCommands(ToyRobot& robot, const string& command){
void RunCommands(ToyRobot& robot, string& command){
    //Conver all command to upper case
     for (char& ch : command) {
        // Check if the character is a letter
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            // Convert the letter to uppercase
            ch = std::toupper(static_cast<unsigned char>(ch));
        }
    }
    string get_command;
    // Create a string stream to parse the command
    stringstream ss(command);
    ss >> get_command; // Read the command "PLACE" or (MOVE, LEFT, RIGHT)
    // Check if the command starts with "PLACE"
    
    
    if (get_command == "PLACE") {
        
        int out_x, out_y;
        ePointing out_towards;

        parsePlaceCommand(command, out_x, out_y, out_towards);
        robot.place(out_x, out_y, out_towards);
        
    }

    else if (get_command == "MOVE"){
        robot.move();
    }
    else if (get_command == "RIGHT"){
        robot.right();
    }
    else if (get_command == "LEFT"){
        robot.left();
    }
    else if (get_command == "REPORT"){
        robot.report();
    }        
    else if (get_command == "TEST"){
        robot.test();
    }
    else{
        cout << "INVALID COMMAND!!!"<< endl;
    }
    
}


void EnterCommands(){
    string input;
    ToyRobot robot;
    cout << "Enter PLACE Command (PLACE x,y,towards): " << endl;
    while (true) {
        getline(cin, input);
        
        if (input == "EXIT" || input == "exit") {
            break;
        }
        else{
            RunCommands(robot, input);
        }
        

    }
}



int main() {
    cout << "NOTE: "<<endl;
    cout << "1.Use ""PLACE X,Y,Direction"" command First!"<<endl;
    cout << "2.Use MOVE command to one step towards the current location and direction"<<endl;
    cout << "3.Use RIGHT command to rotate one step Clockwise"<<endl;
    cout << "4.Use LEFT command to rotate one step Counter-Clockwise"<<endl;
    cout << "5.(OPTIONAL FOR TEST) Use TEST command to Test the Different Robot Commands and it's limitations!"<<endl;
    cout << "6.Type exit or EXIT to close the window and terminate the program\n"<<endl;
    EnterCommands();
    return 0;
}