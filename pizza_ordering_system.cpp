/*
    Name: ilya Mustafa Nuhi
    ID:   150180710
    while compiling please use g++ -std=c++11
*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

class Pizza {
    string name;                         //name of the pizaa
    string size;                         // small, medium, big
    string* ingredients;                 //changes depending on the pizza type(max 6 igredients)
                                         //mozerella,chicken,mushroom,corn,onion
                                         //tomato,broccoli,pepperoni,olive,sausage
    string crust_type;                   //thic, normal, thin
public:
    Pizza();                             //Default Constructor
    Pizza(string, string, int);          //Asked pizza constructor
    Pizza(const Pizza&);                 //Copy constructor
    ~Pizza();                            //Destructor
    string get_size();                   //getter method
    string get_name();                   //getter method
    string* get_ingredients();           //getter method
    string get_crust_type();             //getter method
};
string Pizza::get_size() {               //getter method
    return size;
}
string Pizza::get_name() {               //getter method
    return name;
}
string* Pizza::get_ingredients() {       //getter method
    return ingredients;
}
string Pizza::get_crust_type() {         //getter method
    return crust_type;
}
Pizza::Pizza() {                         //Pizza default constructor
    size = "medium";
    ingredients = new string[1];
    ingredients[0] = "mozarella";
    crust_type = "normal";
}
Pizza::Pizza(string size, string crust_type, int pizza_type) {      //Asked pizza constructor
    if (pizza_type == 1) {
        ingredients = new string[6];
        this->name = "Chicken Pizza";
        this->size = size;
        this->crust_type = crust_type;
        ingredients[0] = "mozarella";
        ingredients[1] = "chicken";
        ingredients[2] = "mushroom";
        ingredients[3] = "corn";
        ingredients[4] = "onion";
        ingredients[5] = "tomato";
    }
    else if (pizza_type == 2) {
        ingredients = new string[6];
        this->name = "Broccoli Pizza";
        this->size = size;
        this->crust_type = crust_type;
        ingredients[0] = "mozarella";
        ingredients[1] = "broccoli";
        ingredients[2] = "pepperoni";
        ingredients[3] = "olive";
        ingredients[4] = "corn";
        ingredients[5] = "onion";
    }
    else if (pizza_type == 3) {
        ingredients = new string[6];
        this->name = "Sausage Pizza";
        this->size = size;
        this->crust_type = crust_type;
        ingredients[0] = "mozarella";
        ingredients[1] = "sausage";
        ingredients[2] = "tomato";
        ingredients[3] = "olive";
        ingredients[4] = "mushroom";
        ingredients[5] = "corn";
    }
    else
        cout << "Wrong number of pizza_type" << endl;
}
Pizza::Pizza(const Pizza& copied_obj) {       //Copy constructor
    this->name = copied_obj.name;
    this->size = copied_obj.size;
    this->crust_type = copied_obj.crust_type;
    this->ingredients = new string[6];
    this->ingredients[0] = copied_obj.ingredients[0];
    this->ingredients[1] = copied_obj.ingredients[1];
    this->ingredients[2] = copied_obj.ingredients[2];
    this->ingredients[3] = copied_obj.ingredients[3];
    this->ingredients[4] = copied_obj.ingredients[4];
    this->ingredients[5] = copied_obj.ingredients[5];

    cout << "Please enter the number of the ingredient you want to remove from pizza." << endl;
    for (int i = 0; i < 6; i++)
        cout << i + 1 << ". " << copied_obj.ingredients[i] << endl;
    cout << "Press 0 to save it." << endl;
    int choice;
    cin >> choice;
    while (choice < 0 || choice>6) {
        cout << "Please choose a valid number." << endl;
        cin >> choice;
    }
    int choice_arr[6];                  //to store choices
    int x = 0;
    while (choice != 0) {
        choice_arr[x] = choice;         //storing which ingredients must be removed
        x++;
        cin >> choice;
    }
    for (int j = 0; j < x; j++) {       //assigning space to removed ingredients. When i ll print the ingredients i ll skip printing ingredient if the ingredient is ""
        this->ingredients[choice_arr[j] - 1] = "";
    }
}

Pizza::~Pizza() {                       //Destructor
    delete[] ingredients;
}
//----------------------------------------------------------------------------------------------------
struct Drinks {
    int drink_type[4] = { 0 };
    string drink_names[4] = { "cola","soda","ice tea","fruit juice" };
};
class Order {
    int pizza_amount;           //i had to add the amounts because it doesn't allow me to find the size of pointer array while printing order with using method size=sizeof(p)/sizeof(p[0]);
    int drink_amount;           //same as above
    string customer;
    Pizza** pizza_order;        //i defined pizza_order as array of pointers because in take order function i am creating Pizza array in curly brackets and when it goes out it calls the destructor and delets my ingredients
    int* drink;                 

public:
    Order(string, Pizza**, int*, int, int);          //Constructor with 3 parameters
    Order(string, Pizza**, int);                     //Constructor with 2 paramterers 
    float getPrice();                                //Calculates the total price
    void printOrder();                               //Prints the order as customer name,ordered pizzas and drinks.
    Order* next;
    string get_customer();                           //getter method
    ~Order();                                        //destructor
};
Order::~Order() {                                    //destructor
    delete* pizza_order;
    delete drink;
}
string Order::get_customer() {                       //getter method
    return customer;
}
Order::Order(string customer_name, Pizza** ordered_pizza, int* drinks, int pizza_numb, int drink_numb) {      //Constructor with 3 parameters
    this->customer = customer_name;
    this->pizza_order = ordered_pizza;
    this->drink = drinks;
    this->pizza_amount = pizza_numb;
    this->drink_amount = drink_numb;
}
Order::Order(string customer_name, Pizza** ordered_pizza, int pizza_numb) {                   //Constructor with 2 paramterers 
    this->customer = customer_name;
    this->pizza_order = ordered_pizza;
    this->pizza_amount = pizza_numb;
    this->drink = NULL;
}
float Order::getPrice() {           // calculates price of the order
    float size_price = 0;
    float total_price = 0;
    float drink_price = 0;
    if (pizza_order[0]->get_size() == "small")
        size_price = 15;
    else if (pizza_order[0]->get_size() == "medium")
        size_price = 20;
    else {
        size_price = 25;
    }

    if (drink != NULL) {            // if there is no drink drink price remains 0
        Drinks* Drinksptr = new Drinks;
        for (int i = 0; i < drink_amount; i++) {
            if (this->drink[i] == 1)
                drink_price += 4;                       //cola
            else if (this->drink[i] == 2)
                drink_price += 2;                       //soda
            else if (this->drink[i] == 3)
                drink_price += 3;                       //ice tea
            else
                drink_price += 3.5;                     //fruit juice
        }
    }
    return total_price = (this->pizza_amount * size_price) + drink_price;
}
void Order::printOrder() {                            //Prints order
    cout << "Name: " << this->customer << endl << endl;
    for (int i = 0; i < this->pizza_amount; i++) {
        cout << this->pizza_order[i]->get_name() << "(";
        for (int j = 0; j < 6; j++) {
            if (this->pizza_order[i]->get_ingredients()[j] != "")
                cout << this->pizza_order[i]->get_ingredients()[j] << ", ";
        }
        cout << ")" << endl;
        cout << "size: " << this->pizza_order[i]->get_size() << ", crust: " << this->pizza_order[i]->get_crust_type() << endl << endl;
    }
    if (drink != NULL) {
        Drinks* Drinksptr = new Drinks;
        for (int i = 0; i < drink_amount; i++) {
            if (this->drink[i] == 1)
                Drinksptr->drink_type[0] += 1;         //cola
            else if (this->drink[i] == 2)
                Drinksptr->drink_type[1] += 1;         //soda
            else if (this->drink[i] == 3)
                Drinksptr->drink_type[2] += 1;         //ice tea
            else
                Drinksptr->drink_type[3] += 1;         //fruit juice
        }
        for (int i = 0; i < 4; i++)
            if (Drinksptr->drink_type[i] != 0)
                cout << Drinksptr->drink_type[i] << " " << Drinksptr->drink_names[i] << ", ";
    }
    cout << endl;
}

//-----------------------------------------------------------------------------------------------------

class OrderList {
    int n;                                //number of orders
    Order* head;
public:
    void takeOrder();
    void listOrders();
    void deliverOrders();
    OrderList();                          //default constructor
    ~OrderList();                         //destructor
};
OrderList::OrderList() {                  //default constructor
    n = 0;
    head = NULL;
}
OrderList::~OrderList() {                 //destructor
    delete head;
}
void OrderList::takeOrder() {
    cout << "Pizza Menu" << endl;
    cout << "1. Chicken Pizza (mozarella, chicken, mushroom, corn, olive, onion, tomato)" << endl;
    cout << "2. Broccoli Pizza (mozarella, broccoli, pepperoni, olive, mushroom, onion)" << endl;
    cout << "3. Sausage Pizza (mozarealla, sausage, tomato, olive, mushroom, corn" << endl;
    cout << "0. Back to main menu" << endl;
    int Pizza_type;
    cin >> Pizza_type;
    while (Pizza_type < 0 || Pizza_type>3) {
        cout << "Please choose a valid number." << endl;
        cin >> Pizza_type;
    }
    if (Pizza_type == 0) {
        return;
    }
    n++;                                                                                    //if user choose a pizza, order number(n) is incrementing by 1 
    cout << "Select size: small (15 TL), medium (20 TL), big (25 TL)" << endl;
    string Size;
    cin >> Size;
    while (Size != "small" && Size != "medium" && Size != "big") {
        cout << "Type error. Please write which size you want for your pizza." << endl;
        cin >> Size;
    }
    cout << "Select crust type: thin, normal, thick" << endl;
    string Crust_type;
    cin >> Crust_type;
    while (Crust_type != "thin" && Crust_type != "normal" && Crust_type != "thick") {
        cout << "Type error. Please write which crust type you want for your pizza." << endl;
        cin >> Crust_type;
    }
    cout << "Enter the amount:";
    int amount;
    cin >> amount;
    while (amount < 1) {
        cout << "Please give the number of amounts more than 0." << endl;
        cin >> amount;
        cout << endl;
    }
    cout << endl;
    Pizza** store_pizzas = new Pizza * [amount];                                           //storing pizzas
    store_pizzas[0] = new Pizza(Size, Crust_type, Pizza_type);
    for (int i = 1; i < amount; i++) {                                                     //if the amount is more than 1 copy constructors work
        store_pizzas[i] = new Pizza(*store_pizzas[0]);
    }
    cout << "Please choose a drink:" << endl;
    cout << "0. no drink" << endl;
    cout << "1. cola 4TL" << endl;
    cout << "2. soda 2TL" << endl;
    cout << "3. ice tea 3 TL" << endl;
    cout << "4. fruit juice 3.5 TL" << endl;
    cout << "Press -1 to save your order" << endl;

    int drink_choice = 100;
    int drink_number = 0;
    int* store_drinks = (int*)malloc(1 * sizeof(int));
    while (drink_choice != -1) {
        cin >> drink_choice;
        if (drink_choice == -1 || drink_choice == 0)
            break;
        else {
            while (drink_choice < -1 || drink_choice >4) {
                cout << "Please choose a valid number." << endl;
                cin >> drink_choice;
            }
            store_drinks[drink_number] = drink_choice;
            drink_number++;
            store_drinks = (int*)realloc(store_drinks, (drink_number + 1) * sizeof(int));    //With every loop circle i allocate 1 more memmory to store_drinks
        }
    }
    string name;
    cout << "Please enter your name:" << endl;
    cin >> name;
    Order* iter = head;                                               //iter will show me the adress of the working order
    if (drink_choice == 0) {                                        //if there is no drink 
        free(store_drinks);                                         //if there is no drink created memory for drinks should be removed
        if (head == NULL) {                                         //if the linked list is empty
            iter = new Order(name, store_pizzas, amount);
            iter->next = NULL;
            head = iter;
        }
        else {                                                      //if the linked list is not empty
            while (iter->next != NULL) {                            //if iter shows the NULL it stops
                iter = iter->next;
            }
            iter->next = new Order(name, store_pizzas, amount);
            iter->next->next = NULL;
        }
    }
    else {                                                          //if there is at least 1 drink
        if (head == NULL) {                                         //if the linked list is empty
            iter = new Order(name, store_pizzas, store_drinks, amount, drink_number);
            iter->next = NULL;
            head = iter;
        }
        else {                                                      //if the linked list is not empty
            while (iter->next != NULL) {                            //if iter shows the NULL it stops
                iter = iter->next;
            }
            iter->next = new Order(name, store_pizzas, store_drinks, amount, drink_number);
            iter->next->next = NULL;
        }
    }
    cout << endl << "Your order is saved, it will be delivered when it is ready..." << endl;
    if (head->next == NULL)
        iter->printOrder();
    else
        iter->next->printOrder();
    return;
}
void OrderList::listOrders() {              //lists orders
    Order* iter = head;
    if (head == NULL)
        cout << "List is empty" << endl;
    else {
        int i = 1;
        while (iter != NULL) {
            cout << i++ << endl;
            iter->printOrder();
            iter = iter->next;
        }
    }
    return;
}

void OrderList::deliverOrders() {         //prints the order list then make the user choose which ordername will be delivered
    listOrders();                         //calling ListOrders() function to print the list
    if (head == NULL) {
        return;
    }
    Order* iter = head;
    Order* temp = head;
    cout << "Please write the customer name in order to deliver his/her order: ";
    string name;
    cin >> name;
    int counter = 0;
    while (iter->get_customer() != name) {
        iter = iter->next;
        counter++;
        if (iter == NULL) {
            cout << "Wrong name" << endl;
            return;
        }
    }
    iter->printOrder();
    float price = iter->getPrice();
    cout << "Total price: " << price << endl;
    string code = "I am a student";
    string your_code;
    while (your_code != code) {
        cout << "Do you have a promotion code? (y/n)" << endl;
        char is_code;
        cin >> is_code;

        while (is_code != 'y' && is_code != 'n') {
            cout << "Wrong letter. Please type y or n" << endl;
            cin >> is_code;
        }
        if (is_code == 'y') {
            cout << "Please enter your code: ";
            cin.ignore();
            getline(cin, your_code);
            if (your_code == code) {
                float discounted_price = iter->getPrice() * 0.9;
                cout << "Discounted price: " << discounted_price << endl;
                break;
            }
            else {
                cout << "Promotion code is not accepted." << endl;
            }
        }
        else {              //if is_code is n 
            break;
        }
    }
    if (head->next==NULL) {     //if there is only 1 order
        delete head;
        n--;
        head = NULL;
    }
    else if (iter == head) {    //if there is more than 1 order and the selected order is the first one
        head = head->next;
        delete iter;
        n--;
    }                           
    else {                      
        for (int i = 0; i < counter - 1; i++)   // temp is moving next times of counter - 1 to stop before the iter
            temp = temp->next;
        temp->next = temp->next->next;
        delete iter;
        n--;
    }
    cout << "The order is delivered succesfully!" << endl << endl;
}


int main()
{
    OrderList obj;
    while (true) {
        cout << "Welcome to the unicorn pizza!" << endl;
        cout << "1. Add an order" << endl;
        cout << "2. List orders" << endl;
        cout << "3. Deliver order" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose what to do: ";
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            obj.takeOrder();
            break;
        case 2:
            obj.listOrders();
            break;
        case 3:
            obj.deliverOrders();
            break;
        case 4:
            cout << "Goodbye...";
            return 0;
        default:
            cout << "Please give a valid number of choice." << endl;
            break;
        }
    }
    return 0;
}
