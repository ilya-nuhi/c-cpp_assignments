#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

class Person {
    string name;
    string surname;
public:
    Person(string, string);             //Constructor
    string get_name();
    string get_surname();
};
Person::Person(string Name, string Surname) {     //Constructor
    this->name = Name;
    this->surname = Surname;
}
string Person::get_name() {
    return name;
}
string Person::get_surname() {
    return surname;
}

// Add new employees
// Constructor parameters: name, surname, vehicle_type
class Courier {
    Person* courier;
    string vehicle;         //bicycle 10kg, motorcycle 35kg, car 200kg
public:
    Courier* next;
    Courier(string, string, string);        //Constructor
    Courier(const Courier&);                //Copy Constructor
    Person* get_courier();  //getter method
    string get_vehicle();   //getter method
    bool operator == (Courier*);
    ~Courier();             //Destructor
};

Courier::~Courier() {       //Destructor
   delete courier;
}
Courier::Courier(const Courier& copy) {     //Copy Constructor
    vehicle = copy.vehicle;
    courier = new Person(copy.courier->get_name(),copy.courier->get_surname());
    next = copy.next;
}
Courier::Courier(string name, string surname, string vehicle_type) {        //Constructor
    courier = new Person(name, surname);
    vehicle = vehicle_type;
    //next = NULL;
}
bool Courier::operator==(Courier *C) {
    if (courier->get_name() == C->courier->get_name() && courier->get_surname() == C->courier->get_surname())
        return true;
    else
        return false;
}
Person* Courier::get_courier() {
    return courier;
}
string Courier::get_vehicle() {
    return vehicle;
}
// Create owners of the business
//Constructor parameters: name, surname
class Owner {
    Person* owner;
    float ownership;
public:
    Owner(string, string);      //Constructor
    Owner() {                   //Default Constructor
        owner = new Person("bos", "bos");
    }                    
    Person* get_owner();
    float get_ownership();
    void set_ownership(int);
    ~Owner();                   //Destructor
};
Owner::Owner(string owner_name, string owner_surname) {             //Constructor
    owner = new Person(owner_name, owner_surname);
}
Owner::~Owner() {
    delete owner;
}
Person* Owner::get_owner() {                //getter method
    return owner;
}
float Owner::get_ownership() {              //getter method
    return ownership;
}
void Owner::set_ownership(int number) {     //setter method
    ownership = (float)100 / number;   
}
//----------------------------------------------
//Crete the business itself
//Constructor parameters: name, address, owner_array, number_of_owners
class Business {
    string b_name;          //business name
    string b_address;
    Courier* head;
    Owner* owners;
    int owner_number;
public:
    Business(string, string, Owner*, int);
    void list_owners();
    void list_couriers();
    void hire_courier(Courier);
    void fire_courier(Courier);
    int calculate_shipment_capacity();
    void operator()();
    Courier operator[](int);
    ~Business();                //Destructor
};
Business::Business(string name, string address, Owner *owner_array, int number_of_owners) {             //Constructor
    b_name = name;
    b_address = address;
    owners = owner_array;
    owner_number = number_of_owners;
    for (int i = 0; i < number_of_owners; i++)
        owner_array[i].set_ownership(number_of_owners);
    head = NULL;
 }
Business::~Business() {         //Destructor
    delete[] owners;
}
void Business::list_owners() {
    for (int i = 0; i < owner_number; i++) {
        cout << owners[i].get_owner()->get_name() << " " << owners[i].get_owner()->get_surname();
        cout << " " << owners[i].get_ownership() << endl;
    }
}
void Business::list_couriers() {
    if (head == NULL) {
        cout << "There is no courier hired." << endl;
        return;
    }
    else {
        Courier* iter=head;
        while (iter != NULL) {
            cout << iter->get_courier()->get_name()<< " " << iter->get_courier()->get_surname();
            cout << " " << iter->get_vehicle() << endl;
            iter = iter->next;
        }
    }
}
void Business::hire_courier(Courier C) {
    if (head == NULL) {
        head = new Courier(C);
        head->next = NULL;
    }
    else {
        Courier* iter = head;
        while (iter->next != NULL)
            iter = iter->next;
        iter->next = new Courier(C);
        iter->next->next = NULL;
    }
}
void Business::fire_courier(Courier c1) {
    Courier* iter = head;
    Courier* temp = head;
    int counter = 0;
    if (head == NULL) {
        cout << "Courier list is empty!" << endl;
    }
    while (!(c1==iter)) {    //using overloaded == operator
        if (iter == NULL) {
            cout << "Error! Couriers doesn't match." << endl;
            return;
        }
        iter = iter->next;
        counter++;
    }
    if (head->next==NULL) {     //deleting removed courier from list
        delete head;
        head = NULL;
    }
    else if (c1 == head) {
        head = head->next;
        delete iter;
    }
    else {
        for (int i = 0; i < counter - 1; i++)
            temp = temp->next;
        temp->next = temp->next->next;
        delete iter;
    }
}
int Business::calculate_shipment_capacity() {
    int capacity = 0;
    Courier* iter = head;
    string vehicle_type;
    while (iter != NULL) {
        vehicle_type = iter->get_vehicle();
        if (vehicle_type == "bicycle")
            capacity += 10;
        else if (vehicle_type == "motorcycle")
            capacity += 35;
        else if (vehicle_type == "car")
            capacity += 200;
        else
            cout << "Some vehicle type written wrong" << endl;
        iter = iter->next;
    }
    return capacity;
}
void Business::operator()() {
    cout << b_name << " " << b_address << " " << endl ;
    list_owners();
    list_couriers();
    cout << endl;
}
Courier Business::operator[](int index) {
    Courier* iter = head;
    for (int i = 0; i < index; i++) {
        iter = iter->next;
        if (iter == NULL)
            cout << "index not found!" << endl;
    }
    return *iter;
}

int main() {

    Owner o1 = Owner("Owner_name1", "surname1");
    Owner o2 = Owner("Owner_name2", "surname2");
    Owner* owner_arr = new Owner[2];
    owner_arr[0] = o1;
    owner_arr[1] = o2;
    
    Business atlas = Business("Atlas", "Maslak Istanbul/Turkey", owner_arr, 2);
   
    // Print owner info to screen: name, surname, ownership
   
    atlas.list_owners();

    
    Courier c1 = Courier("Courier", "surname1", "car");
    Courier c2 = Courier("Courier", "surname2", "motorcycle");
    Courier c3 = Courier("Courier", "surname3", "motorcycle");

    atlas.hire_courier(c1);
    atlas.hire_courier(c2);
    atlas.hire_courier(c3);


    // Print business info to screen
    // Name, address, owners, couriers
    atlas();

    // Get courier with index 1 and remove it from the list/array
    Courier cour = atlas[1];
    atlas.fire_courier(cour);

    // Print remaining couriers
    atlas.list_couriers();
    // Print current maximum shipment capacity
    cout << atlas.calculate_shipment_capacity() << endl;
    return 0;
}