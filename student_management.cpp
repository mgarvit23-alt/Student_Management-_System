#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ---------------- Student class  ----------------
class Student {
private:
    int rollNo;
    string name;
    float marks;
    int standard;
    char grade;

public:
    
    Student() : rollNo(0), name(""), marks(0),standard(0),grade('F') {}

   
    Student(int r, string n, float m,int st) {
        rollNo = r;
        name = n;
        marks = m;
        standard = st;
        grade = calculateGrade(m);
    }

    // ---- Setters ----
    void setRollNo(int r) { rollNo = r; }
    void setName(string n) { name = n; }
    void setName(int st) { standard = st ;}
    void setMarks(float m) {
        marks = m;
        grade = calculateGrade(m);
    }

    // ---- Getters ----
    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    float getMarks() const { return marks; }
    int getstandard() const{ return standard ;}
    char getGrade() const { return grade; }

    // grade calculation logic
    char calculateGrade(float m) {
        if (m >= 90) return 'A';
        else if (m >= 80) return 'B';
        else if (m >= 60) return 'C';
        else if (m >= 40) return 'D';
        else return 'F';
    }

    // display one student's data
    void display() const {
        cout << "Roll No: " << rollNo
             << " | Name: " << name
             << " | Marks: " << marks
             << " | Class Of Student: "<<standard
             << " | Grade: " << grade << endl;
    }

    // convert student data to a single line for file storage
    // format: rollNo,name,marks
    string toFileString() const {
        return to_string(rollNo) + "," + name + "," + to_string(marks) + "," + to_string(standard);
    }
};

// ---------------- File Handling Functions ----------------
const string FILENAME = "students.txt";

// Add student -> append to file
void addStudent() {
    int r;
    string n;
    float m;
    int st;

    cout << "Enter Roll No: ";
    cin >> r;
    cin.ignore(); // buffer clear karne ke liye (getline se pehle zaroori)
    cout << "Enter Name: ";
    getline(cin, n);
    cout << "Enter Marks: ";
    cin >> m;
    cout<<"Enter Class Of Student: ";
    cin>>st;


    Student s(r, n, m,st);

    ofstream outFile(FILENAME, ios::app); // append mode - purana data delete nahi hoga
    if (!outFile) {
        cout << "File Not Opened!" << endl;
        return;
    }
    outFile << s.toFileString() << endl;
    outFile.close();

    cout << "Student added successfully! Grade: " << s.getGrade() << endl;
}

void displayAll() {
    ifstream inFile(FILENAME);
    if (!inFile) {
        cout << "Koi data nahi mila (file exist nahi karti)." << endl;
        return;
    }

    string line;
    bool found = false;
    cout << "\n--- All Students ---" << endl;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        found = true;

        // line ko comma se split karo: rollNo,name,marks
        int pos1 = line.find(',');
        int pos2 = line.find(',', pos1 + 1);
        int pos3 = line.find(',', pos2 + 1);

        int r = stoi(line.substr(0, pos1));
        string n = line.substr(pos1 + 1, pos2 - pos1 - 1);
        float m = stof(line.substr(pos2 + 1));
        int st = stoi(line.substr(pos3 + 1));  

        Student s(r, n, m, st);
        s.display();
    }

    if (!found) cout << "File khaali hai." << endl;
    inFile.close();
}

// Search student by roll number
void searchStudent() {
    int searchRoll;
    cout << "Enter Rollno. To Search Student: ";
    cin >> searchRoll;

    ifstream inFile(FILENAME);
    if (!inFile) {
        cout << "File Not Found." << endl;
        return;
    }

    string line;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        int pos1 = line.find(',');
        int pos2 = line.find(',', pos1 + 1);
        int pos3 = line.find(',', pos2 + 1);

        int r = stoi(line.substr(0, pos1));

        if (r == searchRoll) {
            string n = line.substr(pos1 + 1, pos2 - pos1 - 1);
            float m = stof(line.substr(pos2 + 1));
            int st = stoi(line.substr(pos3 + 1)); 

            Student s(r, n, m, st);
            s.display();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found) cout << "Roll No " << searchRoll << " nahi mila." << endl;
}

#include <cstdio>   // remove() aur rename() ke liye zaroori

void deleteStudent() {
    int delRoll;
    cout << "Delete karne ke liye Roll No enter kar: ";
    cin >> delRoll;

    ifstream inFile(FILENAME);
    if (!inFile) {
        cout << "File Not Found." << endl;
        return;
    }

    ofstream tempFile("temp.txt");   // naya temporary file
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        int pos1 = line.find(',');
        int r = stoi(line.substr(0, pos1));   // sirf rollNo nikaalna hai check karne ke liye

        if (r == delRoll) {
            found = true;
            continue;    // is line ko temp file me MAT likho -> yehi "delete" hai
        }

        tempFile << line << endl;   // baaki sab lines temp file me copy ho jaayengi
    }

    inFile.close();
    tempFile.close();

    remove(FILENAME.c_str());              // purani file delete
    rename("temp.txt", FILENAME.c_str());  // temp file ko original naam de do

    if (found)
        cout << "Roll No " << delRoll << " ka student delete ho gaya!" << endl;
    else
        cout << "Roll No " << delRoll << " nahi mila." << endl;
}

int main() {
    int choice;

    do {
        cout << "\n===== Student Management System =====" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. Display All Students" << endl;
        cout << "3. Search Student by Roll No" << endl;
        cout << "4. Delete Student by Roll No" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: deleteStudent(); break;
            case 5: cout << "Exiting... Dhanyavaad!" << endl; break;
            default: cout << "No choice Available, Try Againṇ." << endl;
        }
    } while (choice != 5);

    return 0;
}
