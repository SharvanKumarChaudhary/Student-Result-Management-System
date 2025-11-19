#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int SUBJECT_COUNT = 5;
const string SUBJECTS[SUBJECT_COUNT] = { "OOPS", "DSA", "Python", "Maths", "QAPD-1" };
const string DATA_FILE = "students.txt";

class Student {
private:
    int roll;
    string name;
    int marks[SUBJECT_COUNT];
    int total;
    double percentage;
    char grade;

public:
    Student() : roll(0), total(0), percentage(0.0), grade('F') {}

    void input() {
        cout << "Enter Roll No: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter marks for subjects:\n";
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            cout << SUBJECTS[i] << ": ";
            cin >> marks[i];
        }
        calculate();
    }

    void calculate() {
        total = 0;
        for (int i = 0; i < SUBJECT_COUNT; ++i)
            total += marks[i];
        percentage = total / (double)SUBJECT_COUNT;

        if (percentage >= 90) grade = 'A';
        else if (percentage >= 75) grade = 'B';
        else if (percentage >= 60) grade = 'C';
        else if (percentage >= 40) grade = 'D';
        else grade = 'F';
    }

    void display() const {
        cout << "\nRoll No   : " << roll << "\nName      : " << name << "\nMarks     : ";
        for (int i = 0; i < SUBJECT_COUNT; ++i)
            cout << SUBJECTS[i] << "=" << marks[i] << "  ";
        cout << "\nTotal     : " << total << "\nPercentage: " << percentage << "%\nGrade     : " << grade << "\n";
    }

    int getRoll() const { return roll; }

    void update() {
        cout << "Enter new Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter new marks:\n";
        for (int i = 0; i < SUBJECT_COUNT; ++i) {
            cout << SUBJECTS[i] << ": ";
            cin >> marks[i];
        }
        calculate();
    }

    void saveToFile(ofstream& fout) const {
        fout << roll << " " << name << " ";
        for (int i = 0; i < SUBJECT_COUNT; ++i)
            fout << marks[i] << " ";
        fout << "\n";
    }

    void loadFromFile(ifstream& fin) {
        fin >> roll >> name;
        for (int i = 0; i < SUBJECT_COUNT; ++i)
            fin >> marks[i];
        calculate();
    }
};

class StudentManager {
private:
    vector<Student> students;

public:
    void loadData() {
    ifstream fin(DATA_FILE);
    if (!fin) return;

    while (true) {
        Student s;
        s.loadFromFile(fin);
        if (!fin) break;
        students.push_back(s);
    }

    fin.close();
}


    void saveData() {
        ofstream fout(DATA_FILE);
        for (const auto& s : students)
            s.saveToFile(fout);
        fout.close();
        cout << "Data saved successfully.\n";
    }

    void addStudent() {
        char choice = 'Y';
        while (choice == 'Y' || choice == 'y') {
            Student s;
            s.input();
            students.push_back(s);
            cout << "Add another student? (Y/N): ";
            cin >> choice;
        }
    }

    void updateStudent() {
        int roll;
        cout << "Enter Roll No to update: ";
        cin >> roll;
        for (auto& s : students) {
            if (s.getRoll() == roll) {
                s.update();
                cout << "Student updated.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void listStudents() const {
        for (const auto& s : students)
            s.display();
    }

    void searchStudent() const {
        int roll;
        cout << "Enter Roll No to search: ";
        cin >> roll;
        for (const auto& s : students) {
            if (s.getRoll() == roll) {
                s.display();
                return;
            }
        }
        cout << "Student not found.\n";
    }
};

void teacherMenu(StudentManager& manager) {
    string password;
    cout << "Enter password: ";
    cin >> password;
    if (password != "teacher@123") {
        cout << "Wrong password.\n";
        return;
    }

    while (true) {
        cout << "\n--- Teacher Menu ---\n";
        cout << "1. Add Student\n2. Update Student\n3. List All Students\n";
        cout << "4. Search Student\n5. Save Changes\n6. Return to Main Menu\nOption: ";
        int opt;
        cin >> opt;

        switch (opt) {
            case 1: manager.addStudent(); break;
            case 2: manager.updateStudent(); break;
            case 3: manager.listStudents(); break;
            case 4: manager.searchStudent(); break;
            case 5: manager.saveData(); break;
            case 6: return;
            default: cout << "Invalid option.\n";
        }
    }
}

void studentMenu(const StudentManager& manager) {
    manager.searchStudent();
}

int main() {
    StudentManager manager;
    manager.loadData();

    while (true) {
        cout << "\n--- Student Result Management ---\n";
        cout << "1. Teacher\n2. Student\n3. Exit\nOption: ";
        int role;
        cin >> role;

        switch (role) {
            case 1: teacherMenu(manager); break;
            case 2: studentMenu(manager); break;
            case 3: return 0;
            default: cout << "Invalid option.\n";
        }
    }
}