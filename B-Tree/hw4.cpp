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
};

node_struct::node_struct(int d1, bool is_leaf1) {
    d = d1;
    is_leaf = is_leaf1;

    key = new Data[2 * d - 1];
    child = new node_struct * [2 * d];

    n = 0;
}

void node_struct::prefix_print() {
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


void BTree::insert(Data k, char attribute) {
    if (root == NULL) {
        root = new node_struct(d, true);
        root->key[0] = k;
        root->n = 1;
    }
    else {
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

            root = s;
        }
        else
            root->insert_node(k, attribute);
    }
}

void node_struct::insert_node(Data k, char attribute) {
    int i = n - 1;

    if (is_leaf == true) {
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

        key[i + 1] = k;
        n = n + 1;
    }

    else {
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

        if (child[i + 1]->n == 2 * d - 1) {
            split_node(i + 1, child[i + 1]);

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
    node_struct* z = new node_struct(y->d, y->is_leaf);
    z->n = d - 1;

    for (int j = 0; j < d - 1; j++)
        z->key[j] = y->key[j + d];

    if (y->is_leaf == false) {
        for (int j = 0; j < d; j++)
            z->child[j] = y->child[j + d];
    }

    y->n = d - 1;
    for (int j = n; j >= i + 1; j--)
        child[j + 1] = child[j];

    child[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        key[j + 1] = key[j];

    key[i] = y->key[d - 1];
    n = n + 1;
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
    mytree.prefix_print();

    return 0;
}