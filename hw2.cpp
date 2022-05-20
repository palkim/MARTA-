#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#define _left 'L'
#define _right 'R'
#define _up 'U'
#define _down 'D'
#define _blank 'I'

using namespace std;

typedef struct State {
    vector< vector<int> > configuration;
    int blank_index_col;
    int blank_index_row; 
    bool up_avail;
    bool down_avail;
    bool left_avail;
    bool right_avail;
} State;

typedef struct Agent{ 
  int agent_id;
  int last_move;
  struct State current_state;
} Agent;

vector<Agent> agents;
vector<State> visited_states;

int number_of_agents;
int step_number = 0;
bool goal_reached = false;

vector< vector<int> > initial_configuration;
vector< vector<int> > goal_configuration;

void printAgentStates() {
    cout << "Step:" << step_number << endl;
    for(int i = 0; i < number_of_agents; i++){
        cout << "Agent" << i+1 << ": " << char(agents[i].last_move) << " ";
        cout << "[";
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                if (j == 2 && k == 2) {
                    cout << agents[i].current_state.configuration[j][k] << "]";
                } else {
                    cout << agents[i].current_state.configuration[j][k] << " ";
                }
            }
        }
        cout << endl;
    }
}

bool isIdentical( vector< vector<int> > conf1, vector< vector<int> > conf2 ) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (conf1[i][j] != conf2[i][j]) {
               return false; 
            }
        }
    }
    return true; 
}

bool checkIfVisited(State state) {
    for(int i = 0; i < visited_states.size(); i++){
        if(isIdentical(state.configuration, visited_states[i].configuration)){
            return true;
        }
    }
    return false;
}

int getManhattanDistance(vector< vector<int> > conf1, vector< vector<int> > conf2){
    int manhattan_distance = 0;
    int x1, y1, x2, y2;

    for ( int i = 0; i < 9; i++ ) {

       for (int j = 0; j < 3; j++) {
           for (int k = 0; k < 3; k++) {
               if (conf1[j][k] == i) {
                   x1 = k;
                   y1 = j;
               }
           }
       } 

       for (int j = 0; j < 3; j++) {
           for (int k = 0; k < 3; k++) {
               if (conf1[j][k] == i) {
                   x2 = k;
                   y2 = j;
               }
           }
       }

       manhattan_distance += ( abs(x2 - x1) + abs(y2-y1) );

    }
    return manhattan_distance;
}

vector<State> getChildStates(State curr){
    vector<State> ret;
    int blank_row = curr.blank_index_row;
    int blank_col = curr.blank_index_col;
    int temp;
    if(curr.up_avail){
        printf("up\n");
        vector<vector<int> > temp_conf1 = curr.configuration;
        temp = temp_conf1[blank_row - 1][blank_col];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf1[blank_row-1][blank_col] = 0;
        temp_conf1[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf1;
        state.blank_index_row = blank_row - 1;
        state.blank_index_col = blank_col;
        if(state.blank_index_row - 1 > 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 > 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        ret.push_back(state);
    }
    if(curr.down_avail){
        printf("down\n");
        vector<vector<int> > temp_conf2 = curr.configuration;
        temp = temp_conf2[blank_row + 1][blank_col];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf2[blank_row+1][blank_col] = 0;
        temp_conf2[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf2;
        state.blank_index_row = blank_row + 1;
        state.blank_index_col = blank_col;
        if(state.blank_index_row - 1 > 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 > 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        ret.push_back(state);
    }
    if(curr.left_avail){
        printf("left\n");
        vector<vector<int> > temp_conf3 = curr.configuration;
        temp = temp_conf3[blank_row][blank_col-1];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf3[blank_row][blank_col-1] = 0;
        temp_conf3[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf3;
        state.blank_index_row = blank_row;
        state.blank_index_col = blank_col-1;
        if(state.blank_index_row - 1 > 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 > 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        ret.push_back(state);
    }
    if(curr.right_avail){
        printf("right\n");
        vector<vector<int> > temp_conf4 = curr.configuration;
        temp = temp_conf4[blank_row][blank_col+1];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf4[blank_row][blank_col+1] = 0;
        temp_conf4[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf4;
        state.blank_index_row = blank_row;
        state.blank_index_col = blank_col+1;
        if(state.blank_index_row - 1 > 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 > 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        ret.push_back(state);
    }
    return ret;
}

void play(){
    for(int i = 0; i < number_of_agents; i++){
        vector<State> child_states;
        State agent_state = agents[i].current_state;
        child_states = getChildStates(agent_state);
        cout << "i: " << i << endl;
        
        for(int j = 0; j < int(child_states.size()); j++){
            State child = child_states[j];
           
            vector<vector<int> > child_conf = child.configuration;
            for(int k = 0; k < 3; k++){
                for(int p = 0; p < 3; p++){
                    cout << child_conf[k][p] << " ";
                }
            }
            cout << endl;
        }
    }
}


int main(){
    srand(time(0));

    string fileName = "input.txt";
    ifstream in(fileName.c_str());
	
    if ( !in ) {	
		cerr << "Unable to find the input file!" << endl;
		exit(1);
	}

    string str;
    vector<string> lines;

    while ( getline(in,str) ) {
        if(str.size() > 0) {
            lines.push_back(str);
        }
    }

    in.close();

    for(int i = 0; i < lines.size(); i++){
        cout << lines[i] << endl;
    }

    number_of_agents = stoi(lines[0]);

    for(int i = 1; i < 7; i++){
        stringstream iss( lines[i] );
        vector<int> row;
        int number;
        while( iss >> number ){
            row.push_back( number );
        }
        if(i < 4) initial_configuration.push_back(row);
        else goal_configuration.push_back(row);
    }

    for(int i = 0; i < number_of_agents; i++){
        Agent robot;
        robot.agent_id = i;
        State state;
        state.configuration = initial_configuration;
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                if(state.configuration[j][k] == 0){
                    state.blank_index_row = j;
                    state.blank_index_col = k;
                }
            }
        }
        if(state.blank_index_row - 1 > 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 > 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }

        robot.current_state = state;
        robot.last_move = _blank;
        agents.push_back(robot); 
    }

    while(goal_reached == false){
        play();
        step_number++;
        printAgentStates();
    }    

   return 0;
}