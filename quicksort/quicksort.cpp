#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<stdio.h>
#include<vector>
#include<chrono>

using namespace std;

struct data {		// Data struct
	string country;
	string type;
	string orderid;
	int units_sold;
	double profit;
};

 
void swap(vector <struct data>& array,int a, int b)	// Swaps the struct array with index a with index b
{
	struct data tempt = array[a];	// Saves the content in array[a]
	array[a]= array[b];				// Changes the content in a to b
	array[b] = tempt;				// Changes the content in b to tempt
}


/* Takes the last element of the struct array as a pivot and compares 
from first element to (N-1)th element because we already chose the last element
as a pivot. In the first if condition it compares the country names starting from
the first element and if that element's country name's alphabetical order
is smaller than our pivot's country name it swaps the element with i'th place. 
i is starting from the leftmost index and it increases with every swap operation.
Else if country names are same it checks for the profit and if the compared
element's profit is bigger than pivot it swaps the element with i'th place.
At the last step we places our pivot to its right place. 
*/
int partition(vector <struct data>& arr, int low, int high)		
{
	struct data pivot = arr[high];    // pivot element
	int i = (low - 1);  // Index of where swapped element goes to 

	for (int j = low; j <= high - 1; j++)	//Checks from low to high-1 included because our highest element is pivot
	{
		if (arr[j].country < pivot.country)	//Compares the country names in alphabetical order
		{
			i++;    // incrementing i because we will do swapping operation 
			swap(arr,i,j);	// Swapping operation
		}
		else if (arr[j].country == pivot.country && arr[j].profit > pivot.profit) {	//Compares the profits if the country names are same
			i++;	// incrementing i because we will do swapping operation
			swap(arr, i, j);	//Swapping operation
		}
	}
	swap(arr,i+1,high);	// Swapping our pivot to its right position
	return (i + 1); // returns to pivot element's index
}

/* Quicksort function itself.*/
void quickSort(vector <struct data>& arr, int low, int high)
{
	if (low < high)	// In order to perform recursive quicksort operations until the length of the smaller array is 1.
	{
		int pi = partition(arr, low, high);	// Perform the partition operation and pi is the pivot index.
 
		quickSort(arr, low, pi - 1);	//Recursive function that does the quicksort operation for the Left array
		quickSort(arr, pi + 1, high);	//Recursive function that does the quicksort operation for the Right array
	}
}

int main(int argc, char** argv) {
	
			ifstream file;
			file.open("sales.txt");

			if (!file) {
				cerr << "File cannot be opened!";
				exit(1);
			}

			int N = stoi(argv[1]);	//you should read value of N from command line

			vector <struct data> data_array(N);


			string line;

			getline(file, line); //this is the header line


			for (int i = 0; i < N; i++) {	//While it gets the data from file it saves the data in data struct

				getline(file, line, '\t'); //country (string)
				data_array[i].country = line;

				getline(file, line, '\t'); //item type (string)
				data_array[i].type = line;

				getline(file, line, '\t'); //order id (string)
				data_array[i].orderid = line;

				file >> line; //units sold (integer)
				data_array[i].units_sold = stoi(line);

				file >> line; //total profit (double) 
				data_array[i].profit = stod(line);

				getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
			}


			auto start = chrono::high_resolution_clock::now();
			quickSort(data_array, 0, N - 1);

			auto finish = chrono::high_resolution_clock::now();
			chrono::duration<double> elapsed = finish - start;
			cout << "Quicksort algorithm takes " << elapsed.count() << " seconds" << " for " << N << " elements " << endl;	//prints the time taken by quicksort algorithm

			//Writes the sorted data to the output file
			ofstream file2;
			file2.open("output.txt");

			for (int i = 0; i < N; i++) {
				file2 << left << setw(33) << data_array[i].country << "\t";
				file2 << left << setw(15) << data_array[i].type << "\t";
				file2 << left << setw(10) << data_array[i].orderid << "\t";
				file2 << left << setw(6) << data_array[i].units_sold << "\t";
				file2 << left << setprecision(11) << data_array[i].profit << endl;
			}
			file2.close();
			file.close();
	return 0;
}
