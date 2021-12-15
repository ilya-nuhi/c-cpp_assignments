/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * BLG 336E Analysis of Algorithms II
 * Spring 2021
 * Student Name: İlya Mustafa Nuhi
 * Student ID: 150180710
 * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    bool isBipartite(vector<vector<int> >& graph) {
        // Y O U R   C O D E   H E R E
        int N = graph.size();
        int *color_arr = new int[N];

        for (int i = 0; i < N; i++) // assigning all nodes colorless. -1 is colorless, 1 is red and 0 is blue.
            color_arr[i] = -1;

        color_arr[0] = 1;   // assigning source node's color to red

        queue <int> myqueue;    // using queue method in order to search for every node efficiently and safely
        myqueue.push(0);        // pushing starting node 0 to my queue

        while (!myqueue.empty()) {  // if myqueue is empty, then it means we have checked for all nodes and finish loop
            int node = myqueue.front(); // Taking the the foremost value as current node
            myqueue.pop();  // popping the searched node from queue

            for (unsigned int i = 0; i < graph[node].size(); i++) {  // if there is a self loop, it is not a bipartite graph.
                if (graph[node][i] == node)
                    return false;
            }

            for (unsigned int i = 0; i < graph[node].size(); i++) {
                if (color_arr[graph[node][i]] == -1)    // if searched node is not colored add the node to queue
                    myqueue.push(graph[node][i]);
                else if (color_arr[node] == color_arr[graph[node][i]])  // if searched node is colored and its color is same with its not bipartite
                    return false;
                color_arr[graph[node][i]] = 1 - color_arr[node];    // assign searched node's color different from its parent.    
            }

        }

        return true;    // if doesn't face with any return false that means its a bipartite graph, returns true.
    }
};

// This function is provided to check if your graph data is stored well or not
void printv(vector <vector <int> > v) {
    for (size_t i = 0; i < v.size(); i++) {
        for (size_t j = 0; j < v[i].size(); j++)
            cout << v[i][j] << " ";
        cout << endl;
    }
}

int main()
{
    // Solution class for bipartite-ness problem
    Solution s;

    // Read data filename from std input stream and open it with file handler/pointer
    string fname;
    cin >> fname;
    ifstream graphFileHandler(fname);

    // An array of integers to store neighbours of a vertex
    vector <int> adj;
    // A 2D array of integers to store neighbours of each vertex in a graph
    vector<vector <int> > graph;

    string line;
    // Read from the file until there is no new line left
    while (graphFileHandler >> line) {
        // Save line as string stream object
        stringstream ss(line);
        // Get a string variable
        string substr;

        // Until the end of the line, read the line as substings separated by commas
        while (ss.good()) {
            getline(ss, substr, ',');
            // Push the substring in adjacency list, we got a neighbour here
            adj.push_back(stoi(substr));
        }

        // When line ends, push all the neighbours of the vertex into the graph
        graph.push_back(adj);

        // Clear the array, before the next iteration begins
        // Next iteration, we will read the subsequent line from the file and it will contain neighbours of some other guy
        adj.clear();
    }

    // Check if data file is read in the way it is supposed to be
    // D O   N O T   P R I N T   T H E   G R A P H   I N   Y O U R   S O L U T I O N
    // The expected output only includes a True or a False not the graph data itself
    // Do not uncomment this line
    // printv(graph);

    // Save the return value of the function
    bool answer = s.isBipartite(graph);

    // If answer is True, print True otherwise False
    if (answer == 1)
        cout << "True" << endl;
    else
        cout << "False" << endl;

    return 0;
}
