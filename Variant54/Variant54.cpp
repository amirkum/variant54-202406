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
    string surname, name, patronymic; // ФИО
    short dd, mm, yyyy; // Дата рождения
    short entranceYear; // Год поступления в институт
    string faculty; // Факультет
    string department; // Кафедра
    string group; // Группа
    string recordBook; // Номер зачетной книжки
    Gender gender; // Пол
    subject** journal; // Журнал (9 сессий и 10 предметов в каждой сессии)
    
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

    // Дата рождения в формате dd.mm.yyyy
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
    
    // Установить дату рождения в формате dd.mm.yyyy
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
        return gender == Gender::MAN ? "М" : "Ж";
    }
    
    // Определение пола по символам М,м,Ж,ж
    void setGender(string gender) {
        this->gender = gender == "М" || gender == "м" ? Gender::MAN : Gender::WOMAN;
    }

    string toString(string delimeter) {
        return surname + delimeter + name + delimeter + patronymic + delimeter + getBirthDate() + delimeter + to_string(entranceYear) + delimeter + faculty + delimeter + department + delimeter + group + delimeter + recordBook + delimeter + getGender() ;
    }
};

void Crypt(string fileName);
void Decrypt(string fileName);

// загрузка студентов из файла
vector<Student*>* load_students_from_file(string fileName) {
    setlocale(LC_ALL, "ru");

    // Дешифровка файла
    Decrypt(fileName);

    vector<Student*> *students = new vector<Student*>;
    ifstream fin;
    fin.open(fileName);
    
    if (!fin.is_open()) {
        cerr << "Не удалось открыть файл " + fileName << endl;
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
        cout << "Загрузка информации из файла успешно произведена" << endl;
    }
    
    fin.close();

    // Шифровка файла
    Crypt(fileName);

    return students;
}

// сохранение списка студентов в файл
void save_students_to_file(string fileName, vector<Student*> *students) {
    setlocale(LC_ALL, "ru");

    // Дешифровка файла
    Decrypt(fileName);

    ofstream fout;
    fout.open(fileName);
    
    if (!fout.is_open()) {
        cerr << "Не удалось открыть файл " + fileName << endl;
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

    // Шифровка файла
    Crypt(fileName);
}

Student *load_student_from_keyboard() {
    Student *student = new Student();
    string birthDate;
    string entranceYear;
    string gender;
    cout << "Введите фамилию: "; cin >> student->surname;
    cout << "Введите имя: "; cin >> student->name;
    cout << "Введите отчество: "; cin >> student->patronymic;
    
    cout << "Введите дату рождения (dd.mm.yyyy): "; cin >> birthDate;
    while (!student->setBirhDate(birthDate)) {
        cerr << "Ошибка: неверная дата рождения" << endl;
        cerr << "Введите корректную дату рождения (dd.mm.yyyy): ";
        cin >> birthDate;
    }
    
    cout << "Введите год поступления в институт: "; cin >> entranceYear;
    while (!student->setEntranceYear(entranceYear)) {
        cerr << "Ошибка: неверный год поступления в институт" << endl;
        cerr << "Введите корректный год поступления в институт: ";
        cin >> entranceYear;
    }
    
    cout << "Введите факультет: "; cin >> student->faculty;
    cout << "Введите кафедру: "; cin >> student->department;
    cout << "Введите группу: "; cin >> student->group;
    cout << "Введите номер зачетной книжки: "; cin >> student->recordBook;
    cout << "Введите пол (М/Ж или м/ж): "; cin >> gender;
    
    while (!(gender == "М" || gender == "м" || gender == "Ж" || gender == "ж")) {
        cerr << "Ошибка: неверный пол" << endl;
        cerr << "Введите корректный пол (М/Ж или м/ж): ";
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
    // создание журнала с оценками для каждого студента по сессиям
    for (vector<Student*>::iterator it = students->begin(); it != students->end(); ++it) {
        for (int session = 0; session < 9; ++session)
        {
            for (int subject = 0; subject < 10; ++subject)
            {
                // Название предмета
                (*it)->journal[session][subject].name = "Предмет_" + to_string(subject + 1);
                // Случайная оценка от 1 до 6
                (*it)->journal[session][subject].mark = ((double)rand() / RAND_MAX) * (6 - 1) + 1;
            }
        }
    }

    vector <StudentWithAvgMark> *list = new vector <StudentWithAvgMark>();
    string group;
    short session;
    string gender;

    cout << "Введите название группы: "; cin >> group;
    cout << "Введите номер сессии от 1 до 9 (0 все сессии): "; cin >> session;
    while (session < 0 || session > 9) {
        cerr << "Ошибка: неверный номер сессии" << endl;
        cerr << "Введите корректный номер сессии от 1 до 9 (0 все сессии): ";
        cin >> session;
    }

    cout << "Введите пол (М/Ж или м/ж): "; cin >> gender;
    while (!(gender == "М" || gender == "м" || gender == "Ж" || gender == "ж")) {
        cerr << "Ошибка: неверный пол" << endl;
        cerr << "Введите корректный пол (М/Ж или м/ж): ";
        cin >> gender;
    }

    for (vector<Student*>::iterator it = students->begin(); it != students->end(); ++it) {
        transform(gender.begin(), gender.end(), gender.begin(), ::toupper);
        if ((*it)->group == group && (*it)->getGender() == gender) {
            list->push_back({ (*it), 0 });
        }
    }

    if (session == 0) {
        // проходим по каждой сессии
        for (int session = 0; session < 9; ++session) {
            // подсчитываем среднюю оценку у каждого студента в текущей сессии
            for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
                int sumMark = 0;
                // подсчитываем сумму оценок по всем предметам
                for (int subject = 0; subject < 10; ++subject) {
                    sumMark += (*it).student->journal[session][subject].mark;
                }
                // вычисляем среднюю оценку
                (*it).avgMarkInSession = sumMark / 10;
            }
            sort(list->begin(), list->end(), [](StudentWithAvgMark x, StudentWithAvgMark y) {
                return x.avgMarkInSession < y.avgMarkInSession;
                });
            cout << "- сессия " << session + 1 << " - успеваемость" << endl;
            for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
                cout << (*it).student->toString(" | ") << " - средняя оценка: " << (*it).avgMarkInSession << endl;
            }
        }
    } else {
        // подсчитываем среднюю оценку у каждого студента в текущей сессии
        for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
            int sumMark = 0;
            // подсчитываем сумму оценок по всем предметам
            for (int subject = 0; subject < 10; ++subject) {
                sumMark += (*it).student->journal[session][subject].mark;
            }
            // вычисляем среднюю оценку
            (*it).avgMarkInSession = sumMark / 10;
        }
        sort(list->begin(), list->end(), [](StudentWithAvgMark x, StudentWithAvgMark y) {
            return x.avgMarkInSession < y.avgMarkInSession;
         });
        cout << "- семестр " << session + 1 << " - успеваемость" << endl;
        for (vector<StudentWithAvgMark>::iterator it = list->begin(); it != list->end(); ++it) {
            cout << (*it).student->toString(" | ") << " - средняя оценка: " << (*it).avgMarkInSession << endl;
        }
    }

    delete list;
}

int main(int argc, const char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Добро пожаловать в систему пользователь" << endl;

    vector<Student*> *students = new vector<Student*>;
    
    int command = -1;
    while (command != 0) {
        cout << "===== Список команд =====" << endl;
        cout << "0: выход" << endl;
        cout << "1: загрузить список студентов из файла" << endl;
        cout << "2: вывести список студентов" << endl;
        cout << "3: добавить студента" << endl;
        cout << "4: сохранить список студентов в файл" << endl;
        cout << "5: информация по студенту" << endl;
        cout << "6: удалить студента" << endl;
        cout << "7: индивидуальное задание" << endl;

        cout << "Введите номер команды: ";
        cin >> command;

        switch (command) {
            case 0: break;
            case 1:
                students->clear();
                students = load_students_from_file("students.txt");
                break;
            case 2:
                if (students->size() == 0) {
                    cerr << "Список студентов пуст" << endl;
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
                cout << "Введите номер зачетной книжки: "; cin >> recordBook;
                vector<Student*>::iterator it;
                for (it = students->begin(); it != students->end(); ++it) {
                    if ((*it)->recordBook == recordBook) {
                        cout << (*it)->toString(" | ") << endl;
                        break;
                    }
                }
                if (it == students->end()) {
                    cerr << "Зачетная книжка не найдена" << endl;
                }
                break;
            }
            case 6: {
                string recordBook;
                cout << "Введите номер зачетной книжки: "; cin >> recordBook;
                vector<Student*>::iterator it;
                bool find = false;
                for (it = students->begin(); it != students->end(); ++it) {
                    if ((*it)->recordBook == recordBook) {
                        it = students->erase(it);
                        find = true;
                        cout << "Студент удален из списка" << endl;
                        break;
                    }
                }
                if (!find) {
                    cerr << "Зачетная книжка не найдена" << endl;
                }
                break;
            }
            case 7:
                variant54(students);
                break;
            default:
                cerr << "Ошибка: неверный номер команды" << endl;
                cerr << "Введите корректный номер команды: ";
                cin >> command;
        }
    }

    students->clear();
    delete students;

    return 0;
}

// Шифрование файла
void Crypt(string fileName)
{
    // генерация случайного пароля
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
    
    // шифруем файл со студентами при помощи сгенерированного пароля
    string command = "openssl.exe enc -aes-256-cbc -salt -in " + fileName + " -out " + fileName + ".enc -pass pass:";
    command += pass;
    system(command.c_str());
    
    // удаляем обычный файл со студентами
    if (remove(fileName.c_str()) != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    ofstream file;
    // создаем файл и сохраняем пароль
    file.open("key.txt", ios::binary);
    file.write(pass, 65);
    file.close();
    // шифруем файл с паролем при помощи открытого ключа
    command = "openssl.exe rsautl -encrypt -inkey rsa.public -pubin -in key.txt -out key.txt.enc";
    system(command.c_str());
    // удаляем обычный файл с паролем
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}

// дешифровка
void Decrypt(string fileName)
{
    // сначала дешифруем файл с паролем при помощи закрытого ключа
    string command = "openssl.exe rsautl -decrypt -inkey rsa.private -in key.txt.enc -out key.txt";
    system(command.c_str());
    // удаляем зашифрованный файл с паролем
    if (remove("key.txt.enc") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    char* pass = new char[64];
    ifstream file;
    // читаем пароль из дешифрованного файла
    file.open("key.txt", ios::binary);
    file.read(pass, 65);
    file.close();
    // удаляем дешифрованный файл с паролем
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    // при помощи полученного пароля дешифруем файл со студентами
    command = "openssl.exe enc -aes-256-cbc -d -in " + fileName + ".enc -out " + fileName + " -pass pass:";
    command += pass;
    system(command.c_str());
    // удаляем шифрованый файл со студентами
    if (remove((fileName + ".enc").c_str()) != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}
