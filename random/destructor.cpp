#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

class String {
    int size;
    char *contents;
public:
    String(const char*);
    String(const String&);
    void print();
    ~String();
};
String::String(const char *in_data){
    size = strlen(in_data);
    contents = new char[size + 1];
    strcpy(contents,in_data);
}
String::String(const String& object_in) {
    size = object_in.size;
    contents = new char[size + 1];
    strcpy(contents, object_in.contents);   
}
void String::print() {
    cout << contents << "" << size << endl;
}
String::~String() {
    delete[] contents;
}
int main()
{
    String string1("string1");
    String other = string1;
    String more(string1);
    string1.print();
    other.print();
    more.print();
    return 0;   //destructor is called

}
