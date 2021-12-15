/*
Student Name: İlya Mustafa Nuhi 
Student Id: 150180710

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;




struct tunnel{          // Tunnel struct

    string location1;   
    string location2;
    int distance;

};

class MST {             // Minimum spanning tree

    vector<tunnel> tunnels_left;        // Stores the tunnel infos that left from the original one
    vector<string> used_locations;      // Stores the location strings that is used
    vector<tunnel> tunnel_order;        // Stores the used tunnels infos

public:



    MST(vector<tunnel> city_plan) {     // First Constructor
        
        tunnels_left = city_plan;

        int tunnel_idx=0;               // Storing searched tunnel's index in tunnels_left
        bool tunnel_empty = true;
        for (unsigned int i = 0; i < tunnels_left.size(); i++) {                                                     // First construction, direct connection between GP and Church
            if (tunnels_left[i].location1 == "GP" && tunnels_left[i].location2.substr(0, 2) == "Ch") {
                if (tunnel_empty) {
                    tunnel_idx=i;           // Storing the tunnels index
                    tunnel_empty = false;
                }
                else {
                    if (tunnels_left[tunnel_idx].distance > tunnels_left[i].distance)          // finding the min distance tunnel index
                        tunnel_idx=i;
                }
            }
        }
        

        // pushing the selected tunnel to tunnel_order and deleting the selected tunnel from tunnels_left 
        tunnel_order.push_back(tunnels_left[tunnel_idx]);           
        used_locations.push_back(tunnel_order[0].location1);
        used_locations.push_back(tunnel_order[0].location2);
        tunnels_left[tunnel_idx] = tunnels_left[tunnels_left.size() - 1];   //swaping with the last element of the vector in order to pop_back
        tunnels_left.pop_back();
 
        
        for (unsigned int i = 0; i < tunnels_left.size(); i++) {                                                     // Second construction, direct connection between Hipp and GP
            if (tunnels_left[i].location1 == "GP" && tunnels_left[i].location2 == "Hipp") {
                tunnel_idx = i;
            }
        }

        // pushing the selected tunnel to tunnel_order and deleting the selected tunnel from tunnels_left 
        tunnel_order.push_back(tunnels_left[tunnel_idx]);
        used_locations.push_back(tunnel_order[1].location2);
        tunnels_left[tunnel_idx] = tunnels_left[tunnels_left.size() - 1];   //swaping with the last element of the vector in order to pop_back
        tunnels_left.pop_back();
        

        construct();        // calling second constructor
        
    }

    void construct() {  // Second Constructor

        bool construct_tunnel = true;   // while loop ending condition

        while (construct_tunnel) {  // while loop

            construct_tunnel = false;   
            int tunnel_idx = 0;     
            bool tunnel_empty = true;   

            for (unsigned int i = 0; i < tunnels_left.size(); i++) {
                if (tunnels_left[i].location1 == "Hipp" && tunnels_left[i].location2.substr(0, 3) == "Bas") // The Hippodrom should not be directly connected to any basilicas.
                    continue;
                if (tunnels_left[i].location1.substr(0, 2) == "Hp" && tunnels_left[i].location2.substr(0, 2) == "Hp")   //There should not be any direct connections between the houses of two improtant peoples.
                    continue;
                
                if (find(used_locations.begin(), used_locations.end(), tunnels_left[i].location1) != used_locations.end() && find(used_locations.begin(), used_locations.end(), tunnels_left[i].location2) != used_locations.end()) // if two locations are already used, don't use this tunnel
                    continue;
                else if (find(used_locations.begin(), used_locations.end(), tunnels_left[i].location1) != used_locations.end() || find(used_locations.begin(), used_locations.end(), tunnels_left[i].location2) != used_locations.end()) {  // if only one location is added to used_location and the other location i not addded means that it is a possible tunnel
                    
                    construct_tunnel = true;

                    if (tunnel_empty) {
                        tunnel_idx = i;           // Storing the tunnels index
                        tunnel_empty = false;
                    }
                    else {
                        if (tunnels_left[tunnel_idx].distance > tunnels_left[i].distance)   // finding the min distance tunnel index
                            tunnel_idx = i;
                    }    
                }
            }

            // if constructing will happen then push push the selected tunnel to tunnel_order and delete the selected tunnel from tunnels_left  
            if (construct_tunnel) {
                if(find(used_locations.begin(), used_locations.end(), tunnels_left[tunnel_idx].location1) != used_locations.end())
                    used_locations.push_back(tunnels_left[tunnel_idx].location2);
                else if(find(used_locations.begin(), used_locations.end(), tunnels_left[tunnel_idx].location2) != used_locations.end())
                    used_locations.push_back(tunnels_left[tunnel_idx].location1);
                tunnel_order.push_back(tunnels_left[tunnel_idx]);
                tunnels_left[tunnel_idx] = tunnels_left[tunnels_left.size() - 1];   //swaping with the last element of the vector in order to pop_back
                tunnels_left.pop_back();
            }
        }

    }

    void print_tunnels() {

        int size = tunnel_order.size();

        // sorting the tunnel order in increasing order of distances
        for (int i = 1; i < size; i++) {
            for (int j = 1; j < size; j++) {
                if (tunnel_order[j].distance < tunnel_order[j - 1].distance) {
                    tunnel temp = tunnel_order[j];
                    tunnel_order[j] = tunnel_order[j - 1];
                    tunnel_order[j - 1] = temp;
                }
            }
        }
        int total = 0;

        //Calculating the total distance, printing the tunnels and total distance
        for (unsigned int i = 0; i < tunnel_order.size(); i++) {
            cout << tunnel_order[i].location1 << " " << tunnel_order[i].location2 << " " << tunnel_order[i].distance << endl;
            total += tunnel_order[i].distance;
        }
        cout << total << endl;
    }


};



int main()
{
    string fname;

    cin >> fname;

    ifstream myfile(fname);     // opening file

    if (!myfile.is_open())
        cout << "Can't access the file!" << endl;

    vector<tunnel> city_plan;               // creating city_plan vector in order to store the tunnel infos    

    tunnel current_tunnel;       // for storing tunnels that read from file to vector
    string info;                 // for storing strings that read from file

    while (getline(myfile, info , ',')) {       // Reading the tunnel info from file and storing the information to vector of structs.

        current_tunnel.location1 = info;
        getline(myfile, info, ',');
        current_tunnel.location2 = info;
        getline(myfile, info);
        current_tunnel.distance = stoi(info);

        city_plan.push_back(current_tunnel);
    }

    MST my_tree(city_plan);     // create tree
    my_tree.print_tunnels();    // print tunnels in order

    myfile.close();

    return 0;
}
