/****************************************************************
 * This program implements a hash table that will organize
 * any desired number of elements by using the DJBX33A hashing
 * algorithm on a 10-digit Student ID and then taking the modulo
 * of the result. This program uses separate chaining, i.e., this
 * is a linked list implementation and does not use the C++ STL
 * member list. Instead, a Student class accompanies the HashMap
 * class and provides pointer references to memory locations in
 * order to access data members of each student and uses them as
 * "nodes" in the linked list implementation.
 *
 * Version:         1.0.0
 * Release date:    16 May, 2022
 * Author:          Rusty Dean Dillard
 *
 * Subsequent versions will make this more general purpose and
 * not specifically for storing Student objects.
 ****************************************************************/

#include <iostream>     // input/output
#include <iomanip>      // allows for gpa precision manipulation
#include <string>       // string manipulation
#include <random>       // used for mt19337 rng

using std::string;      // global use of std::string due to illegality of class usage of "using"
                        // std::string is also widely used throughout the entire program

#define TABLE_SIZE 97   // predefined table size for quick testing of a large array

/**********************************************************
 * Student class contents by section:
 * 1. Student class private data members
 * 2. Student constructor prototypes
 * 3. Student class method prototypes: getters and setters
 **********************************************************/
class Student{
private:
    // 1.
    string name;                                            // student's name
    string id;                                              // student's id
    unsigned int age;                                       // student's age
    double gpa;                                             // student's gpa
    unsigned long value;                                    // student's hash value
    Student* next;                                          // pointer to the next student with the same hash value
    
public:
    // 2.
    Student(string name, string id, int age,
            double gpa, unsigned long value);               // Overloaded student constructor
    Student();                                              // Null student constructor
    
    // 3.
    string getName() const;                                 // get student's name
    void setName(string n);                                 // set student's name
    string getID() const;                                   // get student's ID
    void setID(string i);                                   // set student's ID
    __attribute__((unused)) unsigned int getAge() const;    // get student's age (unused in this version)
    void setAge(unsigned int a);                            // set student's age
    __attribute__((unused)) double getGPA() const;          // get student's GPA (unused in this version)
    void setGPA(double g);                                  // set student's GPA
    unsigned long getValue() const;                         // get student's hash value
    void setValue(unsigned long val);                       // set the student's hash value
    Student *getNext() const;                               // get the student's next pointer
    void setNext(Student *s);                               // set the student's next pointer
    void display() const;                                   // display student information
};

/***************************************
 * HashMap class contents by section:
 * 1. HashMap class private data members
 * 2. HashMap constructor prototype
 * 3. HashMap destructor prototype
 * 4. HashMap class method prototypes
 ***************************************/
class HashMap {
private:
    // 1.
    Student **table;                                        // HashMap's array of Student pointers
    int size;                                               // HashMap's size
    
public:
    // 2.
    explicit HashMap(int size);                             // construct a HashMap object with a table of size "size"
    
    // 3.
    ~HashMap();                                             // memory management/destructor
    
    // 4.
    void display();                                         // class method to display the hash table's contents
    unsigned int getSize() const;                           // get the size data member of this HashMap object
    void autoPut(Student* s);                               // method to insert a student into the hash table
    void manualPut();                                       // method to gather student info (calls autoPut to insert)
    void existenceQuery();                                  // determines the existence of a Student in the table
};

/***************************
 * Static methods to be used
 ***************************/
static string genID();                                      // method to generate a random 10-digit student id
static string genName();                                    // method to generate a random first and last student name
                                                            // with respect to a male or female name
static int genAge();                                        // method to generate a random student age
static double genGPA();                                     // method to generate a random student gpa
static unsigned long hashID(const string& str);             // method to calculate a student's hash value operating on
                                                            // the 10-digit id that's passed into it
static Student *genStudent(const HashMap& hm);              // method that calls the other gen methods to generate a student
static void displayMenu();

/*******************
 * MAIN METHOD BEGIN
 *******************/
int main() {
    // cin can be used in the main method's scope
    using std::cin;
    
    // initialize a hashmap with the predefined TABLE_SIZE of 97
    HashMap hashMap = HashMap(TABLE_SIZE);

    /*
     * generate a number of students equal to the size of the hash table
     * (collisions are expected and resolved in autoPut())
     */
    for(int i = 0; i < hashMap.getSize(); i++){
        // generate and insert the student into the hashmap
        hashMap.autoPut(genStudent(hashMap));
    }

    hashMap.display();
    
    unsigned int input = 0;
    do{
        displayMenu();
        
        cin >> input;
        
        /***********************
         * SWITCH MENU BEGINNING
         ***********************/
        switch (input){
            case 1:
                hashMap.existenceQuery();
                break;
            case 2:
                hashMap.manualPut();
                break;
            case 3:
                hashMap.autoPut(genStudent(hashMap));
                break;
            case 4:
                hashMap.display();
                break;
            case 5:
                printf("Terminating program. Thank you!\n");
                break;
            default:
                printf("This is an invalid entry. Rerouting to the query. . .\n");
                break;
        }
    }while(input != 5);
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

void Student::setName(string n){
    this->name = std::move(n);
}

// Student ID getter
string Student::getID() const{
    return this->id;
}

//
void Student::setID(string i){
    this->id = std::move(i);
}

// Student age getter (unused in this version)
__attribute__((unused)) unsigned int Student::getAge() const{
    return this->age;
}

void Student::setAge(unsigned int a){
    this->age = a;
}

// Student GPA getter (unused in this version)
__attribute__((unused)) double Student::getGPA() const{
    return this->gpa;
}

void Student::setGPA(double g){
    this->gpa = g;
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

void Student::display() const{
    using std::cout;
    using std::setprecision;
    using std::fixed;
    cout << "Name: ";
    cout.width(20);
    cout << this->name << " | ID: " << this->id << " | Age: " << this->age
         << " | GPA: " << setprecision(2) << fixed << this->gpa << "\n";
}

// Overloaded HashMap constructor
HashMap::HashMap(int size){
    this->size = size;                          // set this hashmap's size to size
    this->table = new Student *[this->size];    // allocate memory for this hashmap's array of students
    for (int i = 0; i < this->size; i++){       // go from 0 to size - 1 in the hash table
        this->table[i] = new Student();         // set this hashmap's table's specific index to a Null Student
        this->table[i]->setValue(i);            // update the student's hash value that's stored in that index
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

void HashMap::manualPut(){
    using std::cin;
    
    // variable creation for data members of student object to be inserted
    string studentID;
    string studentName;
    unsigned int studentAge;
    double studentGPA;
    auto *newStudent = new Student();
    
    printf("We will be entering several student data. Please pay close attention to the prompts.\n"
           "Enter a student name: ");
    cin.ignore(256, '\n');                                  // ignores the next 256 new line characters
    getline(cin, studentName);                              // get line of string input for student's name
    newStudent->setName(studentName);                       // set new student's name
    
    printf("\nEnter a 10-digit student ID: ");
    getline(cin, studentID);                                // get line of string input for student's ID
    
    while(studentID.length() != 10){                        // ensure input is 10 characters long
        printf("That ID wasn't 10 digits long. Please try again.\n"
                "Enter a 10-digit student ID: ");
        getline(cin, studentID);                            // get line of string input again for student's ID
    }
    newStudent->setID(studentID);                           // set new student's name
    newStudent->setValue(hashID(studentID) % this->getSize());  // set new student's hash value
    
    printf("\nEnter the an age over 18: ");
    cin >> studentAge;                                      // get integer input for student's age
    while(studentAge < 18){                                 // ensure input is an integer 18 and over
        printf("That age is too low. Please enter an age over 18: ");
        cin >> studentAge;                                  // get integer input again for student's age
    }
    newStudent->setAge(studentAge);                         // set new student's age
    
    printf("\nEnter a GPA as high as a 4.0: ");
    cin >> studentGPA;                                      // get double floating point input for student's GPA
    while(studentGPA > 4.0){                                // ensure input is 4.0 or less
        printf("That GPA is higher than 4.0. Please try again.\n"
                "Enter a GPA as high as a 4.0: ");
        cin >> studentGPA;                                  // get double floating point input again for student's GPA
    }
    newStudent->setGPA(studentGPA);                         // set new student's GPA
    
    printf("The following student has been added to the hash table: \n"
           "\033[1;34m");
    newStudent->display();                                  // display the new student that was entered
    printf("\033[0m");
    this->autoPut(newStudent);                              // put the new student into the hash table
}

// Method to insert a Student object into the HashMap object
void HashMap::autoPut(Student *s){
    Student *temp = this->table[s->getValue()];             // create a temporary pointer that points to the student at the prescribed hash value
    while (temp->getNext() != nullptr){                     // continue until the next pointer is nullptr
        temp = temp->getNext();                             // traverse the linked list
    }
    temp->setNext(s);                                       // set the student s as the last student in the linked list
}

// method to determine the existence of a student in the hash table
void HashMap::existenceQuery(){
    using std::cout;
    using std::cin;
    string queryID;
    
    cout << "Enter the 10-digit numeric student ID you'd like to search for: ";
    cin >> queryID;                                         // get input of a string of characters for student ID query
    
    while(queryID.length() != 10){                          // ensure a length of 10 digits
        cout << "This is an invalid entry.\n"
                "Enter the 10-digit numeric student ID you'd like to search for: ";
        cin >> queryID;                                     // get input again for student ID query
    }
    unsigned int value = hashID(queryID) % this->size;      // calculate the hash value
    Student *temp = this->table[value];                     // begin search at the first student object at that value
    while(temp != nullptr){                                 // continue searching until the end of linked list
        // student found!!! :D
        if(temp->getID() == queryID){
            cout << "\033[1;34m";
            temp->display();                                // display found student
            cout << "\033[0m";
            break;                                          // leave the while loop to avoid unnecessary work
        }
        
        temp = temp->getNext();                             // jump to next student in linked list
        
        // student not found :(
        if(temp == nullptr){
            cout << "\033[1;31mThis student doesn't exist in this hash table.\n\033[0m";
        }
    }
}

// Method to display the hash table of the HashMap object
void HashMap::display(){
    using std::cout;
    for(int i = 0; i < this->size; i++){
        Student *temp = this->table[i];             // create a temporary pointer equal to the first student pointer in the linked list
        
        cout << "Hash Value: " << this->table[i]->getValue() << "\n";   // print this index's hash value
        
        while(temp != nullptr){                     // continue until the temporary pointer is nullptr
            if(temp->getName() == "null"){          // check if this is the place holding pointer at the beginning of the linked list
                temp = temp->getNext();             // move the pointer forward in the linked list
                continue;                           // jump to the loop condition to avoid unnecessary work
            }
            else{
                temp->display();                    // display the actual student
                temp = temp->getNext();
            }
        }
        
        if(i + 1 != this->size) {
            cout << "\n";                           // add a line at the end of the hash table output for aesthetics
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
    
    string randNums = "9";                          // set all student IDs to start with the number 9
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist9(0, 9);      // set to generate any integer between 0 and 9, inclusive
    for(int i = 0; i < 9; i++){                     // run the generator 9 times for a total of 10 digits
        int randInt = dist9(rng);                   // generate random single-digit integer
        randNums += to_string(randInt);             // append to the randNums string
    }
    return randNums;                                // return the resulting student ID
}

// Method for randomly generating a first and last name
static string genName(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_int_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist41(0, 41);    // set to generate any integer between [0 and 41]
    
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
    
    return first[dist41(rng)] + " " + last[dist41(rng)];    // return the resulting student name
}

// Method for generating a name between 18 and 38
static int genAge(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_int_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist20(0, 20);            // set to generate any integer between [0 and 20]
    return 18 + dist20(rng);
}

// Method for generating a GPA between 2.5 and 4.0
static double genGPA(){
    using std::random_device;
    using std::mt19937;
    using std::uniform_real_distribution;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<double> dist1point5(0, 1.5);  // set to generate any real number between [0 and 1.5]
    return 2.5 + dist1point5(rng);                          // return the resulting student GPA
}

// DJBX33A algorithm for hashing
static unsigned long hashID(const string& str){
    unsigned long hash = 5381;                              // DJBX33A traditional starting point
    for(char i : str){                                      // run loop until reaching the last character of the ID
        unsigned long c = static_cast<unsigned char>(i);    // assign the ASCII value of the character stored in i to c
        hash = hash * 33 + c;                               // DJBX33A hashing algorithm
    }
    return hash;                                            // assign this hash value where called
}

// Method for generating a student using the other gen methods
static Student *genStudent(const HashMap& hm){
    // generate a random student ID
    string id = genID();
    
    // calculate the hash value of the student's ID
    unsigned long hashValue = hashID(id);
    
    // create a new student
    auto newStudent = new Student(genName(), id, genAge(), genGPA(), (hashValue % hm.getSize()));
    
    // return the resulting student
    return newStudent;
}

static void displayMenu(){
    printf("\nMain menu:\n"
           "\033[1;32m1. Search and Display Student\n"
           "2. Manually Insert Student Into Hash Table\n"
           "3. Generate Student and Insert Into Hash Table\n"
           "4. Display Entire Hash Table\n"
           "5. Exit Program\n\033[0m"
           "Please make a selection between 1 and 5: ");
}
