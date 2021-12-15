/*
Student Name: İlya Mustafa Nuhi
Student Id:   150180710
Submissioon Date: 6.4.2021
*/



#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>


using namespace std;

class node; // initializing the node Class
class Tree; // initializing the Tree Class

class queue_node {     // Node class for queue

public:

    node* nodeptr;
    queue_node* next;

    queue_node() {
        nodeptr = NULL;
        next = NULL;
    }

};

class my_queue {        // My implementation of queue in order to do operations in BFS search

public:

    queue_node* back;

    queue_node* front;

    my_queue()          // constructor
    {
        front = NULL;
        back = NULL;
    }

    bool empty() {      // checks if the queue is empty
        if (front == NULL)
            return true;
        else
            return false;
    }

    void pushback(node* newnode) {      // pushes the new added element to the end of the queue
        queue_node* temp = new queue_node();
        temp->nodeptr = newnode;
        if (empty()) {
            front = temp;
            back = temp;
        }
        else {
            back->next = temp;
            back = temp;
        }
        back->next = NULL;
    }

    void popfront() {          // pops the first element in the queue
        queue_node* temp;

        temp = front;
        front = front->next;
        delete temp;
    }

    ~my_queue() {               //Destructor
        queue_node* temp;

        while (front !=NULL) {
            temp = front;
            front = front->next;
            delete temp;
            
        }
    }

};

class node {        // node class for the nodes of Tree class

    size_t s;       // Discrete letter number

public:
    

    vector<int> used_numbs;     // Storing used indexes
    bool** matrix;              // pointer pointer of bool, in order to create matrix
    node* child[10];            // child pointer array of nodes
    node* parent;               // parent pointer

    node(int size) {            // Constructor of node
        s = size;
        parent = NULL;
        for (int i = 0; i < 10; i++)
            child[i] = NULL;            // assigning its childs to NULL
        matrix = new bool* [size];
        for (int j = 0; j < size; j++)
            matrix[j] = new bool[10];   // creating 10 bool type
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < 10; j++)
                matrix[i][j] = 0;       // filling the matrix with 0's
        }
    }

    ~node() {                               // Destructor of node
        for (int i = 0; i < s; i++) {
            delete matrix[i];
        }
        delete matrix;
    }

};



class Tree {

public:
    int node_visited = 0;   // keep the number of visited nodes in search algorithms
    int node_created = 0;   // keep the number of created nodes for tree
    node* root;             // root pointer of node

    Tree(vector<char> letters, int child_number, int child_index) {          // Constructor of the Tree
        root = new node(letters.size());
        createTree(root, letters.size(), 0, child_number);

    };

    void createTree(node* temp, int size, int child_index, int child_number) {
        if (temp->parent != NULL) {
            node_created++;     // incrementing the created_node 
            for (int i = 0; i < size; i++) {      // Copying the parent's matrix to child
                for (int j = 0; j < 10; j++)
                    temp->matrix[i][j] = temp->parent->matrix[i][j];
            }
            temp->used_numbs = temp->parent->used_numbs;    // Copying the used_numbs vector to its child

            int index = 0;
            vector<int> used_numbs_btween_brothers;
            for (int p = 0; p < child_index; p++)
                used_numbs_btween_brothers.push_back(temp->parent->child[p]->used_numbs.back());  // Storing the used indexes by the other childs

            vector<int>::iterator it1;
            vector<int>::iterator it2;

            do {
                it1 = find(used_numbs_btween_brothers.begin(), used_numbs_btween_brothers.end(), index);
                it2 = find(temp->used_numbs.begin(), temp->used_numbs.end(), index);
                index++;
            } while (it1 != used_numbs_btween_brothers.end() || it2 != temp->used_numbs.end());     // if the number is not used by the parent nodes or other child nodes its ok
            index--;

            temp->matrix[temp->used_numbs.size()][index] = 1;
            temp->used_numbs.push_back(index);
        }

        if (temp->used_numbs.size() < size) {           // Stop condition for recursive constructing
            for (int i = 0; i < child_number; i++) {    // iterates number of current node's child number
                
                temp->child[i] = new node(size);        // creating new child node
                temp->child[i]->parent = temp;          // linking child with its parent     
                createTree(temp->child[i],size,i,child_number-1);   // sending i as a child index and childnumber-1 as childnumber because every child has 1 less child than its parent

            }
        }

    }
    
    node* DFS(char* number1, char* number2, char* result, vector<char> letters) {
        node* answer;
        answer = DFS_helper(number1, number2, result, letters, root, 10, 0);    // Calling the helper function because we have to send root, child_numb and child_index
        if (answer == NULL) {                                                   // if DFS_helper function returns NULL, then puzzle has no solution
            cout << "There is no solution for this puzzle" << endl;
            exit(0);
        }
        else
            return answer;         // returns answer node

    }

    node* DFS_helper(char* number1, char* number2, char* result, vector<char> letters, node* temp, int child_numb, int child_index) {
        
        node_visited++;

        node *tmp=NULL;

        vector<char>::iterator it;

        if (temp->child[0] == NULL) {                               // if we are at the leaves, means that our matrix is filled, we can check the matrix result if it fits
            int numb1 = 0;
            int numb2 = 0;
            int sum = 0;

            int big_num_size;
            if (strlen(number1) >= strlen(number2)) {
                big_num_size = strlen(number1);
            }
            else
                big_num_size = strlen(number2);

            for (int i = 0; i < big_num_size; i++) {            // in order to find longest adding number
                int numeric1 = 0;
                int numeric2 = 0;
                int numeric3 = 0;
                if (big_num_size > strlen(number1) && i==big_num_size-1) {      // if number 2 is longer than number 1, took the most significant number as 0 for number 1 
                    numeric1 = 0;
                }
                else{
                    it = find(letters.begin(), letters.end(), number1[strlen(number1) - i - 1]);    // in order to find first number's decimal value
                    int index_num1 = distance(letters.begin(), it);

                    
                    while (temp->matrix[index_num1][numeric1] != 1)
                        numeric1++;
                }

                if (big_num_size > strlen(number2) && i == big_num_size-1) {        // if number 1 is longer than number 2, took the most significant number as 0 for number 2
                    numeric2 = 0;
                }
                else{
                    it = find(letters.begin(), letters.end(), number2[strlen(number2) - i - 1]);    // in order to find second number's decimal value
                    int index_num2 = distance(letters.begin(), it);

                    
                    while (temp->matrix[index_num2][numeric2] != 1)
                        numeric2++;
                }

                it = find(letters.begin(), letters.end(), result[strlen(result) - i - 1]);     // in order to find result number's decimal value
                int index_num3 = distance(letters.begin(), it);

                
                while (temp->matrix[index_num3][numeric3] != 1)
                    numeric3++;

                numb1 += numeric1 * pow(10, i);     // calculating the decimal value of the corresponding letter number on matrix
                numb2 += numeric2 * pow(10, i);     // calculating the decimal value of the corresponding letter number on matrix
                sum += numeric3 * pow(10, i);       // calculating the decimal value of the corresponding letter number on matrix
            }

            sum += pow(10, big_num_size);        // left most 1 is added because first letter of the result is always 1

            if ((numb1 + numb2) == sum)             // if the numbers match correctly returns the node
                return temp;
            else
                return NULL;                        // if it doesn't match returns NULL and goes to outer recursive function
        }

        for (int i = 0; i < child_numb; i++) {
            
            // finding the index of the first letters of every number in the letter vector
            it = find(letters.begin(), letters.end(), result[0]);
            int index1 = distance(letters.begin(), it);             // finding the indexes of given letter on the matrix table
            it = find(letters.begin(), letters.end(), number2[0]);
            int index2 = distance(letters.begin(), it);             // finding the indexes of given letter on the matrix table
            it = find(letters.begin(), letters.end(), number1[0]);
            int index3 = distance(letters.begin(), it);             // finding the indexes of given letter on the matrix table

            if (temp->matrix[index1][0] == 1 || temp->matrix[index2][0] == 1 || temp->matrix[index3][0] == 1)       // if any number starts with zero, returns NULL and goes to outer recursive function 
                return NULL;

            bool is_filled = false;                                     // in order to check if the first letter of result's corresponding matrix row is filled or not

            for (int i = 0; i < 10; i++) {                              
                if (temp->matrix[index1][i] == 1)
                    is_filled = true;
            }

            if (is_filled == true && temp->matrix[index1][1] != 1)      // first letter of the result should be 1, otherwise returns NULL and goes to outer recursive function
                return NULL;

            tmp = DFS_helper(number1, number2, result, letters, temp->child[i], child_numb - 1, i);     // recursive function is called and assigned to tmp

            if (tmp != NULL) {  // if answer is found, return the tmp
                return tmp;
            }
        }
    
        return NULL;    // if it finishes the for loop and can't find the solution
    }

    node* BFS(char* number1, char* number2, char* result, vector<char> letters) {
        node* answer;
        answer = BFS_helper(number1, number2, result, letters, root);           // Calling the helper function because we have to send root
        if (answer == NULL) {                                                   // if DFS_helper function returns NULL, then puzzle has no solution
            cout << "There is no solution for this puzzle" << endl;
            exit(0);
        }
        else
            return answer;          // returns answer node

    }

    node* BFS_helper(char* number1, char* number2, char* result, vector<char> letters, node* temp) {
        
        vector<char>::iterator it;

        // finding the index of the first letters of every number in the letter vector
        it = find(letters.begin(), letters.end(), result[0]);
        int index1 = distance(letters.begin(), it);     // finding the indexes of given letter on the matrix table
        it = find(letters.begin(), letters.end(), number2[0]);
        int index2 = distance(letters.begin(), it);     // finding the indexes of given letter on the matrix table
        it = find(letters.begin(), letters.end(), number1[0]);
        int index3 = distance(letters.begin(), it);     // finding the indexes of given letter on the matrix table

        my_queue queue_root;            // Calling my implementation of queue
        queue_root.pushback(temp);      // Firstly pushing the root
 

        while (!queue_root.empty()) {       // if queue is not empty it iterates

            

            node* searching_node;           
            searching_node = queue_root.front->nodeptr;     // Assigning searching_node to first element of the queue
            queue_root.popfront();          // popping the first element in the queue

            

            if (searching_node->child[0] == NULL) {                               // if we are at the leaves, means that our matrix is filled, we can check the matrix result if it fits
                int numb1 = 0;
                int numb2 = 0;
                int sum = 0;

                
                int big_num_size;                                       // in order to find longest adding number
                if (strlen(number1) >= strlen(number2)) {
                    big_num_size = strlen(number1);
                }
                else
                    big_num_size = strlen(number2);
                

                for (int i = 0; i < big_num_size; i++) {

                    int numeric1 = 0;
                    int numeric2 = 0;
                    int numeric3 = 0;

                    if (big_num_size > strlen(number1) && i == big_num_size - 1) {      // if number 2 is longer than number 1, took the most significant number as 0 for number 1 
                        numeric1 = 0;
                    }
                    else {
                        it = find(letters.begin(), letters.end(), number1[strlen(number1) - i - 1]);    // in order to find first number's decimal value
                        int index_num1 = distance(letters.begin(), it);

                        while (searching_node->matrix[index_num1][numeric1] != 1)
                            numeric1++;
                    }

                    if (big_num_size > strlen(number2) && i == big_num_size - 1) {      // if number 1 is longer than number 2, took the most significant number as 0 for number 2
                        numeric2 = 0;
                    }
                    else {
                        it = find(letters.begin(), letters.end(), number2[strlen(number2) - i - 1]);    // in order to find second number's decimal value
                        int index_num2 = distance(letters.begin(), it);

                        while (searching_node->matrix[index_num2][numeric2] != 1)
                            numeric2++;
                    }

                    it = find(letters.begin(), letters.end(), result[strlen(result) - i - 1]);     // in order to find result number's decimal value
                    int index_num3 = distance(letters.begin(), it);

                    while (searching_node->matrix[index_num3][numeric3] != 1)
                        numeric3++;

                    numb1 += numeric1 * pow(10, i);     // calculating the decimal value of the corresponding letter number on matrix
                    numb2 += numeric2 * pow(10, i);     // calculating the decimal value of the corresponding letter number on matrix
                    sum += numeric3 * pow(10, i);       // calculating the decimal value of the corresponding letter number on matrix
                }

                sum += pow(10, big_num_size);        // left most 1 is added

                if ((numb1 + numb2) == sum)             // Checking if the matrix's results matches the solution. If if fits returns the node
                    return searching_node;
            }

           
            if (searching_node->child[0] != NULL) {     // If current searching node has childs, goes in
               
                for (int i = 0; i < (10 - searching_node->used_numbs.size()); i++) {       // iterates number of child times of the current node
                    bool constraint = false;
       
                    if (searching_node->child[i]->matrix[index1][0] == 1 || searching_node->child[i]->matrix[index2][0] == 1 || searching_node->child[i]->matrix[index3][0] == 1)       // if any number starts with zero, constraint is true 
                        constraint = true;

                    bool is_filled = false;                                     // in order to check if the first letter of result's corresponding matrix row is filled or not

                    for (int j = 0; j < 10; j++) {
                        if (searching_node->child[i]->matrix[index1][j] == 1)
                            is_filled = true;
                    }

                    if (is_filled == true && searching_node->child[i]->matrix[index1][1] != 1)      // first letter of the result should be 1, otherwise constraint is true
                        constraint = true;

                    if (!constraint) {                  // if there is no constraints, we can send the ith child of the current node to our queue
                        node_visited++;                 // incrementing the visited nodes
                        queue_root.pushback(searching_node->child[i]);      // pushing the current nodes ith child to queue
                       
                    }
                    
                }
            }
        }
        return NULL;
    }
        
    ~Tree() {       // Destructor
        destructor(root);
    }

    void destructor(node* temp){     // Destructor helper
        if (temp != NULL) {
            for (int i = 0; i < (10 - temp->used_numbs.size()); i++) {
                destructor(temp->child[i]);
            }

            delete temp;
        }
    }

};




int main(int argc, char* argv[])
{

    ofstream myfile;    // file operation

    vector<char> letters;       // letter vector for discrete letters
    vector<char>::iterator it;

    for (int i = 0; i < 3; i++) {                       
        for (int j = 0; j < strlen(argv[i + 2]); j++) {
            it = find(letters.begin(), letters.end(), argv[i + 2][j]);
            if (it==letters.end())
                letters.push_back(argv[i + 2][j]);
        }
    }

    int child_number = 10;  // Starting child number of the root
    int child_index = 0;          // Starting child index
    Tree mytree(letters, child_number, child_index);    // creating mytree object

    node* answer;   

    auto start = chrono::high_resolution_clock::now();      // in order to calculate the running time
    
    // search algorithm is called and answer is get
    if (string(argv[1]) == "DFS") {
        answer = mytree.DFS(argv[2], argv[3], argv[4], letters);
    }
    else if (string(argv[1]) == "BFS") {
        answer = mytree.BFS(argv[2], argv[3], argv[4], letters);
    }
    else {
        cerr << "Search algorithm name is given wrong!" << endl;
        exit(1);
    }

    auto finish = chrono::high_resolution_clock::now();     // in order to calculate the running time
    chrono::duration<double> elapsed = finish - start;      // in order to calculate the running time

    // Writing the answers of asked questions
    cout << "Algorithm: " << string(argv[1]) << endl;
    cout << "Maximum number of nodes kept in the memory: " << mytree.node_created << endl;
    cout << "Number of the visited nodes: " << mytree.node_visited << endl;
    cout << "Running time: " << elapsed.count() << endl;
    cout << "Solution: ";
    for (int i = 0; i < letters.size(); i++) {              
        cout << letters[i] << ": ";
        int index = 0;
        while (answer->matrix[i][index] != 1)
            index++;
        cout << index;
            if (i != letters.size()-1)
                cout << ", ";
    }
    cout << endl;

    // writing solution to output file
    myfile.open(argv[5]);

    myfile << "\t";

    for (int j = 0; j < 10; j++)
        myfile << j << "\t";
    myfile << endl;

    for (int i = 0; i < letters.size(); i++) {
        myfile << (char)tolower(letters[i]) << "\t";
        for (int j = 0; j < 10; j++) {
            if(answer->matrix[i][j]==0)
                myfile << "." << "\t";
            else
                myfile << answer->matrix[i][j] << "\t";
        }
        myfile << endl;
    }




    return 0;
}
