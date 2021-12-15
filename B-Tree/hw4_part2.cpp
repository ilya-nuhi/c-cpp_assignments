#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct data_struct {
    int x;
    int y;
    char z;
};

typedef struct data_struct Data;

class node_struct {

    int d;
    int n;
    bool is_leaf;

public:
    Data* key;
    node_struct** child;
    node_struct(int, bool);

    void insert_node(Data, char);
    void split_node(int, node_struct*);
    void prefix_print();
    void remove(int, char);
    void remove(char, char);
    int find_index(int, char);
    int find_index(char);
    void remove_from_leaf(int idx);
    void remove_from_nonleaf(int, char);
    Data predecessor(int);
    Data successor(int);
    void combine(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);

    friend class BTree;
};

class BTree {

    int d;

public:
    node_struct* root;
    BTree(int temp) {
        root = NULL;
        d = temp;
    }

    void prefix_print() {
        if (root != NULL)
            root->prefix_print();
    }

    void insert(Data, char);
    void remove(int, char);
    void remove(char, char);
};

node_struct::node_struct(int d1, bool is_leaf1) {   //Constructor
    d = d1;
    is_leaf = is_leaf1;

    key = new Data[2 * d - 1];
    child = new node_struct * [2 * d];

    n = 0;
}

void node_struct::prefix_print() {  //Printing function in order of prefix order
    int i;
    for (i = 0; i < n; i++)
    {
        cout << "(" << key[i].x << "," << key[i].y << "," << key[i].z << ")";
    }
    cout << endl;
    if (is_leaf == false)
    {
        for (int j = 0; j <= n; j++)
        {
            child[j]->prefix_print();
        }
    }
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(Data k, char attribute) {
    //Allocate memory for root if tree is empty
    if (root == NULL) {
        root = new node_struct(d, true);
        root->key[0] = k;
        root->n = 1;
    }
    else {
        //if tree is full height of the tree grows
        if (root->n == 2 * d - 1) {
            node_struct* s = new node_struct(d, false);

            s->child[0] = root;

            s->split_node(0, root);

            int i = 0;

            if (attribute == 'x') {
                if (s->key[0].x < k.x)
                    i++;
            }
            if (attribute == 'y') {
                if (s->key[0].y < k.y)
                    i++;
            }
            if (attribute == 'z') {
                if (s->key[0].z < k.z)
                    i++;
            }

            s->child[i]->insert_node(k, attribute);

            root = s;       //changeing root
        }
        else
            root->insert_node(k, attribute);
    }
}

void node_struct::insert_node(Data k, char attribute) {
    int i = n - 1;      // i is the rightmost index of the current node

    if (is_leaf == true) {  // if it is the leaf node
        // Moving all greater keys 1 step ahead
        if (attribute == 'x') {
            while (i >= 0 && key[i].x > k.x) {
                key[i + 1] = key[i];
                i--;
            }
        }
        if (attribute == 'y') {
            while (i >= 0 && key[i].y > k.y) {
                key[i + 1] = key[i];
                i--;
            }
        }
        if (attribute == 'z') {
            while (i >= 0 && key[i].z > k.z) {
                key[i + 1] = key[i];
                i--;
            }
        }

        key[i + 1] = k; //inserting new key 
        n = n + 1;  //incrementing n by  1
    }

    else {  // if node is not leaf
        // finding the child which is going to have the new key
        if (attribute == 'x') {
            while (i >= 0 && key[i].x > k.x)
                i--;
        }
        if (attribute == 'y') {
            while (i >= 0 && key[i].y > k.y)
                i--;
        }
        if (attribute == 'z') {
            while (i >= 0 && key[i].z > k.z)
                i--;
        }

        //if the found child is full
        if (child[i + 1]->n == 2 * d - 1) {
            split_node(i + 1, child[i + 1]);

            /* After split, the middle key of C[i] goes up and 
             C[i] is splitted into two.  See which of the two 
             is going to have the new key*/
            if (attribute == 'x') {
                if (key[i + 1].x < k.x)
                    i++;
            }
            if (attribute == 'y') {
                if (key[i + 1].y < k.y)
                    i++;
            }
            if (attribute == 'z') {
                if (key[i + 1].z < k.z)
                    i++;
            }
        }
        child[i + 1]->insert_node(k, attribute);
    }
}

void node_struct::split_node(int i, node_struct* y) {
    node_struct* z = new node_struct(y->d, y->is_leaf); //creating a new node for right child
    z->n = d - 1;

    //copying the last d-1 keys of y to z
    for (int j = 0; j < d - 1; j++)
        z->key[j] = y->key[j + d];

    //copying the last d children of y to z
    if (y->is_leaf == false) {
        for (int j = 0; j < d; j++)
            z->child[j] = y->child[j + d];
    }

    //rearranging the number of keys of the left child
    y->n = d - 1;

    // moving the greater childs 1 step ahead if there is any
    for (int j = n; j >= i + 1; j--)
        child[j + 1] = child[j];

    // link the new child to this node
    child[i + 1] = z;

    // move all greater keys 1 step ahead
    for (int j = n - 1; j >= i; j--)
        key[j + 1] = key[j];

    // copy the middle key of y to this node
    key[i] = y->key[d - 1];

    // incrementing count of keys in this node
    n = n + 1;
}

void BTree::remove(int k, char attribute)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }


    root->remove(k, attribute);

    /* if the root node has 0 keys, make its first child as the new root if it has a child,
    otherwise set root as NULL*/
    if (root->n == 0)
    {
        node_struct* tmp = root;
        if (root->is_leaf)
            root = NULL;
        else
            root = root->child[0];

        delete tmp; // deleting the old root
    }
    return;
}

// same remove function for attribute z 
void BTree::remove(char k, char attribute)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }


    root->remove(k, attribute);

    if (root->n == 0)
    {
        node_struct* tmp = root;
        if (root->is_leaf)
            root = NULL;
        else
            root = root->child[0];

        delete tmp;
    }
    return;
}

// returns the index of the key or child of the searched key
int node_struct::find_index(int k, char attribute)
{
    int idx = 0;
    if (attribute == 'x') {
        while (idx < n && key[idx].x < k)
            ++idx;
    }
    if (attribute == 'y') {
        while (idx < n && key[idx].y < k)
            ++idx;
    }
    return idx;
}

int node_struct::find_index(char k)
{
    int idx = 0;

    while (idx < n && key[idx].z < k)
        ++idx;

    return idx;
}

// gets the predecessor of the given index of child
Data node_struct::predecessor(int idx)
{
    node_struct* cur = child[idx];
    // goes to the rightmost leaf
    while (!cur->is_leaf)
        cur = cur->child[cur->n];

    return cur->key[cur->n - 1];
   
}

// gets the successor of the given index of child
Data node_struct::successor(int idx)
{

    node_struct* cur = child[idx + 1];
    // goes to the rightmost leaf
    while (!cur->is_leaf)
        cur = cur->child[0];

    return cur->key[0];
}

// Combines the child[idx] and child[idx+1]
void node_struct::combine(int idx)
{
    node_struct* children = child[idx];
    node_struct* sibling = child[idx + 1];

    // pulling the key from current node and inserting it in the middle of the combined node
    children->key[d - 1] = key[idx];

    // adding the keys of the sibling node to children node
    for (int i = 0; i < sibling->n; ++i)
        children->key[i + d] = sibling->key[i];

    // moving the child pointers of the siblings to children node
    if (!children->is_leaf)
    {
        for (int i = 0; i <= sibling->n; ++i)
            children->child[i + d] = sibling->child[i];
    }

    // moving all greater keys to 1 step before because of the gap created from adding the key to child
    for (int i = idx + 1; i < n; ++i)
        key[i - 1] = key[i];
    // moving all greater childs to 1 step before because of the gap created from adding the key to child
    for (int i = idx + 2; i <= n; ++i)
        child[i - 1] = child[i];

    children->n += sibling->n + 1;  // new child's key number is updated which is the max key number
    n--;    // current node's key number is decremented

    delete(sibling);    // freeing the memory of sibling
    return;
}

// removing key from a leaf node
void node_struct::remove_from_leaf(int idx)
{
    // moving all greater keys 1 step before
    for (int i = idx + 1; i < n; ++i)
        key[i - 1] = key[i];

    n--; //updateing key number

    return;
}

// removing key from a nonleaf node
void node_struct::remove_from_nonleaf(int idx, char attribute)
{
    int k1=0;
    char k2=' ';
    if (attribute == 'x')
        k1 = key[idx].x;
    if (attribute == 'y')
        k1 = key[idx].y;
    if (attribute == 'z')
        k2 = key[idx].z;

    /* if the left child of the current index of the key has at least d amount of keys
    predecessor function is called and predecessor key assigned to the key place which will be
    deleted and predecessor key is deleted from child node
    */
    if (child[idx]->n >= d)
    {
        Data pred;
        pred = predecessor(idx);
        key[idx] = pred;

        
        if (attribute == 'x')
            child[idx]->remove(pred.x, attribute);
        if (attribute == 'y')
            child[idx]->remove(pred.y, attribute);
        if (attribute == 'z')
            child[idx]->remove(pred.z, attribute);
    }

    /* if the right child of the current index of the key has at least d amount of keys
    predecessor function is called and predecessor key assigned to the key place which will be
    deleted and predecessor key is deleted from child node
    */
    else if (child[idx + 1]->n >= d)
    {
        Data succ;
        succ = successor(idx);
        key[idx] = succ;
        
        if (attribute == 'x')
            child[idx + 1]->remove(succ.x, attribute);
        if (attribute == 'y')
            child[idx + 1]->remove(succ.y, attribute);
        if (attribute == 'z')
            child[idx + 1]->remove(succ.z, attribute);

    }

    /* if both child have smaller than d keys then childs and key are combined and 
    remove fucntion is called again for the child node*/
    else
    {
        combine(idx);
        if (attribute == 'x' || attribute == 'y')
            child[idx]->remove(k1, attribute);
        if (attribute == 'z')
            child[idx]->remove(k2, attribute);
    }
    return;
}

/* Inserts the current key to the leftmost key of child[idx], 
Borrows a key from rightmost child[idx-1] and insert it to the node*/
void node_struct::borrowFromPrev(int idx)
{

    node_struct* children = child[idx];
    node_struct* sibling = child[idx - 1];

    // moving all keys in child[idx] 1 step ahead
    for (int i = children->n - 1; i >= 0; --i)
        children->key[i + 1] = children->key[i];

    // if child[idx] is not a leaf move all its child pointers 1 step ahead
    if (!children->is_leaf)
    {
        for (int i = children->n; i >= 0; --i)
            children->child[i + 1] = children->child[i];
    }

    // Setting children's first key equal to keys[idx-1] from the current node
    children->key[0] = key[idx - 1];

    // Moving sibling's last child as child[idx]'s first child
    if (!children->is_leaf)
        children->child[0] = sibling->child[sibling->n];

    // moving the key from sibling to parent
    key[idx - 1] = sibling->key[sibling->n - 1];

    // updateing key numbers
    children->n += 1;
    sibling->n -= 1;

    return;
}

/* Inserts the current key to the rightmost key of child[idx],
Borrows a key from leftmost child[idx+1] and insert it to the node*/
void node_struct::borrowFromNext(int idx)
{

    node_struct* children = child[idx];
    node_struct* sibling = child[idx + 1];

    // inserting the key to the rightmost key of the children
    children->key[(children->n)] = key[idx];

    // siblings first child is inserted as the last child
    if (!(children->is_leaf))
        children->child[(children->n) + 1] = sibling->child[0];

    //The first key from sibling is inserted into keys[idx]
    key[idx] = sibling->key[0];

    // Moving all keys in sibling one step behind
    for (int i = 1; i < sibling->n; ++i)
        sibling->key[i - 1] = sibling->key[i];

    // Moving the child pointers one step behind
    if (!sibling->is_leaf)
    {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->child[i - 1] = sibling->child[i];
    }

    // updateing the key numbers
    children->n += 1;
    sibling->n -= 1;

    return;
}

// A function to fill child C[idx] which has less than d-1 keys
void node_struct::fill(int idx)
{

    // If the previous child(child[idx-1]) has more than d-1 keys, borrow a key from that child
    if (idx != 0 && child[idx - 1]->n >= d)
        borrowFromPrev(idx);

    // If the next child(child[idx+1]) has more than d-1 keys, borrow a key from that child
    else if (idx != n && child[idx + 1]->n >= d)
        borrowFromNext(idx);


    /* Combine if both childs have less than d keys.
    If child[idx] is the last child, combine it with with its previous sibling 
     Otherwise combine it with its next sibling*/
    else
    {
        if (idx != n)
            combine(idx);
        else
            combine(idx - 1);
    }
    return;
}

// remove the key k from tree
void node_struct::remove(int k, char attribute)
{
    //find the index of the child or key of the searched key
    int idx = find_index(k, attribute);  

    // if the searched key is in this node
    if (idx < n && (key[idx].x == k || key[idx].y == k))
    {

        if (is_leaf)
            remove_from_leaf(idx);
        else
            remove_from_nonleaf(idx, attribute);
    }
    else
    {
        // If this node is a leaf node, then the key is not present in tree
        if (is_leaf)
        {
            cout << "The key " << k << " is does not exist in the tree\n";
            return;
        }

        // if the searched key is in the rightmost child
        bool flag = ((idx == n) ? true : false);

        // if the child which has the searched key has less than d keys fill the child
        if (child[idx]->n < d)
            fill(idx);

        /* if last child has been combined it must be removed from idx-1'th child because 
        we are deleting a key from current node, otherwise its removed from idx'th child*/
        if (flag && idx > n)
            child[idx - 1]->remove(k, attribute);
        else
            child[idx]->remove(k, attribute);
    }
    return;
}

// same remove function for attribute z 
void node_struct::remove(char k, char attribute)
{
    int idx = find_index(k);

    if (idx < n && key[idx].z == k)
    {

        if (is_leaf)
            remove_from_leaf(idx);
        else
            remove_from_nonleaf(idx, attribute);
    }
    else
    {

        if (is_leaf)
        {
            cout << "The key " << k << " is does not exist in the tree\n";
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (child[idx]->n < d)
            fill(idx);

        if (flag && idx > n)
            child[idx - 1]->remove(k, attribute);
        else
            child[idx]->remove(k, attribute);
    }
    return;
}

int main() {

    int total_node, degree, x, y;
    char attribute, z;
    cin >> total_node;
    cin >> degree;
    cin >> attribute;

    BTree mytree(degree);

    for (int i = 0; i < total_node; i++) {
        cin >> x;
        cin >> y;
        cin >> z;
        Data temp;
        temp.x = x;
        temp.y = y;
        temp.z = z;
        mytree.insert(temp, attribute);
    }

    int del1;
    char del2;

    if (attribute == 'z') {
        cin >> del2;
        mytree.remove(del2, attribute);
    }
    else if (attribute == 'x' || attribute == 'y') {
        cin >> del1;
        mytree.remove(del1, attribute);
    }
    else
        cerr << "Given attribute is wrong" << endl;

    mytree.prefix_print();

    return 0;
}