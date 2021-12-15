/*
    Name: ilya Mustafa Nuhi
    ID:   150180710
    while compiling please use g++ -std=c++11
*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cmath> 
#include <algorithm>

using namespace std;

class Neuron {
protected:
    double value;
    double activated_value;
public:
    virtual void activate() = 0; //Pure Virtual Function
    Neuron() { value = 0; activated_value = 0; }      //Default Constructor
    double get_a();   //getter method
};
class Layer {      
    Neuron* Nptr;
    int neuron_num;
public:
    Layer(int, int, Layer&); //  Constructor
    Layer(int, int, double*);  //  Layer 0 Constructor
    Layer() { Nptr = NULL; }    //  Default Constructor
    int get_neuron_num();   //getter method     
    Neuron* get_Nptr();     //getter method
    ~Layer() { delete[] Nptr; } //Destructor
};

double Neuron::get_a() {
    return activated_value;
}
//---------------------------------------------------
class SigmoidNeuron : public Neuron {

public:
    void activate();    //Virutal Function
    SigmoidNeuron(double); //For Layer 0 Constructor
    SigmoidNeuron(Layer&, double*, double);  //For other layers Constructor
    SigmoidNeuron(){}   //Default Constructor
};

void SigmoidNeuron::activate() {
    activated_value = 1 / (1 + exp(-value));
}

SigmoidNeuron::SigmoidNeuron(double initial) { //For Layer 0 Constructor
    value = initial;
    activated_value = initial;
}
SigmoidNeuron::SigmoidNeuron(Layer& prev_layer, double* matrix_W, double matrix_b){
    value = 0;
    for (int i = 0; i < prev_layer.get_neuron_num(); i++) {
        value += prev_layer.get_Nptr()[i].get_a() * matrix_W[i];   // W.X part
    }
    value += matrix_b;
    activate(); 
}
//----------------------------------------------------
class ReluNeuron : public Neuron {

public:
    void activate();    //Virutal Function
    ReluNeuron(double); //For Layer 0 Constructor
    ReluNeuron(Layer&, double*, double);  //For other Layers Constructor
    ReluNeuron() {}   //Default Constructor
};

void ReluNeuron::activate() {
    activated_value = max(0.0,value);
}
ReluNeuron::ReluNeuron(double initial) {    //For layer 0 Constructor
    value = initial;
    activated_value = initial;
}
ReluNeuron::ReluNeuron(Layer& prev_layer, double* matrix_W, double matrix_b) {
    value = 0;
    for (int i = 0; i < prev_layer.get_neuron_num(); i++) {
        value += prev_layer.get_Nptr()[i].get_a() * matrix_W[i];   // W.X part
    }
    value += matrix_b;
    activate();
}
//---------------------------------------------------
class LReluNeuron : public Neuron {

public:
    void activate();    //Virutal Function
    LReluNeuron(double); //For Layer 0 Constructor
    LReluNeuron(Layer&, double*, double);  //For other Layers Construcotor
    LReluNeuron() {}   //Default Constructor
};

void LReluNeuron::activate() {
    activated_value = max(0.1 * value, value);
}
LReluNeuron::LReluNeuron(double initial) {      //For layer 0 Constructor
    value = initial;
    activated_value = initial;
}
LReluNeuron::LReluNeuron(Layer& prev_layer, double*matrix_W, double matrix_b) {
    value = 0;
    for (int i = 0; i < prev_layer.get_neuron_num(); i++) {
        value += prev_layer.get_Nptr()[i].get_a() * matrix_W[i];   // W.X part
    }
    value += matrix_b;
    activate();
}
//---------------------------------------------------


Neuron* Layer::get_Nptr() {
    return Nptr;
}
int Layer::get_neuron_num() {
    return neuron_num;
}

Layer::Layer(int neuron_numb, int type, double* initials) {        //Layer 0 Constructor
    neuron_num = neuron_numb;
    switch (type) {
    case 0:
        Nptr = new SigmoidNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = SigmoidNeuron(initials[i]);
        break;
    case 1:
        Nptr = new LReluNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = LReluNeuron(initials[i]);
        break;
    case 2:
        Nptr = new ReluNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = ReluNeuron(initials[i]);
        break;
    default:
        cout << "Neuron type is wrong!" << endl;
    }
}

Layer::Layer(int neuron_numb,int type,Layer& previous_layer) {        //Constructor
    neuron_num = neuron_numb;
    double** w_matrix;                          //Creating W matrix
    w_matrix = new double*[neuron_numb];
    for (int i = 0; i < neuron_num; i++) {
        w_matrix[i] = new double[previous_layer.get_neuron_num()];
    }
    for (int j = 0; j < neuron_numb; j++) {
        for (int k = 0; k < previous_layer.get_neuron_num(); k++) {
            w_matrix[j][k] = 0.1;
        }
    }
    double* b_matrix;               //Creating b matrix
    b_matrix = new double[neuron_numb];
    for (int p = 0; p < neuron_numb; p++) {
        b_matrix[p] = 0.1;
    }

    switch (type) {
    case 0:
        Nptr = new SigmoidNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = SigmoidNeuron(previous_layer,w_matrix[i], b_matrix[i]);
        break;
    case 1:
        Nptr = new LReluNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = LReluNeuron(previous_layer,w_matrix[i],b_matrix[i]);
        break;
    case 2:
        Nptr = new ReluNeuron[neuron_numb];
        for (int i = 0; i < neuron_numb; i++)
            Nptr[i] = ReluNeuron(previous_layer, w_matrix[i], b_matrix[i]);
        break;
    default:
        cout << "Neuron type is wrong!" << endl;
    }
    for (int s = 0; s < neuron_numb; s++) {
        delete w_matrix[s];
    }
    delete[] w_matrix;
    delete[] b_matrix;
}
//---------------------------------------------------
class Network {
    Layer** Lptr;
    int layer;
public:
    Network(int,int*,int*,double*);   //Constructor
    void output();
    ~Network() {        //Destructor
        for (int i = 0; i < layer; i++) {
            delete Lptr[i];
        }
        delete[] Lptr;}
};

Network::Network(int numb,int* neuron, int* type, double* value0) {     //Constructor
    layer = numb;
    Lptr = new Layer*[numb];
    Lptr[0] = new Layer(neuron[0], type[0],value0);
    for (int i = 1; i < numb; i++)
        Lptr[i] = new Layer(neuron[i],type[i],*Lptr[i-1]);
}
void Network::output() {        //Print Function
    for (int i = 0; i < layer; i++) {
        cout << "Layer " << i << ":" << endl;
        for (int j = 0; j < Lptr[i]->get_neuron_num(); j++) {
            cout << Lptr[i]->get_Nptr()[j].get_a() << endl;
        }
    }
}
void is_type(int* type,int numb) {
    for (int i = 0; i < numb; i++)
        if (type[i] != 0 && type[i] != 1 && type[i] != 2) 
            throw "Unidentified activation function!";
}
int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {    //To read all files at the same time
        ifstream input(argv[i]);
        int layer;
        input >> layer;
        int* neuron_numb = new int[layer];
        int* neuron_type = new int[layer];
        double* initial_values = new double[layer];
        for (int j = 0; j < layer; j++)
            input >> neuron_numb[j];
        for (int j = 0; j < layer; j++)
            input >> neuron_type[j];
        try {
            is_type(neuron_type, layer);
        }
        catch (const char* result1) {
            cout << result1 << endl;
            delete[] neuron_numb;
            delete[] neuron_type;
            delete[] initial_values;
            exit(1);
        }
        int check_shape = 0;
        try {
            while (!input.eof()) {
                if (check_shape < layer) {
                    input >> initial_values[check_shape];
                }
                else
                    throw "Input shape does not match!";
                check_shape++;
            }
            if (check_shape < layer)
                throw "Input shape does not match!";
        }
        catch (const char* result2) {
            cout <<  result2 << endl;
            delete[] neuron_numb;
            delete[] neuron_type;
            delete[] initial_values;
            exit(2);
        }
        Network obj(layer, neuron_numb, neuron_type, initial_values);
        obj.output();
        delete[] neuron_numb;
        delete[] neuron_type;
        delete[] initial_values;
    }
    return 0;
}
