#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#define _left 'R'
#define _right 'L'
#define _up 'D'
#define _down 'U'
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
    int createdWith; 
    int hG;
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

    for ( int i = 1; i < 9; i++ ) {

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
               if (conf2[j][k] == i) {
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
        vector<vector<int> > temp_conf1 = curr.configuration;
        temp = temp_conf1[blank_row - 1][blank_col];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf1[blank_row-1][blank_col] = 0;
        temp_conf1[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf1;
        state.blank_index_row = blank_row - 1;
        state.blank_index_col = blank_col;
        if(state.blank_index_row - 1 >= 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 >= 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        state.createdWith = _up;
        state.hG = getManhattanDistance(state.configuration, goal_configuration);
        ret.push_back(state);
    }
    if(curr.down_avail){
        vector<vector<int> > temp_conf2 = curr.configuration;
        temp = temp_conf2[blank_row + 1][blank_col];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf2[blank_row+1][blank_col] = 0;
        temp_conf2[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf2;
        state.blank_index_row = blank_row + 1;
        state.blank_index_col = blank_col;
        if(state.blank_index_row - 1 >= 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 >= 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        state.createdWith = _down;
        state.hG = getManhattanDistance(state.configuration, goal_configuration);
        ret.push_back(state);
    }
    if(curr.left_avail){
        vector<vector<int> > temp_conf3 = curr.configuration;
        temp = temp_conf3[blank_row][blank_col-1];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf3[blank_row][blank_col-1] = 0;
        temp_conf3[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf3;
        state.blank_index_row = blank_row;
        state.blank_index_col = blank_col-1;
        if(state.blank_index_row - 1 >= 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 >= 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        state.createdWith = _left;
        state.hG = getManhattanDistance(state.configuration, goal_configuration);
        ret.push_back(state);
    }
    if(curr.right_avail){
        vector<vector<int> > temp_conf4 = curr.configuration;
        temp = temp_conf4[blank_row][blank_col+1];
        //make blank_row - 1 blank_col 0, push blank_row blank_col temp
        temp_conf4[blank_row][blank_col+1] = 0;
        temp_conf4[blank_row][blank_col] = temp;
        State state;
        state.configuration = temp_conf4;
        state.blank_index_row = blank_row;
        state.blank_index_col = blank_col+1;
        if(state.blank_index_row - 1 >= 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 >= 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        state.createdWith = _right;
        state.hG = getManhattanDistance(state.configuration, goal_configuration);
        ret.push_back(state);
    }
    return ret;
}

void play(){
    for(int i = 0; i < number_of_agents; i++){
        vector<State> child_states;
        State agent_state = agents[i].current_state;
       
        
        child_states = getChildStates(agent_state);
        vector<int> heuristics;
        vector<int> isVisibleForHeuristics;
        int min = 1000; //unused 1000
        int min_child_index = -1;
        int visitedFlag = 0;
        // cout << "************************" << endl;


        // for(int j = 0; j < 3; j++){
        //     for(int k = 0; k < 3; k++){
        //         if (j == 2 && k == 2) {
        //             cout << agents[i].current_state.configuration[j][k] << "]";
        //         } else {
        //             cout << agents[i].current_state.configuration[j][k] << " ";
        //         }
        //     }
        // }
        // cout << endl;
        // cout << "up avail: " << agent_state.up_avail << endl;
        // cout << "down avail: " << agent_state.down_avail << endl;
        // cout << "left avail: " << agent_state.left_avail << endl;
        // cout << "right avail: " << agent_state.right_avail << endl;
        // cout << "************************" << endl;
        for(int j = 0; j < int(child_states.size()); j++){
            // cout << "num child states: " << child_states.size() << endl;
            // cout << "------------------------" << endl;
            State child = child_states[j];
            vector<vector<int> > child_conf = child.configuration;
            if(checkIfVisited(child)){ //calculate h with rta*
                int hL;
                vector<State> child_states_of_child = getChildStates(child);
                vector<int> heuristic_array_of_childs;
                for(int k = 0; k < int(child_states_of_child.size()); k++){
                    State child_of_child = child_states_of_child[k];
                    vector<vector<int> > child_of_child_conf = child_of_child.configuration;
                    int h = getManhattanDistance(child_of_child_conf, goal_configuration);
                    heuristic_array_of_childs.push_back(h);
                }
                
                int temp_min;
                int temp_min_index = -1;
                for(int d = 0; d < heuristic_array_of_childs.size(); d++){
                    if(temp_min_index == -1){
                        temp_min = heuristic_array_of_childs[d];
                        temp_min_index = d;
                    } else {
                        if(temp_min >= heuristic_array_of_childs[d]){
                            temp_min = heuristic_array_of_childs[d];
                            temp_min_index = d;
                        }
                    }
                }
                int best_index = temp_min_index;
                int best = temp_min;
                temp_min_index = -1;
                for(int d = 0; d < heuristic_array_of_childs.size(); d++){
                    if(temp_min_index == -1 && d!=best_index){
                        temp_min = heuristic_array_of_childs[d];
                        temp_min_index = d;
                    } else {
                        if(temp_min >= heuristic_array_of_childs[d] && d!=best_index){
                            temp_min = heuristic_array_of_childs[d];
                            temp_min_index = d;
                        }
                    }
                }
                if(best == temp_min){
                    hL = 1000;
                } else hL = temp_min;
                // cout << "created with: " << char(child.createdWith) << endl;
                // cout << "hL: " << hL << endl;
                heuristics.push_back(hL);
                isVisibleForHeuristics.push_back(1);
            } else { // calculate h with lrta*
                int hG = getManhattanDistance(child_conf, goal_configuration);
                // cout << "created with: " << char(child.createdWith) << endl;
                // cout << "hG: " << hG << endl;
                heuristics.push_back(hG);
                isVisibleForHeuristics.push_back(0);
            }
        }
        for(int h_index = 0; h_index < heuristics.size(); h_index++){
            if(heuristics[h_index] < min){
                min = heuristics[h_index];
                min_child_index = h_index;
            }
        }
        vector<int> tie_list;
        for(int h_index = 0; h_index < heuristics.size(); h_index++){
            if(heuristics[h_index] == min){
                tie_list.push_back(h_index);
            }
        }
        int r;
        if(tie_list.size() > 1){
            r = rand() % tie_list.size();
            min_child_index = tie_list[r];
            min = heuristics[min_child_index];
        }
        if(isVisibleForHeuristics[min_child_index] == 0){
            agent_state.hG = min;
            agents[i].current_state = child_states[min_child_index];
            agents[i].last_move = child_states[min_child_index].createdWith;
            visited_states.push_back(child_states[min_child_index]);
            
        } else {
            agent_state.hG = min;
            agents[i].current_state = child_states[min_child_index];
            agents[i].last_move = child_states[min_child_index].createdWith;
        }
        if(isIdentical(child_states[min_child_index].configuration, goal_configuration)){
            goal_reached = true;
        }
        // printf("min for agent %d: %d\n", i, min);
    }
}


int main(){
    //@todo: set goal_found
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

    // for(int i = 0; i < lines.size(); i++){
    //     cout << lines[i] << endl;
    // }

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
        if(state.blank_index_row - 1 >= 0){
            state.up_avail = true;
        } else{ state.up_avail = false; }
        if(state.blank_index_row + 1 < 3){
            state.down_avail = true;
        } else{ state.down_avail = false; }
        if(state.blank_index_col - 1 >= 0){
            state.left_avail = true;
        } else{ state.left_avail = false; }
        if(state.blank_index_col + 1 < 3){
            state.right_avail = true;
        } else{ state.right_avail = false; }
        state.createdWith = 0;
        state.hG = getManhattanDistance(state.configuration, goal_configuration);
        if (!checkIfVisited(state)) visited_states.push_back(state); 
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