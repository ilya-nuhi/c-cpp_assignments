/*
* Student Name:İlya Mustafa Nuhi
* Student Id:150180710
*
* q3_maxProfit_skeleton.cpp
*
* Created on: June 14th, 2021
*     Author: Uğur Önal
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <set>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;

pair<int, set<int>> MaxProfit(int numOfCrystals, vector<int> profits, vector<int> entryCosts)
{
	/****************************************************************/
	/********* YOU NEED TO USE HERE AS BASE OF YOUR METHOD! *********/
	/* YOU CAN USE HELPER METHODS BUT main() WILL CALL THIS METHOD! */
	/****************************************************************/

	int numOfCities = entryCosts.size();	
	set<int> citiesToVisit;	// creating integer set for visited cities
	int maxProfit = 0;	
	int** matrix;
	// creating memory space for 2d matrix array
	matrix = new int* [numOfCities + 1];
	for (int i = 0; i <= numOfCities; i++)
		matrix[i] = new int[numOfCrystals + 1];

	for (int j = 0; j <= numOfCrystals; j++) {	// filling first row of matrix with 0s
		matrix[0][j] = 0;
	}

	// filling matrix elements
	for (int i = 1; i <= numOfCities; i++) {
		for (int j = 0; j <= numOfCrystals; j++) {
			if (entryCosts[i - 1] > j)	// if entry cost of the current city is more than our crystals assign current element equal to its upper element
				matrix[i][j] = matrix[i - 1][j];

			/* else assign the element with max of the current element's 1 upper and entry cost of the current city
			times left element' value + profit of the current city or 1 upper element*/
			else {
				matrix[i][j] = max(matrix[i - 1][j], profits[i - 1] + matrix[i - 1][j - entryCosts[i - 1]]);
			}
		}
	}

	maxProfit = matrix[numOfCities][numOfCrystals];	// max profit is always at the right bottom corner element

	// drawing matrix
	cout << "Dynaming Programming Table" << endl;
	for (int i = 0; i <= numOfCities; i++) {
		for (int j = 0; j <= numOfCrystals; j++) {
			cout << std::right << std::setw(3) << matrix[i][j];
		}
		cout << endl;
	}

	int i = numOfCities, j = numOfCrystals;

	// starting from right bottom corner of the matrix, if current element is not equal to its upper element add the current city to citiesTovisit set
	while (i > 0 && j > 0) {	
		if (matrix[i][j] != matrix[i - 1][j]) {
			citiesToVisit.insert(i);
			j = j - entryCosts[i - 1];
		}
		i = i - 1;

	}

	return pair<int, set<int>>(maxProfit, citiesToVisit);
}

int main() {
	int numOfCrystals;
	vector<int> profits;
	vector<int> entryCosts;

	string inputFilename;
	cout << "Enter the name of the input file: ";
	cin >> inputFilename;

	ifstream input(inputFilename);

	if (!input.is_open()) {
		cerr << "File named \"" << inputFilename << "\" could not open!" << endl;
		return EXIT_FAILURE;
	}

	string line;
	if (getline(input, line)) {
		numOfCrystals = stoi(line);
	}
	while (getline(input, line, ' ')) {
		profits.push_back(stoi(line));
		getline(input, line);
		entryCosts.push_back(stoi(line));
	}

	pair<int, set<int>> result = MaxProfit(numOfCrystals, profits, entryCosts);

	cout << "Max profit is " << result.first << "." << endl;
	cout << "Cities visited:";
	for (int cityNumber : result.second) {
		cout << " " << cityNumber;
	}
	cout << endl;
}

