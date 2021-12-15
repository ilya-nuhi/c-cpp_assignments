/***********************************************************
2021 Spring - BLG 336E-Analysis of Algorithms II
Final Project
Question on Greedy Algorithms
Modified Dijkstra Algorithms for Maximum Capacity Path
Submitted: 15.06.2021
**********************************************************/

/***********************************************************
STUDENT INFORMATION
Full Name :İlya Mustafa Nuhi
Student ID:150180710
**********************************************************/

// Some of the libraries you may need have already been included.
// If you need additional libraries, feel free to add them
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

// Do not change this definition
#define INT_MAX 1000


using namespace std;

class Graph {
public:
	int node_count;
	int edge_count;
	int** adjacency_matrix;

	Graph() {};
	void read_file(char* filename);
	void print_adjacency_matrix(); // in case you need
	int weight(int i, int j) { return this->adjacency_matrix[i][j]; }
	~Graph();
};

Graph::~Graph() {
	for (int i = 0; i < node_count; i++) {
		delete[] adjacency_matrix[i];
	}
}

void Graph::print_adjacency_matrix() {
	// Prints the adjacency matrix
	for (int i = 0; i < this->node_count; i++) {
		for (int j = 0; j < this->node_count; j++) {
			cout << this->adjacency_matrix[i][j] << ", ";
		}
		cout << endl;
	}
}

void Graph::read_file(char* filename) {

	// opening input file
	ifstream infile;
	infile.open(filename);
	string line;
	infile >> line;
	node_count = stoi(line);	// first number is node number

	vector <pair<pair<int, int>, int>> data;	// 2 nested pairs, first pair's first is source node; first pair's second is destination node and second pair is the weight
	
	// storing input file data to our vector
	while (!infile.eof()) {
		int source, destination, weight;
		infile >> line;
		source = stoi(line);
		infile >> line;
		destination = stoi(line);
		infile >> line;
		weight = stoi(line);
		data.push_back(make_pair(make_pair(source, destination), weight));
	}
	infile.close();	// closing file after read operations

	edge_count = data.size();	// edge count is equal to number of elements in vector

	adjacency_matrix = new int*[node_count];	// creating space for nxn matrix
	for (int i = 0; i < node_count; i++)
		adjacency_matrix[i] = new int[node_count];
	
	for (int i = 0; i < node_count; i++) {			// filling matrix with weight values of corresponding nodes
		for (int j = 0; j < node_count; j++) {
			bool placed = false;
			for (unsigned int k = 0; k < data.size(); k++) {
				// since its a undirected graph x,y and y,x should show the same result
				if ((i == int(data[k].first.first) && j== int(data[k].first.second))||(j == int(data[k].first.first) && i == int(data[k].first.second))) {	
					adjacency_matrix[i][j] = data[k].second;
					placed = true;
				}
			}
			if (!placed)	// assigning other matrix elements to 0
				adjacency_matrix[i][j] = 0;
		}
	}

	/*********************************************/
	/****** CODE HERE TO READ THE TEXT FILE ******/
	/*********************************************/
}

void Modified_Dijkstra(Graph* graph) {

	/*********************************************/
	/****** CODE HERE TO FOR THE ALGORITHM *******/
	/*********************************************/

	// creating memory space size of node_count for status wt and dad arrays
	int* status = new int[graph->node_count];
	int* wt=new int[graph->node_count];
	int* dad = new int[graph->node_count];

	// first filling status array with 0s; wt and dad arrays with -1. 
	for (int i = 0; i < graph->node_count; i++) {
		status[i] = 0;
		wt[i] = -1;
		dad[i] = -1;
	}

	status[0] = 2;	// assigning source node's status as 2 then we will check for it's neigbors
	wt[0] = INT_MAX;	// assigning weight of the node 0's weight to INT_MAX

	for (int i = 1; i < graph->node_count; i++) {
		if (graph->adjacency_matrix[0][i] != 0) {
			status[i] = 1;
			wt[i] = graph->adjacency_matrix[0][i];
			dad[i] = 0;
		}
	}

	bool fringes = true;	// fringes is true if all the status are not 2
	while (fringes) {		// implement the algorithm until all nodes' status values are 2 
		
		// find the maximum weighted node with status value 1 and assign it to v
		int v = 0;
		for (int j = 1; j < graph->node_count; j++) {
			if (status[j]==1) {
				if (v == 0)
					v = j;
				else if (wt[j] > wt[v])
					v = j;
			}
		}

		status[v] = 2;	// since we are in the v node we can assign it's status value as 2
		
		// finding node v's neighbors and assigning and updateing neighbor values for dad and wt arrays
		for (int k = 1; k < graph->node_count; k++) {
			if (graph->adjacency_matrix[v][k] != 0) {
				// if searched node's status value is 0, assign the wt and dad values
				if (status[k] == 0) {	
					status[k] = 1;
					wt[k] = min(wt[v], graph->adjacency_matrix[v][k]);
					dad[k] = v;
				}
				// if searched node's status value is 1 and this node's weight value is smaller than the source node's weight or path, update the wt and dad
				else if (status[k] == 1 && (wt[k] < min(wt[v], graph->adjacency_matrix[v][k]))) {	
					wt[k] = min(wt[v], graph->adjacency_matrix[v][k]);
					dad[k] = v;
				}
			}
		}

		fringes = false;	// assign fringes as false
		for (int i = 0; i < graph->node_count; i++) {	// if there are any status value that is not 2 make fringes as true
			if (status[i] != 2)
				fringes = true;
		}
	}
	

	/*********************************************/
	/***** DO NOT CHANGE THE FOLLOWING LINES *****/
	/**** THEY PRINT OUT THE EXPECTED RESULTS ****/
	/*********************************************/

	// The following line prints wt array (or vector).
	// Do not change anything in the following lines.
	cout << "###########RESULTS###########" << endl;
	cout << endl;

	cout << "1. WT ARRAY" << endl;
	cout << "------------------------" << endl;
	cout << "  ";
	for (int i = 0; i < graph->node_count - 1; i++) {
		cout << wt[i] << ", ";
	}
	cout << wt[graph->node_count - 1] << endl;

	// The following lines print the final path.
	// Do not change anything in the following lines.
	int iterator = graph->node_count - 1;
	vector<int> path_info;
	path_info.push_back(iterator);
	while (iterator != 0) {
		path_info.push_back(dad[iterator]);
		iterator = dad[iterator];
	}
	cout << endl;
	cout << "2. MAXIMUM CAPACITY PATH" << endl;
	cout << "------------------------" << endl;
	cout << "  ";
	vector<int>::iterator it;
	for (it = path_info.end() - 1; it > path_info.begin(); it--)
		cout << *it << " -> ";
	cout << *path_info.begin() << endl;

	cout << endl;
	cout << "3. MAXIMUM CAPACITY" << endl;
	cout << "------------------------" << endl;
	cout << "  ";
	cout << wt[graph->node_count - 1] << endl;
	cout << "#############################" << endl;

	return;
}

int main(int argc, char** argv) {
	Graph* graph = new Graph();
	graph->read_file(argv[1]);
	Modified_Dijkstra(graph);

	return 0;
}
