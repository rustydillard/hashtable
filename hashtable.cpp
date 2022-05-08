//Name:           Rusty Dillard
//Student ID:     200897416
//Email:          dilla017@cougars.csusm.edu

/***************************************************************
 * This program implements a hash table that will organize
 * any desired number of elements by using the DJBX33A hashing
 * algorithm on a 10-digit Student ID and then taking the modulo
 * of the result. This program uses separate hashing, i.e., this
 * is a linked list implementation and does not use the C++ STL
 * member list. Instead, a Student class accompanies the HashMap
 * class and provides pointer references to memory locations in
 * order to access data members of each student.
 *
 * Version:         1.0.0
 * Release date:    16 May, 2022
 * Author:          Rusty Dean Dillard
 *
 * Subsequent versions will make this more general purpose and
 * not specifically for storing Student objects.
 ***************************************************************/

#include <iostream>     // input/output
#include <string>       // string manipulation
#include <random>       // used for mt19337 rng

using std::string;    // global use of std::string due to illegality of class usage of "using"
                        // also because of the widespread use of std::string.

#define TABLE_SIZE 97   // predefined table size for quick testing of a large array

/*********************************************************
 * Student class contents by section:
 * 1. Student class private data members
 * 2. Student constructor prototypes
 * 3. Student class method prototypes: getters and setters
 *********************************************************/
class Student{
    
    // 1.
    string name;                                    // student's name
    string id;                                      // student's id
    int age;                                        // student's age
    double gpa;                                     // student's gpa
    unsigned long value;                            // student's hash value
    Student* next;                                  // pointer to the next student with the same hash value
    
public:
    
    // 2.
    Student(string name, string id, int age,
            double gpa, unsigned long value);       // Student to be generated and used as an "actual value" in the hash table
    Student();                                      // Null student for the first student in the list
    
    // 3.
    string getName() const;                         // get student's name
    string getID() const;                           // get student's ID
    int getAge() const;                             // get student's age
    double getGPA() const;                          // get student's GPA
    unsigned long getValue() const;                 // get student's hash value
    void setValue(unsigned long val);               // set the student's hash value
    Student *getNext() const;                       // get the student's next pointer
    void setNext(Student *s);                       // set the student's next pointer
};

/***************************************
 * HashMap class contents by section:
 * 1. HashMap class private data members
 * 2. HashMap constructor prototype
 * 3. HashMap destructor prototype
 * 4. HashMap class method prototypes
 ***************************************/
class HashMap {
    
    // 1.
    Student **table;                                // HashMap's array of Student pointers
    int size;                                       // HashMap's size
    
public:
    
    // 2.
    explicit HashMap(int size);                     // construct a HashMap object with a table of size "size"
    
    // 3.
    ~HashMap();                                     // memory management/destructor
    
    // 4.
    void put(Student* s);                           // class method to insert a student into the hash table
    void display();                                 // class method to display the hash table's contents
    unsigned int getSize() const;                   // get the size data member of this HashMap object
};

/***************************
 * Static methods to be used
 ***************************/
static string genID();                              // static method to generate a random 10-digit student id
static string genName();                            // static method to generate a random first and last student name
                                                    // with respect to a male or female name
static int genAge();                                // static method to generate a random student age
static double genGPA();                             // static method to generate a random student gpa
static unsigned long hashID(const string& str);     // static method to calculate a student's hash value operating on
                                                    // the 10-digit id that's passed into it
static Student *genStudent(const HashMap& hm);      // static method that calls the other gen methods to generate a student

/*******************
 * MAIN METHOD BEGIN
 *******************/
int main() {
    // cout can be used in the main method's scope
    using ::std::cout;
    
    cout << "Welcome to Student Information Storage.\n"
            "Let's get the storage space set up.\n"
            "The storage space will be setup to correspond to 97 hash values\n"
            "to be shared by any number of students.\n"
            "To demonstrate its capabilities, we will randomly generate a set\n"
            "of students using the mt19937 pseudorandom number generator.\n"
            "Each student will be placed into their respective hash value slot\n"
            "in the hash table.\n";
    
    // initialize a hashmap with the predefined TABLE_SIZE of 96
    HashMap hashMap = HashMap(TABLE_SIZE);
    
    //
    for(int i = 0; i < hashMap.getSize(); i++){
        // generate and insert the student into the hashmap
        hashMap.put(genStudent(hashMap));
    }
    
    cout << "The hash table will then be displayed.\n\n";
    
    // display the hash table
    hashMap.display();
}
/*****************
 * MAIN METHOD END
 *****************/

// Null Student constructor
Student::Student(){
    this->name = "null";            // set this student's name to null
    this->id = "null";              // set this student's id to null
    this->age = 0;                  // set this student's age to 0
    this->gpa = 0.0;                // set this student's gpa to 0.0
    this->value = 0;                // set this student's hash value to 0 (will be updated later)
    this->next = nullptr;           // set this student's next pointer to nullptr
}

// Overloaded Student constructor
Student::Student(string name, string id, int age, double gpa, unsigned long value){
    this->name = move(name);        // set this student's name to name
    this->id = move(id);            // set this student's id to id
    this->age = age;                // set this student's age to age
    this->gpa = gpa;                // set this student's gpa to gpa
    this->value = value;            // set this student's hash value to value
    this->next = nullptr;           // initially, this student doesn't point to another student so set to nullptr
}

// Student name getter
string Student::getName() const{
    return this->name;
}

// Student ID getter
string Student::getID() const{
    return this->id;
}

// Student age getter
int Student::getAge() const{
    return this->age;
}

// Student GPA getter
double Student::getGPA() const{
    return this->gpa;
}

// Student hash value getter
unsigned long Student::getValue() const{
    return this->value;
}

// Student hash value setter
void Student::setValue(unsigned long val){
    this->value = val;
}

// Student next pointer getter
Student *Student::getNext() const{
    return this->next;
}

// Student next pointer setter
void Student::setNext(Student *s){
    this->next = s;
}

// Overloaded HashMap constructor
HashMap::HashMap(int size){
    this->size = size;                          // set this hashmap's size to size
    this->table = new Student *[this->size];    // allocate memory for this hashmap's array of students
    for (int i = 0; i < this->size; i++){
        this->table[i] = new Student();         // set this hashmap's table's specific index to a Null Student
        this->table[i]->setValue(i);        // update the student's hash value that's stored in that index
    }
}

// HashMap destructor
HashMap::~HashMap(){
    for (int i = 0; i < this->size; i++) {
        if (this->table[i] != nullptr){         // check to make sure we're not attempting to delete a nullptr (avoids seg fault)
            Student *temp1 = this->table[i];    // create a temporary pointer to the first student in the table's index
            while(temp1 != nullptr){            // continue until temp1 is a nullptr
                Student *temp2 = temp1;         // create a second temporary pointer to delete while the other traverses the list
                temp1 = temp1->getNext();       // traverse the linked list of students
                delete temp2;                   // deallocate the student memory
            }
        }
    }
    delete[] this->table;                       // deallocate the table memory
}

// DJBX33A algorithm for hashing
unsigned long hashID(const string& str){
    unsigned long hash = 5381;                              // DJBX33A traditional starting point
    for(char i : str){                                      // run loop until reaching the last character of the ID
        unsigned long c = static_cast<unsigned char>(i);    // assign the ASCII value of the character stored in i to c
        hash = hash * 33 + c;                               // DJBX33A hashing algorithm
    }
    return hash;                                            // assign this hash value where called
}

// Method to insert a Student object into the HashMap object
void HashMap::put(Student *s){
    Student *temp = this->table[s->getValue()];     // create a temporary pointer that points to the student at the prescribed hash value
    while (temp->getNext() != nullptr){             // continue until the next pointer is nullptr
        temp = temp->getNext();                     // traverse the linked list
    }
    temp->setNext(s);                               // set the student s as the last student in the linked list
}

// Method to display the hash table of the HashMap object
void HashMap::display(){
    using std::cout;
    for(int i = 0; i < this->size; i++){
        Student *temp = this->table[i];             // create a temporary pointer equal to the first student pointer in the linked list
        
        cout << "Hash Value: " << this->table[i]->getValue() << "\n";
        
        while(temp != nullptr){                     // continue until the temporary pointer is nullptr
            if(temp->getName() == "null"){          // check if this is the place holding pointer at the beginning of the linked list
                temp = temp->getNext();             // move the pointer forward in the linked list
            }
            else{
                // display all the Student object's data members
                cout << "Name: ";
                cout.width(20);
                cout << temp->getName() << " | ID: " << temp->getID() << " | Age: " << temp->getAge();
                cout << " | GPA: " << temp->getGPA() << "\n";
                temp = temp->getNext();
            }
        }
        
        if(i + 1 != this->size) {
            cout << "\n";
        }
    }
}

// HashMap hash table size getter
unsigned int HashMap::getSize() const{
    return this->size;
}

// Method for randomly generating a 10-digit ID
static string genID(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_int_distribution;
    using std::to_string;
    
    string randNums = "9";
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist9(0, 9);
    for(int i = 0; i < 9; i++){
        int randInt = dist9(rng);
        randNums += to_string(randInt);
    }
    return randNums;
}

// Method for randomly generating a first and last name
static string genName(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_int_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist6(0, 41);
    
    string first[] = {"Brigitte", "Stephanie", "Jennifer", "Yolanda", "Ju", "Fen", "Min",
                      "Nora", "Aria", "Nova", "Jade", "Leilani", "Samantha", "Sage",
                      "Rachel", "Sutton", "Harlow", "Gia", "Amina", "Ariel", "Samara",
                      "Alekzandr", "Rusty", "James", "Fei-hung", "Juan", "Gil",
                      "Charles", "Devin", "Lucas", "Mason", "Logan", "Jackson",
                      "Samuel", "Mateo", "Owen", "Bastian", "Maverick", "Everett",
                       "Kai", "Titan", "Rex"};
    string last[] = {"Dillard", "Smith", "Chi", "Li", "Castro", "Brown", "Huang", "Lee",
                     "Moore", "Lopez", "Gomez", "Wright", "Clark", "Sanchez", "Patel",
                     "Bryant", "Tran", "Nguyen", "Ali", "Mohammed", "Wang", "Cross",
                     "Moss", "Quinn", "Rivas", "Chan", "Manning", "Khan", "Parsons",
                     "Strickland", "Suarez", "Salas", "Cobb", "Moody", "Archer", "Tang",
                     "Odom", "Hendrix", "Good", "Novak", "Lu", "Branch"};
    
    int randFirst = dist6(rng);
    int randLast = dist6(rng);
    return first[randFirst] + " " + last[randLast];
}

// Method for generating a name between 18 and 38
static int genAge(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_int_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist20(0, 20);
    return 18 + dist20(rng);
}

// Method for generating a GPA between 2.5 and 4.0
static double genGPA(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_real_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<double> dist1point5(0, 1.5);
    return 2.5 + dist1point5(rng);
}

// Method for generating a student using the other gen methods
static Student *genStudent(const HashMap& hm){
    string name = genName();                            // generate a random student name
    string id = genID();                                // generate a random student ID
    int age = genAge();                                 // generate a random student age
    double gpa = genGPA();                              // generate a random student GPA
    unsigned long hashValue = hashID(id);               // calculate the hash value of the student's ID
    auto newStudent = new Student(name, id, age, gpa, (hashValue % hm.getSize()));      // create a new student
    return newStudent;
}
