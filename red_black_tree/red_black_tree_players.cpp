#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {			// Struct of the Node of the RBT
	string season;
	string name;
	string team;
	int rebound;
	int assist;
	int point;
	Node* parent;
	Node* left;
	Node* right;
	int color;
};

typedef Node* NodePtr;

struct max_player {			// Stores the maximum scorer's asked values.(max rebounder, max assister, mas pointer)
	string season = "";
	string name = "";
	int rebound = 0;
	int assist = 0;
	int point = 0;
};


class Red_Black_Tree {		// Red Black Tree class
private:

	NodePtr root;

	NodePtr SearchTreeHelper(NodePtr, NodePtr);		// SearchTree funcitons helper function, searches the given node's name in the red black tree and returns its pointer
	void insertFix(NodePtr);						// For balancing the tree after insertion
	void PrintTreeHelper(NodePtr, string, bool);	// PrintTree function's helper function, prints the red black tree in preorder format
	void RotateLeft(NodePtr);						// In order to fix the red black tree after inserting a new element, rotates left
	void RotateRight(NodePtr);						// In order to fix the red black tree after inserting a new element, rotates right

public:

	NodePtr NULLNODE;								// its a null node

	Red_Black_Tree();								// Constructor

	NodePtr SearchTree(NodePtr key) {				// Searches the given node's name in the red black tree and returns its pointer
		return SearchTreeHelper(this->root, key);
	}
	
	
	void insert(NodePtr);							// Inserting a new node to red black tree

	void PrintTree() {								// Prints the red black tree in preorder format
		if (root) {
			PrintTreeHelper(this->root, "", true);	
		}
	}
};

// Constructor, assigns the NULLNODE variable
Red_Black_Tree::Red_Black_Tree() {														
	NULLNODE = new Node;
	NULLNODE->point = 0;
	NULLNODE->assist = 0;
	NULLNODE->rebound = 0;
	NULLNODE->color = 0;
	NULLNODE->left = nullptr;
	NULLNODE->right = nullptr;
	root = NULLNODE;
}												

// Searches the given node's name in the red black tree and returns its pointer
NodePtr Red_Black_Tree::SearchTreeHelper(NodePtr node, NodePtr key) {
	if (node == NULLNODE || key->name == node->name) {
		return node;
	}

	if (key->name < node->name) {
		return SearchTreeHelper(node->left, key);
	}
	return SearchTreeHelper(node->right, key);
}

// Prints the red black tree in preorder format
void Red_Black_Tree::PrintTreeHelper(NodePtr root, string indent, bool last) {
	if (root != NULLNODE) {
		cout << indent;
		if (last) {
			cout << "-";
			indent += "-";
		}
		else {
			cout << "-";
			indent += "-";
		}
		string node_color;
		if (root->color == 1)	node_color = "RED";
		else node_color = "BLACK";
		cout << "(" << node_color << ")" << root->name << endl;
		PrintTreeHelper(root->left, indent, false);
		PrintTreeHelper(root->right, indent, true);
	}
}

// Inserting a new node to red black tree
void Red_Black_Tree::insert(NodePtr key) {


	NodePtr y = nullptr;
	NodePtr x = this->root;

	while (x != NULLNODE) {
		y = x;
		if (key->name < x->name) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	key->parent = y;
	if (y == nullptr) {
		root = key;

	}
	else if (key->name < y->name) {
		y->left = key;
	}
	else {
		y->right = key;
	}

	if (key->parent == nullptr) {
		key->color = 0;
		return;
	}

	if (key->parent->parent == nullptr) {
		return;
	}

	insertFix(key);
}

// For balancing the tree after insertion
void Red_Black_Tree::insertFix(NodePtr key) {
	NodePtr u;
	while (key->parent->color == 1) {
		if (key->parent == key->parent->parent->right) {
			u = key->parent->parent->left;
			if (u->color == 1) {
				u->color = 0;
				key->parent->color = 0;
				key->parent->parent->color = 1;
				key = key->parent->parent;
			}
			else {
				if (key == key->parent->left) {
					key = key->parent;
					RotateRight(key);
				}
				key->parent->color = 0;
				key->parent->parent->color = 1;
				RotateLeft(key->parent->parent);
			}
		}
		else {
			u = key->parent->parent->right;

			if (u->color == 1) {
				u->color = 0;
				key->parent->color = 0;
				key->parent->parent->color = 1;
				key = key->parent->parent;
			}
			else {
				if (key == key->parent->right) {
					key = key->parent;
					RotateLeft(key);
				}
				key->parent->color = 0;
				key->parent->parent->color = 1;
				RotateRight(key->parent->parent);
			}
		}
		if (key == root) {
			break;
		}
	}
	root->color = 0;
}

// In order to fix the red black tree after inserting a new element, rotates left
void Red_Black_Tree::RotateLeft(NodePtr x) {
	NodePtr y = x->right;
	x->right = y->left;
	if (y->left != NULLNODE) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nullptr) {
		this->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

// In order to fix the red black tree after inserting a new element, rotates right
void Red_Black_Tree::RotateRight(NodePtr x) {
	NodePtr y = x->left;
	x->left = y->right;
	if (y->right != NULLNODE) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == nullptr) {
		this->root = y;
	}
	else if (x == x->parent->right) {
		x->parent->right = y;
	}
	else {
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}



int main(int argc, char** argv) {			// main function

	Red_Black_Tree bst;						//creating rbt object

	ifstream file;
	file.open(argv[1]);


	if (!file) {
		cerr << "File cannot be opened!";
		exit(1);
	}


	string line;														// stores the read strings from file
	string season, prev_season, name, team;
	int rebound, assist, point;
	bool first_season = true;											// in order to arrange intended conditions for first season
	bool first_season2 = true;											// in order to arrange intended conditions for first season

	max_player max_points, max_assist, max_rebound;						// stores the max players' datas
	NodePtr finder;														// points to asked node
	getline(file, line);												// in order to read the first header line

	while (getline(file, line, ',')) {									// while read file still have unread data does the operations and reads the season value

		season = line;													// assigns the read value which is the season information from file to season variable											

		if ((season != prev_season) && !first_season) {					// if the season is changed finished season's infos are written

			cout << "End of the " << prev_season << " Season" << endl;
			cout << "Max Points: " << max_points.point << " - " << "Player Name: " << max_points.name << endl;
			cout << "Max Assists: " << max_assist.assist << " - " << "Player Name: " << max_assist.name << endl;
			cout << "Max Rebs " << max_rebound.rebound << " - " << "Player Name: " << max_rebound.name << endl;
			if (first_season2) {										// if the season is first season it prints the red black tree
				bst.PrintTree();
			}
			first_season2 = false;	// first season is over
			
		}

		// getting the other informations from the file and assigning to variables
		getline(file, line, ',');
		name = line;
		getline(file, line, ',');
		team = line;
		getline(file, line, ',');
		rebound = stoi(line);
		getline(file, line, ',');
		assist = stoi(line);
		getline(file, line, '\n');
		point = stoi(line);

		// storing the read values from file to node structure
		NodePtr node = new Node;

		node->name = name;
		node->season = season;
		node->team = team;
		node->rebound = rebound;
		node->assist = assist;
		node->point = point;
		node->parent = nullptr;
		node->left = bst.NULLNODE;
		node->right = bst.NULLNODE;
		node->color = 1;

		if (((season == prev_season) || first_season) && first_season2) {	// if it is the first season
			first_season = false;
			prev_season = season;
	
		}
		else {																// if it is not the first seasons
			prev_season = season;
		}

		finder = bst.SearchTree(node);		// searches the new player in the tree  
		if (finder == bst.NULLNODE) {			// if it doesn't find a player in the tree, adds the new player to the red blac tree
			bst.insert(node);
			finder = bst.SearchTree(node);
		}
		else {								// if it finds a player in the tree adds the assist point and rebound values
			finder->assist += assist;
			finder->point += point;
			finder->rebound += rebound;
		}

		// checks the max players and if we have a new max player it changes the max datas for the corresponding attribute
		if (max_points.point < finder->point) {
			max_points.assist = finder->assist;
			max_points.name = finder->name;
			max_points.rebound = finder->rebound;
			max_points.point = finder->point;
			max_points.season = finder->season;
		}
		if (max_assist.assist < finder->assist) {
			max_assist.assist = finder->assist;
			max_assist.name = finder->name;
			max_assist.rebound = finder->rebound;
			max_assist.point = finder->point;
			max_assist.season = finder->season;
		}
		if (max_rebound.rebound < finder->rebound) {
			max_rebound.assist = finder->assist;
			max_rebound.name = finder->name;
			max_rebound.rebound = finder->rebound;
			max_rebound.point = finder->point;
			max_rebound.season = finder->season;
		}
	}

	// in order to print the last seasons infos
	cout << "End of the " << season << " Season" << endl;
	cout << "Max Points: " << max_points.point << " - " << "Player Name: " << max_points.name << endl;
	cout << "Max Assists: " << max_assist.assist << " - " << "Player Name: " << max_assist.name << endl;
	cout << "Max Rebs " << max_rebound.rebound << " - " << "Player Name: " << max_rebound.name << endl;

	

	return 0;
}