// Variant54.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;

enum Gender {
    MAN,
    WOMAN
};

struct subject {
    string name;
    short mark;
};

class Student {
public:
    string surname, name, patronymic; // ���
    short dd, mm, yyyy; // ���� ��������
    short entranceYear; // ��� ����������� � ��������
    string faculty; // ���������
    string department; // �������
    string group; // ������
    string recordBook; // ����� �������� ������
    Gender gender; // ���
    subject** journal; // ������ (9 ������ � 10 ��������� � ������ ������)
    
    Student() {
        this->journal = new subject*[9];
        for (int i = 0; i < 9; ++i) {
            this->journal[i] = new subject[10];
        }
    }
    
    Student(string surname, string name, string patronymic, string birthDate, short entranceYear, string faculty, string department, string group, string recordBook, string gender) {
        Student();
        this->surname = surname;
        this->name = name;
        this->patronymic = patronymic;
        setBirhDate(birthDate);
        this->entranceYear = entranceYear;
        this->faculty = faculty;
        this->department = department;
        this->group = group;
        this->recordBook = recordBook;
        setGender(gender);
    }

    ~Student() {
        for (int i = 0; i < 9; ++i) {
            delete[] this->journal[i];
        }
        delete[] this->journal;
    }

    string getFullName() {
        return surname + " " + name + " " + patronymic;
    }

    // ���� �������� � ������� dd.mm.yyyy
    string getBirthDate() {
        string dd = to_string(this->dd);
        if (dd.length() == 1) {
            dd = "0" + dd;
        }
        string mm = to_string(this->mm);
        if (mm.length() == 1) {
            mm = "0" + mm;
        }
        return dd + "." + mm + "." + to_string(yyyy);
    }
    
    // ���������� ���� �������� � ������� dd.mm.yyyy
    bool setBirhDate(string birthDate) {
        tm tm;
        istringstream ss(birthDate);
        ss >> get_time(&tm, "%d.%m.%Y");
        if (!ss.fail()) {
            this->dd = tm.tm_mday;
            this->mm = tm.tm_mon + 1;
            this->yyyy = tm.tm_year + 1900;
        }
        return !ss.fail();
    }
    
    bool setEntranceYear(string entranceYear) {
        tm tm;
        istringstream ss(entranceYear);
        ss >> get_time(&tm, "%Y");
        if (!ss.fail()) {
            this->entranceYear = tm.tm_year + 1900;
        }
        return !ss.fail();
    }
    
    string getGender() {
        return gender == Gender::MAN ? "�" : "�";
    }
    
    // ����������� ���� �� �������� �,�,�,�
    void setGender(string gender) {
        this->gender = gender == "�" || gender == "�" ? Gender::MAN : Gender::WOMAN;
    }

    string toString(string delimeter) {
        return surname + delimeter + name + delimeter + patronymic + delimeter + getBirthDate() + delimeter + to_string(entranceYear) + delimeter + faculty + delimeter + department + delimeter + group + delimeter + recordBook + delimeter + getGender() ;
    }
};

void Crypt(string fileName);
void Decrypt(string fileName);

// �������� ��������� �� �����
vector<Student*>* load_students_from_file(string fileName) {
    setlocale(LC_ALL, "ru");

    // ���������� �����
    Decrypt(fileName);

    vector<Student*> *students = new vector<Student*>;
    ifstream fin;
    fin.open(fileName);
    
    if (!fin.is_open()) {
        cerr << "�� ������� ������� ���� " + fileName << endl;
    } else {
        while (!fin.eof()) {
            string line;
            string column[10];
            getline(fin, line);
            
            size_t pos_begin = 0, pos_end;
            
            for (int i = 0; i < 10; ++i) {
                pos_end = line.find(";", pos_begin);
                column[i] = line.substr(pos_begin, pos_end - pos_begin);
                pos_begin = pos_end + 1;
            }
            
            Student *student = new Student();
            student->surname = column[0];
            student->name = column[1];
            student->patronymic = column[2];
            student->setBirhDate(column[3]);
            student->setEntranceYear(column[4]);
            student->faculty = column[5];
            student->department = column[6];
            student->group = column[7];
            student->recordBook = column[8];
            student->setGender(column[9]);
            students->push_back(student);
        }
        cout << "�������� ���������� �� ����� ������� �����������" << endl;
    }
    
    fin.close();

    // �������� �����
    Crypt(fileName);

    return students;
}

// ���������� ������ ��������� � ����
void save_students_to_file(string fileName, vector<Student*> *students) {
    setlocale(LC_ALL, "ru");

    // ���������� �����
    Decrypt(fileName);

    ofstream fout;
    fout.open(fileName);
    
    if (!fout.is_open()) {
        cerr << "�� ������� ������� ���� " + fileName << endl;
    } else {
        fout.clear();
        for (vector<Student*>::iterator it = students->begin(); it != students->end(); ) {
            fout << (*it)->toString(";");
            ++it;
            if (it != students->end()) {
                fout << endl;
            }
        }
    }
    fout.close();

    // �������� �����
    Crypt(fileName);
}

Student *load_student_from_keyboard() {
    Student *student = new Student();
    string birthDate;
    string entranceYear;
    string gender;
    cout << "������� �������: "; cin >> student->surname;
    cout << "������� ���: "; cin >> student->name;
    cout << "������� ��������: "; cin >> student->patronymic;
    
    cout << "������� ���� �������� (dd.mm.yyyy): "; cin >> birthDate;
    while (!student->setBirhDate(birthDate)) {
        cerr << "������: �������� ���� ��������" << endl;
        cerr << "������� ���������� ���� �������� (dd.mm.yyyy): ";
        cin >> birthDate;
    }
    
    cout << "������� ��� ����������� � ��������: "; cin >> entranceYear;
    while (!student->setEntranceYear(entranceYear)) {
        cerr << "������: �������� ��� ����������� � ��������" << endl;
        cerr << "������� ���������� ��� ����������� � ��������: ";
        cin >> entranceYear;
    }
    
    cout << "������� ���������: "; cin >> student->faculty;
    cout << "������� �������: "; cin >> student->department;
    cout << "������� ������: "; cin >> student->group;
    cout << "������� ����� �������� ������: "; cin >> student->recordBook;
    cout << "������� ��� (�/� ��� �/�): "; cin >> gender;
    
    while (!(gender == "�" || gender == "�" || gender == "�" || gender == "�")) {
        cerr << "������: �������� ���" << endl;
        cerr << "������� ���������� ��� (�/� ��� �/�): ";
        cin >> gender;
    }
    
    student->setGender(gender);
    
    return student;
}

struct StudentWithAvgMark {
    Student *student;
    double avgMarkInSession;
};

void variant54(vector<Student*> *students) {
    // �������� ������� � �������� ��� ������� �������� �� �������
    for (vector<Student*>::iterator it = students->begin(); it != students->end(); ++it) {
        for (int session = 0; session < 9; ++session)
        {
            for (int subject = 0; subject < 10; ++subject)
            {
                // �������� ��������
                (*it)->journal[session][subject].name = "�������_" + to_string(subject + 1);
                // ��������� ������ �� 1 �� 6
                (*it)->journal[session][subject].mark = ((double)rand() / RAND_MAX) * (6 - 1) + 1;
            }
        }
    }

    vector <StudentWithAvgMark> *list = new vector <StudentWithAvgMark>();
    string group;
    short session;
    string gender;

    cout << "������� �������� ������: "; cin >> group;
    cout << "������� ����� ������ �� 1 �� 9 (0 ��� ������): "; cin >> session;
    while (session < 0 || session > 9) {
        cerr << "������: �������� ����� ������" << endl;
        cerr << "������� ���������� ����� ������ �� 1 �� 9 (0 ��� ������): ";
        cin >> session;
    }

    cout << "������� ��� (�/� ��� �/�): "; cin >> gender;
    while (!(gender == "�" || gender == "�" || gender == "�" || gender == "�")) {
        cerr << "������: �������� ���" << endl;
        cerr << "������� ���������� ��� (�/� ��� �/�): ";
        cin >> gender;
    }

    for (vector<Student*>::iterator it = students->begin(); it != students->end(); ++it) {
        transform(gender.begin(), gender.end(), gender.begin(), ::toupper);
        if ((*it)->group == group && (*it)->getGender() == gender) {
            list->push_back({ (*it), 0 });
        }
    }

    if (session == 0) {
        // �������� �� ������ ������
        for (int session = 0; session < 9; ++session) {
            // ������������ ������� ������ � ������� �������� � ������� ������
            for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
                int sumMark = 0;
                // ������������ ����� ������ �� ���� ���������
                for (int subject = 0; subject < 10; ++subject) {
                    sumMark += (*it).student->journal[session][subject].mark;
                }
                // ��������� ������� ������
                (*it).avgMarkInSession = sumMark / 10;
            }
            sort(list->begin(), list->end(), [](StudentWithAvgMark x, StudentWithAvgMark y) {
                return x.avgMarkInSession < y.avgMarkInSession;
                });
            cout << "- ������ " << session + 1 << " - ������������" << endl;
            for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
                cout << (*it).student->toString(" | ") << " - ������� ������: " << (*it).avgMarkInSession << endl;
            }
        }
    } else {
        // ������������ ������� ������ � ������� �������� � ������� ������
        for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
            int sumMark = 0;
            // ������������ ����� ������ �� ���� ���������
            for (int subject = 0; subject < 10; ++subject) {
                sumMark += (*it).student->journal[session][subject].mark;
            }
            // ��������� ������� ������
            (*it).avgMarkInSession = sumMark / 10;
        }
        sort(list->begin(), list->end(), [](StudentWithAvgMark x, StudentWithAvgMark y) {
            return x.avgMarkInSession < y.avgMarkInSession;
         });
        cout << "- ������� " << session + 1 << " - ������������" << endl;
        for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
            cout << (*it).student->toString(" | ") << " - ������� ������: " << (*it).avgMarkInSession << endl;
        }
    }

    delete list;
}

int main(int argc, const char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "����� ���������� � ������� ������������" << endl;

    vector<Student*> *students = new vector<Student*>;
    
    int command = -1;
    while (command != 0) {
        cout << "===== ������ ������ =====" << endl;
        cout << "0: �����" << endl;
        cout << "1: ��������� ������ ��������� �� �����" << endl;
        cout << "2: ������� ������ ���������" << endl;
        cout << "3: �������� ��������" << endl;
        cout << "4: ��������� ������ ��������� � ����" << endl;
        cout << "5: ���������� �� ��������" << endl;
        cout << "6: ������� ��������" << endl;
        cout << "7: �������������� �������" << endl;

        cout << "������� ����� �������: ";
        cin >> command;

        switch (command) {
            case 0: break;
            case 1:
                students->clear();
                students = load_students_from_file("students.txt");
                break;
            case 2:
                if (students->size() == 0) {
                    cerr << "������ ��������� ����" << endl;
                } else {
                    for (vector<Student*>::iterator it = students->begin(); it != students->end(); ++it) {
                        cout << (*it)->toString(" | ") << endl;
                    }
                }
                break;
            case 3: {
                Student *student = load_student_from_keyboard();
                students->push_back(student);
                break;
            }
            case 4:
                save_students_to_file("students.txt", students);
                break;
            case 5: {
                string recordBook;
                cout << "������� ����� �������� ������: "; cin >> recordBook;
                vector<Student*>::iterator it;
                for (it = students->begin(); it != students->end(); ++it) {
                    if ((*it)->recordBook == recordBook) {
                        cout << (*it)->toString(" | ") << endl;
                        break;
                    }
                }
                if (it == students->end()) {
                    cerr << "�������� ������ �� �������" << endl;
                }
                break;
            }
            case 6: {
                string recordBook;
                cout << "������� ����� �������� ������: "; cin >> recordBook;
                vector<Student*>::iterator it;
                bool find = false;
                for (it = students->begin(); it != students->end(); ++it) {
                    if ((*it)->recordBook == recordBook) {
                        it = students->erase(it);
                        find = true;
                        cout << "������� ������ �� ������" << endl;
                        break;
                    }
                }
                if (!find) {
                    cerr << "�������� ������ �� �������" << endl;
                }
                break;
            }
            case 7:
                variant54(students);
                break;
            default:
                cerr << "������: �������� ����� �������" << endl;
                cerr << "������� ���������� ����� �������: ";
                cin >> command;
        }
    }

    students->clear();
    delete students;

    return 0;
}

// ���������� �����
void Crypt(string fileName)
{
    // ��������� ���������� ������
    srand(time(NULL));
    char* pass = new char[64];
    for (int i = 0; i < 64; ++i) {
        switch (rand() % 3) {
            case 0:
                pass[i] = rand() % 10 + '0';
                break;
            case 1:
                pass[i] = rand() % 26 + 'A';
                break;
            case 2:
                pass[i] = rand() % 26 + 'a';
        }
    }
    
    // ������� ���� �� ���������� ��� ������ ���������������� ������
    string command = "openssl.exe enc -aes-256-cbc -salt -in " + fileName + " -out " + fileName + ".enc -pass pass:";
    command += pass;
    system(command.c_str());
    
    // ������� ������� ���� �� ����������
    if (remove(fileName.c_str()) != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    ofstream file;
    // ������� ���� � ��������� ������
    file.open("key.txt", ios::binary);
    file.write(pass, 65);
    file.close();
    // ������� ���� � ������� ��� ������ ��������� �����
    command = "openssl.exe rsautl -encrypt -inkey rsa.public -pubin -in key.txt -out key.txt.enc";
    system(command.c_str());
    // ������� ������� ���� � �������
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}

// ����������
void Decrypt(string fileName)
{
    // ������� ��������� ���� � ������� ��� ������ ��������� �����
    string command = "openssl.exe rsautl -decrypt -inkey rsa.private -in key.txt.enc -out key.txt";
    system(command.c_str());
    // ������� ������������� ���� � �������
    if (remove("key.txt.enc") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    char* pass = new char[64];
    ifstream file;
    // ������ ������ �� �������������� �����
    file.open("key.txt", ios::binary);
    file.read(pass, 65);
    file.close();
    // ������� ������������� ���� � �������
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    // ��� ������ ����������� ������ ��������� ���� �� ����������
    command = "openssl.exe enc -aes-256-cbc -d -in " + fileName + ".enc -out " + fileName + " -pass pass:";
    command += pass;
    system(command.c_str());
    // ������� ���������� ���� �� ����������
    if (remove((fileName + ".enc").c_str()) != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}
