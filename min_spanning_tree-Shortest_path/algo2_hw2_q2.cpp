/*
Student Name: İlya Mustafa Nuhi
Student Id: 150180710

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#define MAX 999999999       // defining MAX to a big integer in order to use it for detecting not safe spots

using namespace std;




struct path {       // path struct

    string spot1;
    string spot2;
    int distance;
    int total_distance= MAX;    // initializing total_distance to MAX
    bool first_parent;          // if the used path's parent node is spot1 it will be true, otherwise false

};

class SP {      // 

    vector<path> paths_left;        // Stores the tunnel infos that left from the original one
    vector<string> used_spots;      // Stores the location strings that is used
    vector<path> path_order;        // Stores the used tunnels infos

public:



    SP(vector<path> path_plan) {     // First Constructor

        paths_left = path_plan;     //Storing path_plan info to paths_left

        int path_idx = 0;         //Storing searched path's index in paths_left
        int min_distance=MAX;

        for (unsigned int i = 0; i < paths_left.size(); i++) {                              // First construction, start with "Ma"
            if (paths_left[i].spot1 == "Ma" && paths_left[i].spot2.substr(0, 1) != "E") {   // Searching for "Ma" in spot1 and not enemy in spot 2
                
                paths_left[i].total_distance = paths_left[i].distance+con_helper(paths_left[i].spot2,i);  // assigning total distance of the current path, con_helper returns MAX if the spot 2 has an enemy spot connection with <5 distance, returns 0 if it is safe

                if (paths_left[i].total_distance<min_distance) {    // if the total distance of the current path is smaller than the min_distance
                    min_distance = paths_left[i].total_distance;    // assigning min distance to current path's total distance
                    paths_left[i].first_parent = true;              // spot1 is the mother spot for the initial edge
                    path_idx = i;                                   // in order to save the min distance path index

                }
            }
        }

        // pushing the selected path to path_order and deleting the selected path from paths_left 
        path_order.push_back(paths_left[path_idx]);
        used_spots.push_back(path_order[0].spot1);
        used_spots.push_back(path_order[0].spot2);
        paths_left[path_idx] = paths_left[paths_left.size() - 1];   //swaping with the last element of the vector in order to pop_back
        paths_left.pop_back();



        construct();        // calling second constructor

    }

    int con_helper(string parent,int i) {


        for (unsigned int j = 0; j < paths_left.size(); j++) {  // Searches the paths_left in order to find the safety of next child. If next child has <5 distance to the enemy spot then return MAX else return 0

                if ((paths_left[j].spot1 == parent|| paths_left[j].spot2 == parent)) {  // if one of the spots of the path is equal to parent of this function call
                   
                    if ((paths_left[j].spot1.substr(0, 1) == "E" || paths_left[j].spot2.substr(0, 1) == "E") && paths_left[j].distance < 5) {   // if the path has an enemy spot and its distance is <5 then it is not safe location for parent, return MAX
                      
                        return MAX;     
                    }
                    
                }

        }
        return 0;   // returns 0 if the spot is safe for parent

    }

    void construct() {  // Second Constructor


        int prev_size = 0;  

        while (true) {

            int path_idx = 0;               // storing the index of the current searching path

            int min_distance = MAX;         // initializing min_distance to MAX in order to compare with the first search

            int size = paths_left.size();   // assigning size to paths_left's size in order to compare it with prev_size

            if (prev_size == size) {        // if prev_size == size that means in previous cycle we couldn't find a possbile path means every possible path is already searched and found
                break;  
            }
            
            bool in = false;    // if in is true that means we found a possbile path

            for (unsigned int i = 0; i < paths_left.size(); i++) {      // search paths_left 

                if (paths_left[i].spot1.substr(0, 1) == "E" || paths_left[i].spot2.substr(0, 1) == "E") // Enemy spots won't be used
                    continue;
                if (find(used_spots.begin(), used_spots.end(), paths_left[i].spot1) != used_spots.end() && find(used_spots.begin(), used_spots.end(), paths_left[i].spot2) != used_spots.end()) // if two spots are already used, don't use this path
                    continue;
                else if (find(used_spots.begin(), used_spots.end(), paths_left[i].spot1) != used_spots.end() || find(used_spots.begin(), used_spots.end(), paths_left[i].spot2) != used_spots.end()) {  // if only one of the spots is used

                    if (find(used_spots.begin(), used_spots.end(), paths_left[i].spot2) != used_spots.end()) {      // if path_left[i]'s spot2 is a parent and spot1 is a child
                        paths_left[i].first_parent = false;                                                         // spot1 is not a parent so assign first_parent to false
                        int distance_sofar=0;                                                                       // initializing distance_sofar with 0, stores the distance taken so far                                                          
                        for (unsigned int p = 0; p < path_order.size(); p++) {                                      
                            if (path_order[p].first_parent) {                               // if first_parent is true, we are looking for spot2 as a child
                                if (path_order[p].spot2 == paths_left[i].spot2)             // if we find spot2 of the path in path_order equals to spot2 in paths_left we update the distance so far
                                    distance_sofar = path_order[p].total_distance;          // distance so far is equal to total distance of the current path
                            }
                            else {
                                if (path_order[p].spot1 == paths_left[i].spot2)             // if we find spot1 of the path in path_order equals to spot2 in paths_left we update the distance so far
                                    distance_sofar = path_order[p].total_distance;          // distance so far is equal to total distance of the current path
                            }
                        }
                        paths_left[i].total_distance=paths_left[i].distance+distance_sofar+ con_helper(paths_left[i].spot1, i); // Assigning current paths_left's total distance to sum of path's distance, distance so far and return value of con helper which is MAX if its not safe and 0 if it is safe
                    }
                    else {                                                                                           // if path_left[i]'s spot1 is a parent and spot2 is a child

                        paths_left[i].first_parent = true;                                                           // spot1 is a parent so first_parent is true
                        int distance_sofar=0;                                                                        // initializing distance_sofar with 0, stores the distance taken so far
                        for (unsigned int p = 0; p < path_order.size(); p++) {
                            if (path_order[p].first_parent) {                               // if first_parent is true, we are looking for spot2 as a child                    
                                if (path_order[p].spot2 == paths_left[i].spot1)             // if we find spot2 of the path in path_order equals to spot1 in paths_left we update the distance so far
                                    distance_sofar = path_order[p].total_distance;          // distance so far is equal to total distance of the current path
                            }
                            else {
                                if (path_order[p].spot1 == paths_left[i].spot1)             // if we find spot1 of the path in path_order equals to spot1 in paths_left we update the distance so far
                                    distance_sofar = path_order[p].total_distance;          // distance so far is equal to total distance of the current path
                            }
                        }
                        paths_left[i].total_distance = paths_left[i].distance + distance_sofar + con_helper(paths_left[i].spot2, i);    // Assigning current paths_left's total distance to sum of path's distance, distance so far and return value of con helper which is MAX if its not safe and 0 if it is safe
                    }

                    if (paths_left[i].total_distance < min_distance) {      // if the total distance of the current path is smaller than the min_distance
                        min_distance = paths_left[i].total_distance;        // assigning min distance to current path's total distance
                        path_idx = i;                                       //in order to save the min distance path index
                        in=true;                                            // if in is true that means we found a possbile path
                    }
                    
                }
            }

            
            prev_size = size;       // assigning prev_size to size in order to check if path_left's size is changed or not 

            // pushing the selected path to path_order and deleting the selected path from paths_left if in is true
            if (in) {
                if (paths_left[path_idx].first_parent)                  // if parent is the spot1 then push the child spot2 else push spot1 to the used spots
                    used_spots.push_back(paths_left[path_idx].spot2);
                else
                    used_spots.push_back(paths_left[path_idx].spot1);
                path_order.push_back(paths_left[path_idx]);
                paths_left[path_idx] = paths_left[paths_left.size() - 1];
                paths_left.pop_back();
            }
        }

    }

   int print_path(string destination,int d) {

        string parent;  // stores the next spot's name in order to send it in a function as a parameter
        int distance;   // stores the distance in order to send it in a function as a parameter
        if (destination == "Ma") {   // if destination is "Ma", means that we have reached the beginning spot, return the total distance and print the all destinations recursively by closing the outer recursive functions   
            cout << destination << " ";
            return d;
        }
        for (unsigned int i = 0; i < path_order.size(); i++) {  // Searches the path_order 

            if (path_order[i].first_parent) {   // if spot1 is a parent of the searching path

                if (path_order[i].spot2 == destination) {       // if destination is found in spot2 update the distance to total distance of the current path and set the parent as spot1 in order the use it in recursive function call
                    distance = path_order[i].total_distance;
                    parent = path_order[i].spot1;               // new parent is the parent of the last spot
                }
            }
            else {                              // if spot2 is a parent of the searching path

                if (path_order[i].spot1 == destination) {       // if destination is found in spot1 update the distance to total distance of the current path and set the parent as spot2 in order the use it in recursive function call
                    distance = path_order[i].total_distance;
                    parent = path_order[i].spot2;               // new parent is the parent of the last spot
                }
            }

            
        }

        if (distance < d) { // if distance < d set the distance to d in order to save the whole path's distance
            distance = d;
        }

        int result = print_path(parent,distance);   // returns the result of the distance and calls the recursive function
        cout << destination << " ";                 // prints the spot
        return result;                              // returns distance

    }


};



int main()
{
    /*string fname;

    cin >> fname;*/

    ifstream myfile("path_info_2.txt");     // opening file

    if (!myfile.is_open())
        cout << "Can't access the file!" << endl;

    vector<path> path_plan;     // creating path_plan vector which stores the path infos

    path current_path;          // for storing paths that read from file to vector
    string info;                // for storing strings that read from file

    while (getline(myfile, info, ',')) {       // Reading the path info from file and storing the information to vector of structs.

        current_path.spot1 = info;
        getline(myfile, info, ',');
        current_path.spot2 = info;
        getline(myfile, info);
        current_path.distance = stoi(info);

        path_plan.push_back(current_path);
    }

    SP my_tree(path_plan);  // Create Shortest path class
    
    cout << my_tree.print_path("Mo", 0) << endl;    // printing path and total distance, sending "Mo" and 0 as a parameters to the print_path function in order to get the result
    myfile.close();

    return 0;
}
