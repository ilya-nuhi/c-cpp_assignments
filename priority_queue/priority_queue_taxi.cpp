#include <iostream>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <fstream>
#include <string>
#include <math.h> 
#include<chrono>

using namespace std;

class Heap {                // My priority queue binary minheap data structure
private:
	vector <double> heap;   // Heap array
	int left_child(int);    // Left child node
	int right_child(int);   // Right child node
	int parent(int);        // Parent node
	void heapifyup(int);    // This function is not used in public because its just used in heapifyup and Insert functions
	void heapifydown(int);  // This function is not used in public because its just used in heapifydown and DeleteMin functions
public:
	Heap() {}				// Constructor
	void Insert(double);    // Insert the new taxi's distance to the heap vector and arranges new distance's location in the PQ heap
	void DeleteMin();       // Closest taxi is called. So deletes the min distance of the heap and rearrange the PQ heap
	int Size();             // Returns the size of the heap vector
	void new_distance();	// Decreases random taxi distance by 0.01
};


int Heap::Size() {										// Returns the size of the heap vector
	return heap.size();
}
void Heap::Insert(double new_element) {					// Insert the new taxi's distance to the heap vector and arranges new distance's location in the PQ heap
	heap.push_back(new_element);						// Push the new element to back
	heapifyup(heap.size() - 1);							// Places the new element to its right place in the tree
}
void Heap::DeleteMin() {								// Closest taxi is called. So deletes the min distance of the heap and rearrange the PQ heap
	if (heap.size() == 0) {								// I already provide not calling this function if the heap is empty but still in case...
		cout << "Heap is Empty" << endl;
		return;
	}
	cout << "Called taxi's distance is: " << heap[0] << endl;
	heap[0] = heap[heap.size() - 1];					// I assign the last index of the vector to the 0 index because i will pop_back after and don't wanna lose any value
	heap.pop_back();									
	heapifydown(0);										// Places the value of the index 0 to its right location
}
int Heap::left_child(int parent) {						// Returns the index of the left child's node
	int left = 2 * parent + 1;
	if (left < heap.size())
		return left;
	else
		return -1;
}
int Heap::right_child(int parent) {						// Returns the index of the right child's node
	int right = 2 * parent + 2;
	if (right < heap.size())
		return right;
	else
		return -1;
}
int Heap::parent(int child) {							// Returns the index of the parent node
	int par = (child - 1) / 2;
	if (child == 0)
		return -1;
	else
		return par;
}
void Heap::heapifyup(int index) {													// Compares the asked index with its parents until it finds its right place, if parent is bigger it swaps with its current parent
	if (index >= 0 && parent(index) >= 0 && heap[parent(index)] > heap[index]) {	// It works recursively to the top of the tree until it reaches top or find its right place
		double temp = heap[index];							
		heap[index] = heap[parent(index)];
		heap[parent(index)] = temp;
		heapifyup(parent(index));													// To work recursively
	}
}
void Heap::heapifydown(int index) {													// Compares the asked index with its smallest child until it finds its right place, it swaps with its smallest child
	int childleft = left_child(index);
	int childright = right_child(index);
	if (childleft >= 0 && childright >= 0 && heap[childleft] > heap[childright]) {	// Finds which child is smaller
		childleft = childright;
	}
	if (childleft > 0 && heap[index] > heap[childleft]) {							// It works	recursively to the bottom of the tree until it reaches the bottom or find its right place	
		double t = heap[index];
		heap[index] = heap[childleft];
		heap[childleft] = t;
		heapifydown(childleft);														// To work recursively
	}	
}

void Heap::new_distance() {															// Decreases random taxi distance by 0.01
	int random = rand() % heap.size();
	heap[random] -= 0.01;
	if (heap[random] < 0) heap[random] = 0;											// in oder to prevent the distance value go to negative numbers.
	heapifyup(random);
}

int main(int argc, char** argv) {										// main function

	Heap pqheap;														// Creating heap class

	ifstream file;
	file.open("locations.txt");


	if (!file) {
		cerr << "File cannot be opened!";
		exit(1);
	}

	string line;

	getline(file, line);												// in order to read the first header line

	int m = stoi(argv[1]);												// getting the operation number
	double p = stod(argv[2]);											// getting the probability
	//int m = 1000;
	//double p = 0.2;


	double longitude, latitude, distance;

	int taxi = 0;														// for counting added taxi numbers
	int update = 0;														// for counting how many distance updates have been made

	auto start = chrono::high_resolution_clock::now();					// for counting the running time of PQ in milliseconds.

	for (int i = 1; i <= m; i++) {

		double random = rand() / (double)RAND_MAX;						// generating random double type number between 0 and 1 

		/* if the heap is not empty and our random number is equal or smaller than p
		it updates the random taxi distance in our heap*/
		if ((random <= p ) && (pqheap.Size() != 0)) {
			pqheap.new_distance();
			update += 1;												// increments the number of update operations have made
		}

		// This statement is for if our heap is empty and random number is equal or smaller than p
		else if (random <= p ) {
			continue;
		}

		// If our random number is greater than p it reads the taxi info, calculates the distance and store the distance to the heap
		else {
			file >> longitude >> latitude;
			distance = sqrt(((longitude - 33.40819) * (longitude - 33.40819)) + ((latitude - 39.19001) * (latitude - 39.19001)));
			pqheap.Insert(distance);
			taxi += 1;
		}

		// After every 100 operations closes taxi is called and removed from the heap
		if ((i % 100 == 0) && i > 0 && pqheap.Size() > 0) {
			pqheap.DeleteMin();
		}
	}
	cout << "Total added taxi number:" << taxi << endl;
	cout << "Total update operation number:" << update << endl;

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	cout << "Priority Queue Algorithm takes " << elapsed.count() << " seconds" << " for " << m << " operations. " << endl;	//prints the time taken by PQ algorithm   

	return 0;
}