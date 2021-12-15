/*
Student Name: ilya Mustafa Nuhi
Student Id: 150180710
Please Compile with -std=c++11 
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

//constant score variables
#define MATCH 1
#define MISMATCH -2
#define GAP -4

using namespace std;

struct matrix {     // matrix struct 
    string word1;               // first letter
    string word2;               // second letter
    int** matrix_points=NULL;   // 2d array for comparing words
    vector<string> sequences;   // storing the seqeunces
    int score=0;                // max score
    matrix* next=NULL;          
};

class SW { // Smith-Waterman

    vector<string> sorted_words;    // sorted words

public:

    matrix* head;   // head pointer of matrix struct


    SW(vector <string> words) { // Constructor
        sorted_words = words;
        head = NULL;
    }


    void LSA() {    //Local Sequence Alignment
        
        // 2 for cycles in order to compare all possible combinations of 2 words
        for (unsigned int i = 0; i < sorted_words.size()-1; i++) {  
            for (unsigned int j = i; j < sorted_words.size()-1; j++) {
                matrix* temp = new matrix;  // creating memory for new matrix struct

                temp->word1 = sorted_words[i];  // assigning compared first word
                temp->word2 = sorted_words[j + 1];  // assigning compared second word
                temp->matrix_points = new int* [(int)sorted_words[i].size() + 1]; // creating memory for 2d array
                for (unsigned int k = 0; k < sorted_words[i].size() + 1; k++)   // creating memory for 2d array
                    temp->matrix_points[k] = new int[(int)sorted_words[j + 1].size() + 1];
                for (unsigned int p = 0; p < sorted_words[i].size() + 1; p++) { // initializing new 2d array with 0 values in order to not get warnings
                    for (unsigned int r = 0; r < sorted_words[j + 1].size() + 1; r++) {
                        temp->matrix_points[p][r] = 0;
                    }
                }
                
                for (unsigned int m = 0; m < sorted_words[i].size()+1; m++)     // assigning 0 to leftmost column
                    temp->matrix_points[m][0] = 0;
                for (unsigned int n = 0; n < sorted_words[j + 1].size() + 1; n++)   // assigning 0 to uppermost row
                    temp->matrix_points[0][n] = 0;
                // filling the 2d array with smith waterman algorithm with given match mismatch and gap values
                for (unsigned int p = 1; p < sorted_words[i].size() + 1; p++) { 
                    for (unsigned int r = 1; r < sorted_words[j + 1].size() + 1; r++) {
                        vector <int> values;    // creating int vector in order to store the value comes from upper left, left, up and 0 then chosing the max value among them to assign it in the current element in 2d array
                        // upper left value
                        if (sorted_words[j+1][r - 1] == sorted_words[i][p - 1])   // if current element's corresponding letters are same its a match so add MATCH value to upper left element, else add MISMATCH value
                            values.push_back(temp->matrix_points[p - 1][r - 1] + MATCH);
                        else
                            values.push_back(temp->matrix_points[p - 1][r - 1] + MISMATCH);
                        values.push_back(temp->matrix_points[p][r - 1] + GAP);      // left value   ,  add GAP value to left element of the current element
                        values.push_back(temp->matrix_points[p - 1][r] + GAP);      // up value     ,  add GAP value to upper element of the current element 
                        values.push_back(0);                                        // 0            ,  add 0 in order to not get negative values
                        temp->matrix_points[p][r]= *max_element(values.begin(), values.end());   // picking the highest value among 4 options and assigning it to the current element of 2 d array
                    }
                }

                // finding max score by searching 2d array elements
                for (unsigned int p = 1; p < sorted_words[i].size() + 1; p++) {
                    for (unsigned int r = 1; r < sorted_words[j + 1].size() + 1; r++) {
                        if (temp->matrix_points[p][r] > temp->score)
                            temp->score = temp->matrix_points[p][r];
                    }
                }

                // finding the sequences in 2d array by finding the max score element and tracing back to 0 diagonaly
                for (unsigned int p = 1; p < sorted_words[i].size() + 1; p++) {
                    for (unsigned int r = 1; r < sorted_words[j + 1].size() + 1; r++) {
                        if (temp->matrix_points[p][r] == temp->score) { // if current element's value is equal to max score value do the following
                            int temp_p = p;
                            int temp_r = r;
                            vector<char> letters;   // char vector in order to store letters
                            while (temp->matrix_points[temp_p][temp_r] != 0) {  // diagonally tracing back until 0
                                letters.push_back(sorted_words[i][temp_p-1]);   // pushing corresponding letters from highest value in 2d array to 0
                                temp_p--;
                                temp_r--;
                            }
                            reverse(letters.begin(), letters.end());    // reversing char vector in order to get the string in correct order
                            string common(letters.begin(), letters.end());  // creating string from char vector's elements
                            temp->sequences.push_back(common);  // pushing found sequence string 
                        }
                    }
                }

                if (head == NULL) { // if head equals NULL filled struct is assigned to head
                    head = temp;
                }
                else {  // if head is not NULL, filled struct is assigned to NULL pointer of the linkes list
                    matrix* iter = head;
                    // find the address of the iter where its next is NULL
                    while (iter->next != NULL) {    
                        iter = iter->next;
                    }
                    iter->next = temp;  // assign filled struct to iter->next which is NULL
                }

            }
        }
    }

    void print_sequences(){

        ofstream outfile;   // output file operations

        outfile.open("output.txt"); // creating output.txt file in order to write sequences and scores

        matrix* iter = head;    // creating iter pointer assigned to address of head to start printing from head
        while (iter != NULL) {  // printing until the NULL element
            sort(iter->sequences.begin(), iter->sequences.end());   // sorting sequences vector in alphabetical order
            iter->sequences.erase(unique(iter->sequences.begin(), iter->sequences.end()), iter->sequences.end());   // deleting the duplicates from sequence vector
            // writing the compared words' scores and sequences in asked format
            outfile << iter->word1 << " - " << iter->word2 << endl;
            outfile << "Score: " << iter->score << " Sequence(s):";
            if (iter->score != 0) { // if score is not 0 write the sequence
                for (unsigned int i = 0; i < iter->sequences.size(); i++) {
                    outfile << " \"" << iter->sequences[i] << "\"";
                }
            }
            outfile << endl;
            iter = iter->next;  // passing next matrix
        }

        outfile.close();    // closing output file
    }

    ~SW() { // Destructor
        matrix* temp;   // temporary matrix pointer
        while (head != NULL) {  // while head is not null keep deleting the created memory of matrix struct and its 2d arrays
            temp = head;    // assignin temp to head
            head = head->next;  // head is now at it's next 
            // deleting the 2d array
            for (unsigned int i = 0; i < temp->word1.size() + 1; i++) { 
                delete temp->matrix_points[i];
            }
            delete temp->matrix_points;
            delete temp;    // deleting memory of matrix struct
        }

    }

};


int main()
{
    ifstream myfile;    // input file operation
    myfile.open("strings.txt"); // opening input file
    if (!myfile.is_open()) {    // if file is not found give an error message
        cout << "Couldn't access file" << endl;
    }

    vector<string> words;   // vector for storing words from input file
    string line;    // in order to pull the words one by one
    while (getline(myfile,line)) {  // reading to the end of the file
        words.push_back(line);  // pushing word to the vector
    }

    myfile.close(); // closing input file

    sort(words.begin(), words.end());   // sorting words vector in alphabetical order

    SW obj(words);  // creating Sw object and sending words vector
    obj.LSA();  // calling Local Sequence Alignment function to implement algorithm
    obj.print_sequences();  // printing the found sequences between words and scores

    return 0;
}

