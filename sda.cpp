#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include<iterator>
using namespace std;

void displayMainMenu() {
    cout << "\nComplaint Management System\n";
    cout << "1. Teacher\n";
    cout << "2. Manager\n";
    cout << "3. Employee\n";
    cout << "4. Administrator\n";
    cout << "5. Generate Reports\n";
    cout << "6. Exit\n";
}


class Main;
class Person;
class Teacher;
class Department;
class Complaint;
class State;
class Employee;
bool compareChronologicalOrder(const Complaint* a, const Complaint* b);
void teacherMenu(Main& main);
void managerMenu(Main& main);
void employeeMenu(Main& main);
void adminMenu(Main& main);
void generatereport(Main& main);
class Date
{
    int dd;
    int mm;
    int yy;
public:
    Date(int d, int m, int y)
    {
        dd = d;
        mm = m;
        yy = y;
    }
    void get_date(int& d, int& m, int& y)const
    {
        d = dd;
        m = mm;
        y = yy;
    }
    bool operator<(const Date& other) const
    {
        // Compare years first
        if (yy < other.yy) {
            return true;
        }
        else if (yy > other.yy) {
            return false;
        }

        // If years are equal, compare months
        if (mm < other.mm) {
            return true;
        }
        else if (mm > other.mm) {
            return false;
        }

        // If months are equal, compare days
        return dd < other.dd;
    }
    bool operator<=(const Date& other) const {
        if (yy != other.yy) return yy < other.yy;
        if (mm != other.mm) return mm < other.mm;
        return dd <= other.dd;
    }

    bool operator>=(const Date& other) const {
        if (yy != other.yy) return yy > other.yy;
        if (mm != other.mm) return mm > other.mm;
        return dd >= other.dd;
    }
};
class Assignment
{
    Date date;
    Complaint* complaint;
    Employee* employee;
    int status;
public:
    Assignment(int d, int m, int y, Complaint* c, Employee* e, int status);
    string employeename();
    string getdate() const
    {
        int d, m, y;
        date.get_date(d, m, y);
        string date = to_string(y) + "-" + to_string(m) + "-" + to_string(d);
        return date;
    }
    string getstatus();
    bool getStatus() const
    {
        if (status == 1)
        {
            return true;
        }
        return false;
    }
    string employeename() const;
    int employeeid() const;
    void setstatus(int status)
    {
        this->status = status;
    }
    void viewcomplaints();
};


class Person
{
    int id;
    string name;
public:
    Person(string name, int id) { this->name = name; this->id = id; }
    virtual ~Person() {}
    string getName() const { return name; }
    int getid() const { return id; }
};

class Teacher : public Person
{
    vector<Complaint*> complaints;
public:
    Teacher(string name, int id) :Person(name, id) {}

    void addComplaint(Complaint* complaint) {

        complaints.push_back(complaint);
        sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
    }
    const vector<Complaint*>& getcomplaints()
    {
        return complaints;
    }
    void printComplaints() const;
    ~Teacher() {
        for (auto* complaint : complaints) {
            delete complaint;
        }
    }

};

class Employee : public Person
{
    Department* dept;
    vector<Assignment*> assignments;
public:
    Employee(string name, Department* d, int id) :Person(name, id) { dept = d; }
    Department* getDepartment() const { return dept; }
    void addassignment(Assignment* a)
    {
        assignments.push_back(a);
    }
    bool hasAssignments() const {
        if (!assignments.empty())
        {
            for (const auto& assignment : assignments)
            {
                if (!assignment->getStatus())
                {
                    return true;
                }
            }
        }
        return false;
    }
    void viewcomplaints()
    {
        for (const auto& assignment : assignments)
        {
            assignment->viewcomplaints();
        }
    }
    void printComplaints(const string& status) const;
    ~Employee() {
        for (auto* assignment : assignments) {
            delete assignment;
        }
    }


};

class Manager :public Person
{
    Department* dept;
public:
    Manager(string name, Department* d, int id) :Person(name, id) { dept = d; }
};



class Department
{
    vector<Employee*> emp;
    Manager* mgr;
    vector<Complaint*> complaints;
public:
    void setdept(int id, string name)
    {
        this->emp.push_back(new Employee(name, this, id));
    }
    void addComplaint(Complaint* complaint)
    {
        
        complaints.push_back(complaint);
        sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
    }
    void sortcomplaints()
    {
        sort(complaints.begin(), complaints.end(), compareChronologicalOrder);
    }
    void setManager(int id, string name)
    {
        mgr = new Manager(name, this, id);
    }
    template<typename T>
    void setcomplaint(State* stateInstance, string date, string desc, string filedby)
    {
        this->complaints.push_back(new T(stateInstance, date, desc, filedby, this));
        sortcomplaints();
    }
    const vector<Employee*>& getEmployees()const
    {
        return emp;
    }

    Manager* getManager() const
    {
        return mgr;
    }
    const vector<Complaint*>& getComplaints() const
    {

        return complaints;
    }
    virtual void printEmployees() const {}
    virtual void printManager() const {}
    virtual void printComplaints()const {}
    virtual string getdeptname() const = 0;
    Department* getdept()
    {
        return this;
    }
    void printComplaints(const string& status) const;
    void addassignment(int id, vector<int>e, int d, int m, int y, vector<int>status);
    virtual ~Department()
    {
        delete mgr;
        for (auto& employee : emp) {
            delete employee;
        }
    }
    void removeEmployeeById(int id) {
        for (size_t i = 0; i < emp.size(); ++i) {
            if (emp[i]->getid() == id) {
                if (emp[i]->hasAssignments()) {
                    cout << "Cannot remove employee: Employee is currently assigned to a job." << endl;
                    return;
                }
                for (size_t j = i; j < emp.size() - 1; ++j) {
                    emp[j] = emp[j + 1];
                }
                emp.pop_back();
                return;
            }
        }
    }
    void deletemanager()
    {
        mgr = nullptr;
    }
    bool checkid(int id);
    bool check_personid(int id);
    bool check_managerid(int id);
    void setState(int id, State* state);
    bool printComplaintbyId(int id);
    int checkcomplaint(int sy, int sm, int sd, int ey, int em, int ed);

};

class IT : public Department {
public:
    void printEmployees() const
    {
        cout << "IT Department Employees:" << endl;
        for (const auto& employee : getEmployees())
        {
            cout << employee->getid() << "," << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "IT Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "IT Department has no manager." << endl;
        }
    }
    void printComplaints() const;
    string getdeptname() const
    {
        return "IT";
    }
};

class Admin : public Department
{
public:
    void printEmployees() const
    {
        cout << "Admin Department Complaints:" << endl;
        for (const auto& employee : getEmployees()) {
            cout << employee->getid() << "," << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "Admin Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "Admin Department has no manager." << endl;
        }
    }
    void printComplaints() const;
    string getdeptname() const
    {
        return "Admin";
    }
};

class Accounts : public Department {
public:
    void printEmployees() const
    {
        cout << "Accounts Department Employees:" << endl;
        for (const auto& employee : getEmployees()) {
            cout << employee->getid() << "," << employee->getName() << endl;
        }
    }

    void printManager() const
    {
        if (getManager()) {
            cout << "Accounts Department Manager: " << getManager()->getName() << endl;
        }
        else {
            cout << "Accounts Department has no manager." << endl;
        }
    }

    void printComplaints() const;
    string getdeptname() const
    {
        return "Accounts";
    }
};


class State {
public:
    virtual string handle(Complaint& context) = 0;
    virtual string getStatus() const = 0;
    virtual ~State() {}
};

// Singleton New class
class New : public State {

    static New* instance;
    New() {} // Private constructor

public:
    static New* getInstance() {
        if (!instance)
            instance = new New();
        return instance;
    }

    string handle(Complaint& context) override {
        return "Complaint is open";
    }

    string getStatus() const override {
        return "New";
    }
};

New* New::instance = nullptr;

// Singleton Assigned class
class Assigned : public State {

    static Assigned* instance;
    Assigned() {}

public:
    static Assigned* getInstance() {
        if (!instance)
            instance = new Assigned();
        return instance;
    }

    string handle(Complaint& context) override {
        return "Complaint is in progress";
    }

    string getStatus() const override {
        return "Assigned";
    }
};

Assigned* Assigned::instance = nullptr;

// Singleton Resolved class
class Resolved : public State {

    static Resolved* instance;
    Resolved() {}

public:
    static Resolved* getInstance() {
        if (!instance)
            instance = new Resolved();
        return instance;
    }

    string handle(Complaint& context) override {
        return "Complaint is resolved";
    }

    string getStatus() const override {
        return "Resolved";
    }
};

Resolved* Resolved::instance = nullptr;

// Singleton Closed class
class Closed : public State {

    static Closed* instance;
    Closed() {}

public:
    static Closed* getInstance() {
        if (!instance)
            instance = new Closed();
        return instance;
    }

    string handle(Complaint& context) override {
        return "Complaint is closed";
    }

    string getStatus() const override {
        return "Closed";
    }
};

Closed* Closed::instance = nullptr;
class Complaint
{
    vector<Assignment*>assignments;
    State* currentState;
    Department* dept;
    Teacher* teacher;
    Date date;
    string description;
    int id;
public:
    Complaint(int id, int d, int m, int y, State* state, string desc, Department* dept, Teacher* teach) :date(d, m, y)
    {
        this->id = id;
        currentState = state;
        description = desc;
        this->dept = dept;
        teacher = teach;
        this->dept->addComplaint(this);
        this->teacher->addComplaint(this);
    }

    void setid(int id)
    {
        this->id = id;
    }
    void setState(State* state) {
        currentState = state;
    }

    string processComplaint() const {
        return currentState->handle(const_cast<Complaint&>(*this));
    }
    string getdate() const
    {
        int d, m, y;
        date.get_date(d, m, y);
        string date = to_string(y) + "-" + to_string(m) + "-" + to_string(d);
        return date;
    }
    string getAssignmentDate() const {
        string earliestDate;
        if (!assignments.empty())
        {
            earliestDate = assignments[0]->getdate(); // Assuming getdate() returns the date string
            for (const auto& assignment : assignments) {
                string currentDate = assignment->getdate();
                if (currentDate < earliestDate) {
                    earliestDate = currentDate;
                }
            }
        }

        return earliestDate; // Returns the earliest date among assignments
    }
    const Date& getDate() const
    {
        return date;
    }

    const State* getState() const {
        return currentState;
    }
    string getFiledBy() const {
        if (teacher != nullptr) {
            return to_string(teacher->getid());
        }
        else {
            return "Unknown Teacher"; 
        }
    }
    string getStatus() const {
        return currentState->getStatus();
    }

    bool checkassignmentstatus()
    {
        if (getStatus() == "Assigned")
        {
            for (const auto& assignment : assignments)
            {
                if (!assignment->getStatus())
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }
    string getdesc()
    {
        return description;
    }
    bool setassignmentstatus(int eid, int status)
    {
        for (const auto& assignment : assignments)
        {
            if (assignment->employeeid() == eid)
            {
                assignment->setstatus(status);
                return true;
            }
        }
        return false;
    }
    bool isEmployeeAssigned(int id)const;
    void printComplaint() const
    {
        int d, m, y;
        cout << endl << "\n\nComplaint Details:" << endl;
        cout << "ID: " << id << endl;
        cout << "Date: " << getdate() << endl;
        cout << "Description: " << description << endl;
        cout << "Status: " << currentState->getStatus() << endl;
        cout << "Department: " << dept->getdeptname() << endl;
        cout << "Teacher: " << teacher->getName() << endl;
        if (!assignments.empty())
        {
            Assignment* firstAssignment = assignments.front();
            cout << "Assigned to:" << endl;
            for (const auto& assignment : assignments) {

                cout << assignment->employeename() << endl;
                if (currentState->getStatus() == "Assigned")
                {
                    cout << "Progress : ";
                    if (assignment->getStatus())
                        cout << "Done" << endl;
                    else
                        cout << "In Process" << endl;

                }
            }
            cout << "assigned Date :" << firstAssignment->getdate();
        }
        else
        {
            cout << "Not assigned to anyone yet" << endl;
        }

    }
    string getassignedemployees()
    {
            string employees;
            for (const auto& assignment : assignments)
            {
                if (!employees.empty()) {
                    employees += ","; // Only add the comma if this is not the first entry
                }
                string id = to_string(assignment->employeeid());
                bool st = assignment->getStatus();
                employees += id + ":" + to_string(st);
            }
            return employees;
    }
    void setdescription(string& desc)
    {
        description = desc;
    }
    void addassignment(Assignment* a)
    {
        assignments.push_back(a);
    }
    int getid()
    {
        return id;
    }
    void deleteAllAssignments() {
        for (auto& assignment : assignments) {
            delete assignment;
        }
        assignments.clear();
    }
    void closeDueToTeacherRemoval() {
        if (getStatus() != "Closed")
        {
            setState(Closed::getInstance());
            for (const auto& a : assignments)
            {
                a->setstatus(1);
            }
            description += " - Complaint was closed due to the removal of the teacher.";
        }
    }
    ~Complaint() {
        for (auto* assignment : assignments) {
            delete assignment;
        }
    }

};



class Main {
    vector<Teacher*> teachers;
    vector<Department*> dept;
public:
    Main() {
        // Initialize vectors
        dept.push_back(new IT());
        dept.push_back(new Admin());
        dept.push_back(new Accounts());
    }

    void loadData() {
        loadFromFile_t("teachers.txt");
        loadFromFile_e("IT.txt", 0);
        loadFromFile_e("Admin.txt", 1);
        loadFromFile_e("Accounts.txt", 2);
        loadFromFile_m("managers.txt");
        loadFromFile_c("ITcomplaints.txt", 0);
        loadFromFile_c("Admincomplaints.txt", 1);
        loadFromFile_c("Accountscomplaints.txt", 2);
    }
    void savedata()
    {
        saveTeachersToFile("teachers.txt");
        saveEmployeesToFile("IT.txt", 0);
        saveEmployeesToFile("Admin.txt", 1); 
        saveEmployeesToFile("Accounts.txt", 2);
        saveManagersToFile("managers.txt");
        saveDeptComplaintsToFile("ITcomplaints.txt", 0);
        saveDeptComplaintsToFile("Admincomplaints.txt", 1);
        saveDeptComplaintsToFile("Accountscomplaints.txt", 2);
    }
    void saveTeachersToFile(const string& filename)
    {
        ofstream file(filename, ofstream::trunc);
        if (file.is_open()) {
            for (const auto& teacher : teachers) {
                file << teacher->getid() << "," << teacher->getName() << endl;
            }
            file.close();
        }
        else {
            cout << "Unable to open file for writing: " << filename << endl;
        }
    }
    void saveEmployeesToFile(const string& filename,int i)
    {
        ofstream file(filename, ofstream::trunc);
        if (file.is_open()) {
            const vector<Employee*>& employees = dept[i]->getEmployees();
            for (const auto& emp : employees) {
                file << emp->getid() << "," << emp->getName() << endl;
            }
            file.close();
        }
        else {
            cout << "Unable to open file for writing: " << filename << endl;
        }
    }
    void saveManagersToFile(const string& filename)
    {
        ofstream file(filename, ofstream::trunc);
        if (file.is_open()) {
            for (const auto& d : dept) {
                Manager* mgr = d->getManager();
                if (mgr != nullptr) {
                    file << mgr->getid() << "," << mgr->getName() << endl;
                }
                else
                {
                    file << endl;
                }
            }
            file.close();
        }
        else {
            cerr << "Unable to open file for writing: " << filename << endl;
        }
    }
    void saveDeptComplaintsToFile(const string& filename, int deptIndex) {
        ofstream file(filename, ofstream::trunc); // 'ofstream::trunc' to overwrite the file.
        if (file.is_open()) {
            const vector<Complaint*>& complaints = dept[deptIndex]->getComplaints();

            for (const auto& complaint : complaints) {
                file << complaint->getid() << ";";
                file << complaint->getdate() << ";";
                file << complaint->getStatus() << ";";
                file << complaint->getdesc() << ";";
                file << complaint->getFiledBy();

                if (complaint->getStatus() == "Assigned" || complaint->getStatus() == "Resolved" || complaint->getStatus() == "Closed") {
                    // Include the assignment details and status.
                    string assignments = complaint->getassignedemployees(); // Assuming this returns "employeeid:status," for each assignment.
                    file << ":" << assignments; // No need to remove the trailing comma if it's part of the required format.
                    file << ";" ;
                    file << complaint->getAssignmentDate(); // Assuming this returns the correct date.
                }

                // For "New" status, assignments and date of assignment are not included.
                file << endl; // New line for the next complaint.
            }
            file.close();
        }
        else {
            cerr << "Unable to open file for writing: " << filename << endl;
        }
    }

    void loadFromFile_t(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            addteacher(line);
        }

        file.close();
    }
    void addteacher(string& line)
    {
        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);
        int id1 = stoi(id);
        teachers.push_back(new Teacher(name, id1));
    }
    void removeTeacher(int id)
    {
        for (size_t i = 0; i < teachers.size(); ++i) {
            if (teachers[i]->getid() == id) {
                auto& complaints = teachers[i]->getcomplaints();
                for (auto* complaint : complaints) {
                    complaint->closeDueToTeacherRemoval();
                }
                for (size_t j = i; j < teachers.size() - 1; ++j) {

                    teachers[j] = teachers[j + 1];
                }
                teachers.pop_back();
                return;
            }
        }
    }
    bool addfeedback(int  cid, int id, int feedback)
    {
        for (const auto& teacher : teachers)
        {
            if (teacher->getid() == id)
            {
                for (const auto& complaint : teacher->getcomplaints())
                {
                    if (complaint->getid() == cid)
                    {
                        if (complaint->getStatus() == "Resolved")
                        {
                            string desc = complaint->getdesc();
                            desc += "-Feedback: ";
                            if (feedback == 1)
                            {
                                desc += "Satisfactory";
                                complaint->setState(Closed::getInstance());
                            }
                            else if (feedback == 2)
                            {
                                desc += "Unsatisfactory";
                                complaint->setState(New::getInstance());
                                desc += complaint->getassignedemployees();
                                complaint->deleteAllAssignments();
                            }
                            complaint->setdescription(desc);
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
    void loadFromFile_e(const string& filename, int i) {
        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            addemployee(line, i);
        }

        file.close();
    }
    void addemployee(string& line, int i)
    {

        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);
        int id1 = stoi(id);
        dept[i]->setdept(id1, name);
    }
    void removeEmployee(int id, int d)
    {
        dept[d]->removeEmployeeById(id);
    }
    string getcomplaintdesc(int id, int i)
    {
        for (const auto& teacher : teachers)
        {
            if (teacher->getid() == i)
            {
                for (const auto& complaint : teacher->getcomplaints())
                {
                    if (complaint->getid() == id)
                    {
                        return complaint->getdesc();
                    }
                }
            }
        }
        return "";
    }
    void setcomplaintdescription(string& desc, int id, int i)
    {
        for (const auto& teacher : teachers)
        {
            if (teacher->getid() == i)
            {
                for (const auto& complaint : teacher->getcomplaints())
                {
                    if (complaint->getid() == id)
                    {
                        complaint->setdescription(desc);
                        break;
                    }
                }
            }
        }
    }
    bool checkid(int id, int i)
    {
        if (dept[i]->check_personid(id))
        {
            return true;
        }
        return false;
    }
    void loadFromFile_m(const string& filename) {
        ifstream file(filename);
        string line;
        int i = 0;
        while (getline(file, line) && i < size(dept)) {
            if (line.empty())
            {
                i++;
            }
            else
            {
                addmanager(line, i);
                i++;
            }
            
        }

        file.close();
    }
    bool checkmanager(int i)
    {
        if (dept[i]->getManager())
        {
            return true;
        }
        return false;
    }
    void addmanager(string& line, int i)
    {
        stringstream ss(line);
        string id;
        string name;
        getline(ss, id, ',');
        getline(ss, name);

        dept[i]->setManager(stoi(id), name);
    }
    void removemanager(int i)
    {
        dept[i]->deletemanager();
    }
    void loadFromFile_c(string filename, int i)
    {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            string date, month, year, description, filedBy, status, id;
            string employeeList;
            string ayear, amonth, adate;
            stringstream ss(line);
            if (getline(ss, id, ';') &&
                getline(ss, year, '-') &&
                getline(ss, month, '-') &&
                getline(ss, date, ';') &&
                getline(ss, status, ';')) {
            }
            int d, m, y;
            d = stoi(date);
            m = stoi(month);
            y = stoi(year);
            if (status == "New")
            {
                if (getline(ss, description, ';') &&
                    getline(ss, filedBy)) {
                }

                addcomplaint(stoi(id), d, m, y, status, description, filedBy, i);
            }
            else
            {
                if (getline(ss, description, ';') &&
                    getline(ss, filedBy, ':') &&
                    getline(ss, employeeList, ';') &&
                    getline(ss, ayear, '-') &&
                    getline(ss, amonth, '-') &&
                    getline(ss, adate, ';')) {
                }
                addcomplaint(stoi(id), d, m, y, status, description, filedBy, i);
                addassignment(stoi(id), employeeList, stoi(adate), stoi(amonth), stoi(ayear), i, status);
            }

        }

        file.close();
    }
    void addcomplaint(int id, int d, int m, int y, string status, string description, string filedBy, int i)
    {

        State* stateInstance = nullptr;
        // Determine the correct State singleton instance based on stateStr
        if (status == "New") {
            stateInstance = New::getInstance();
        }
        else if (status == "Assigned") {
            stateInstance = Assigned::getInstance();
        }
        else if (status == "Resolved") {
            stateInstance = Resolved::getInstance();
        }
        else if (status == "Closed") {
            stateInstance = Closed::getInstance();
        }
        else {
            cout << "Invalid state: " << status << endl;
        }

        for (const auto& teacher : teachers) {
            if (teacher->getid() == stoi(filedBy))
            {
                if (stateInstance != nullptr)
                {
                    new Complaint(id, d, m, y, stateInstance, description, dept[i], teacher);
                    break;
                }
            }
        }
    }
    bool checkcomplaint(int id, int i)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
            if (complaint->getid() == id)
            {
                return true;
            }
        }
        return false;
    }
    void addassignment(int id, string employeeList, int d, int m, int y, int i, string state)
    {
        vector<int> status;
        vector<int> employeeIds = split(employeeList, ',', status, state);
        dept[i]->addassignment(id, employeeIds, d, m, y, status);
    }
    void displayTeachers() {
        for (const auto& teacher : teachers) {
            cout << teacher->getName() << endl;
        }
    }
    bool teacher(int id, string& name)
    {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id)
            {
                name = teacher->getName();
                return true;
            }
        }

        return false;
    }
    void printteachercomplaints() {
        for (const auto& teacher : teachers) {
            teacher->printComplaints();
        }
    }
    void teachercomplaints(int id)
    {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id)
            {
                teacher->printComplaints();
            }
        }
    }
    void displayEmployees(int i) {
        dept[i]->printEmployees();
    }
    bool checkemployee(int id, string& name, int i)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
            if (employee->getid() == id)
            {
                name = employee->getName();
                return true;
            }
        }
        return false;
    }
    bool checkmgr(int id, string& name, int i)
    {
        if (dept[i]->getManager())
        {
            if (dept[i]->getManager()->getid() == id)
            {
                name = dept[i]->getManager()->getName();
                return true;
            }
        }
        return false;
    }
    void displayManager(int i) {
        if (dept[i]->getManager())
        {
            dept[i]->printManager();
        }
        else
        {
            cout << "this department has no manager" << endl;
        }
    }
    void displaycomplaints(int i, string status) {
        dept[i]->printComplaints(status);
    }
    void displaydeptComplaints(int i) {
        dept[i]->printComplaints();
        cout << endl << "--------------------------------------------------------------------------------" << endl;

    }
    void teachercomplaintsbystatus(int id, string status)
    {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id)
            {
                for (const auto& complaint : teacher->getcomplaints())
                {
                    if (complaint->getStatus() == status)
                    {
                        complaint->printComplaint();
                    }
                }
            }
        }
    }
    vector<int> split(const string& str, char delimiter, vector<int>& status, string state) {
        vector<int> tokens;
        string token, status1;
        stringstream ss(str);
        if (state == "Assigned")
        {
            while (getline(ss, token, ':') && getline(ss, status1, delimiter)) {
                tokens.push_back(stoi(token));
                status.push_back(stoi(status1));
            }
        }
        else if (state == "Resolved" || state == "Closed")
        {
            while (getline(ss, token, delimiter)) {
                tokens.push_back(stoi(token));
                status.push_back(1);
            }
        }

        return tokens;
    }

    int generateid(int i)
    {
        int id;
        do {
            // Seed the random number generator with the current time
            std::srand(static_cast<unsigned int>(std::time(0)));

            // Generate and return a random 4-digit ID
            return 1000 + std::rand() % 9000; // Generates a random number between 1000 and 9999

        } while (dept[i]->checkid(id));
        return id;
    }
    int generate_personid(int i)
    {
        int id;
        do {
            srand(static_cast<unsigned int>(std::time(0)));

            return 1000 + std::rand() % 9000;

        } while (dept[i]->check_personid(id));
        return id;
    }
    bool check_managerid(int id, int d)
    {
        if (dept[d]->check_managerid(id))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int generate_managerid(int i)
    {
        int id;
        do {
            srand(static_cast<unsigned int>(std::time(0)));

            return 1000 + std::rand() % 9000;

        } while (dept[i]->check_managerid(id));
        return id;
    }
    int generate_teacher_id()
    {
        int id;
        do {
            srand(static_cast<unsigned int>(std::time(0)));

            return 1000 + std::rand() % 9000;

        } while (check_teacherid(id));
        return id;
    }
    bool check_teacherid(int id) {
        for (const auto& teacher : teachers) {
            if (teacher->getid() == id) {
                return true;
            }
        }
        return false;
    }
    void viewemployeecomplaints(int id, int i)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
            if (employee->getid() == id)
            {
                employee->viewcomplaints();
            }
        }
    }
    void printemployeestatus(int id, int i, string status)
    {
        for (const auto& employee : dept[i]->getEmployees()) {
            if (employee->getid() == id)
            {
                employee->printComplaints(status);
            }
        }
    }
    bool setState(int id, State* state, int i)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
            if (complaint->getid() == id)
            {
                complaint->setState(state);
                return true;
            }
        }
        return false;
    }
    bool checkassignmentstatus(int id, int i)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
            if (complaint->getid() == id)
            {
                return complaint->checkassignmentstatus();
            }
        }
        return false;
    }
    bool setassignmentstatus(int cid, int eid, int i, int status)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
            if (complaint->getid() == cid)
            {
                return complaint->setassignmentstatus(eid, status);
            }
        }
        return false;
    }
    string getassignedemployees(int id, int i)
    {
        for (const auto& complaint : dept[i]->getComplaints())
        {
            if (complaint->getid() == id)
            {
                return complaint->getassignedemployees();
            }
        }
        return "";
    }
    void viewComplaintDetails(int id)
    {
        bool flag = false;
        for (int i = 0; i < 3; i++)
        {
            if (dept[i]->printComplaintbyId(id)) {
                flag = true;
                return;
            }


        }
        if (flag == false)
        {
            cout << "invalid id" << endl;
            return;
        }
    }
    void generateReportSummary(int sy, int sm, int sd, int ey, int em, int ed)
    {
        int s = 0;
        for (int i = 0; i < 3; i++)
        {
            s += dept[i]->checkcomplaint(sy, sm, sd, ey, em, ed);
        }
        cout << endl;
        cout << "the total complaints received within this time period were : " << s << endl;
    }
    int noofemployees(int i)
    {
        return dept[i]->getEmployees().size();
    }
    ~Main()
    {
        for (auto& teacher : teachers)
        {
            delete teacher;
        }

        for (auto& d : dept)
        {
            delete d;
        }
    }
};

int main()
{
    Main app;

    app.loadData();
    cout << "Teachers" << endl;
    app.displayTeachers();
    app.displayManager(0);
    app.displayEmployees(0);
    app.displayManager(1);
    app.displayEmployees(1);
    app.displayManager(2);
    app.displayEmployees(2);
    app.displaydeptComplaints(0);
    app.displaydeptComplaints(1);
    app.displaydeptComplaints(2);
    app.printteachercomplaints();
    int choice;
    do {
        displayMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            teacherMenu(app);
        }
        else if (choice == 2) {
            managerMenu(app);
        }
        else if (choice == 3) {
            employeeMenu(app);
        }
        else if (choice == 4) {
            adminMenu(app);
        }
        else if (choice == 5) {
            generatereport(app);
        }
        else if (choice == 6)
        {
            cout << "Exiting the system. Goodbye!\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);
    app.savedata();
    return 0;
}
int Department::checkcomplaint(int sy, int sm, int sd, int ey, int em, int ed)
{
    Date startDate(sd, sm, sy);
    Date endDate(ed, em, ey);
    int count = 0;
    for (const auto& complaint : complaints) {
        Date complaintdate = complaint->getDate();
        if (complaintdate >= startDate && complaintdate <= endDate) {
            count++;
            complaint->printComplaint();
        }
    }
    return count;
}
bool Department::printComplaintbyId(int id) {
    for (const auto& complaint : complaints) {
        if (complaint->getid() == id) {
            complaint->printComplaint();
            return true;
        }
    }
    return false;
}
void IT::printComplaints() const
{
    cout << "IT Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}

void Admin::printComplaints() const
{
    cout << "Admin Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}
void Accounts::printComplaints() const
{
    cout << "Accounts Department Complaints:" << endl;
    for (const auto& complaint : getComplaints())
    {
        complaint->printComplaint();
    }
}

void Teacher::printComplaints() const
{
    cout << "\n\n\nTeacher: " << getName() << endl;
    for (const auto& complaint : complaints)
    {
        complaint->printComplaint();
    }
}

Assignment::Assignment(int d, int m, int y, Complaint* c, Employee* e, int status) :date(d, m, y)
{
    this->status = status;
    complaint = c;
    employee = e;
    complaint->addassignment(this);
    employee->addassignment(this);
}

string Assignment::employeename()
{
    return employee->getName();
}

bool compareChronologicalOrder(const Complaint* a, const Complaint* b) {
    return a->getDate() < b->getDate();
}

void Department::printComplaints(const string& status) const {
    cout << getdeptname() << " Department Complaints:" << endl;
    for (const auto& complaint : getComplaints()) {
        if (complaint->getStatus() == status) {
            complaint->printComplaint();
        }
    }
}

void teacherMenu(Main& main) {
    int choice;
    string id, name;
    cout << "Enter ID: ";
    cin.ignore();
    getline(cin, id);
    if (main.teacher(stoi(id), name))
    {
        cout << "\n\nWelcome " << name << endl;
        do {
            // Display teacher menu options
            cout << "\nTeacher Menu\n";
            cout << "1. File a Complaint\n";
            cout << "2. View Complaints\n";
            cout << "3. Record Feedback\n";
            cout << "4. Go Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1) {
                int dept;
                cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
                cin >> dept;
                if (dept >= 1 && dept <= 3)
                {
                    int year, month, date;
                    string description;
                    cout << "Enter date in the format yyyy-mm-dd: ";
                    cin >> year >> month >> date;
                    cout << "Enter Description: ";
                    cin.ignore();
                    getline(cin, description);
                    int complaint_id = main.generateid(dept - 1);
                    cout << "Your complaint id is: " << complaint_id << endl;
                    main.addcomplaint(complaint_id, date, month, year, "New", description, id, dept - 1);
                }
                else
                {
                    cout << "invalid choice" << endl;
                }

            }
            else if (choice == 2) {
                main.teachercomplaints(stoi(id));
            }
            else if (choice == 3) {
                // Record feedback
                int complaint_id;
                main.teachercomplaintsbystatus(stoi(id), "Resolved");
                cout << "Enter complaint ID: ";
                cin >> complaint_id;
                cout << "Are you satisfied with the resolution of your complaint?\n1.Yes\n2.No\n";
                int c;
                cin >> c;
                if (main.addfeedback(complaint_id, stoi(id), c))
                {
                    cout << "feedback recorded successfully" << endl;
                }
                else
                {
                    cout << "invalid choice" << endl;
                }

            }
            else if (choice == 4) {
                cout << "Returning to the main menu.\n";
            }
            else {
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 4);
    }
    else
    {
        cout << "Invalid id, Please enter again\n";
    }


}

void adminMenu(Main& main)
{
    int choice;
    bool exitMenu = false;

    while (!exitMenu) {
        cout << "Admin Menu\n";
        cout << "1. Add Employee\n";
        cout << "2. Remove Employee\n";
        cout << "3. Add Manager\n";
        cout << "4. Remove Manager\n";
        cout << "5. Add Teacher\n";
        cout << "6. Remove Teacher\n";
        cout << "7. Go Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int dept;
            string name;
            int person_id;
            cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
            cin >> dept;
            if (dept >= 1 && dept <= 3)
            {
                cout << "Enter your name : ";
                cin.ignore();
                getline(cin, name);
                person_id = main.generate_personid(dept - 1);
                cout << "Your employee ID is : " << person_id << endl;
                string l = to_string(person_id) + "," + name;
                main.addemployee(l, dept - 1);
                main.displayEmployees(0);
            }
            else
            {
                cout << "invalid choice" << endl;
            }

        }
        else if (choice == 2) {
            int dept, id;
            cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
            cin >> dept;
            if (dept >= 1 && dept <= 3)
            {
                cout << "Enter your ID :";
                cin >> id;
                bool check = main.checkid(id, dept - 1);
                if (check)
                {
                    main.removeEmployee(id, dept - 1);
                    main.displayEmployees(dept - 1);
                }
                else
                {
                    cout << "Employee with id : " << id << "does not exist" << endl;
                }
            }
            else
            {
                cout << "invalid choice" << endl;
            }



        }
        else if (choice == 3) {
            int dept;
            string name;
            int manager_id;
            int c;
            cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
            cin >> dept;
            if (dept >= 1 && dept <= 3)
            {
                bool mgr = main.checkmanager(dept - 1);
                if (mgr)
                {
                    cout << "this department already has a manager" << endl;
                    cout << "do you still want to add a new manager? \n1.Yes\n2.No" << endl;
                    cin >> c;
                    if (c == 1)
                    {
                        main.removemanager(dept - 1);
                        cout << "Enter your name : ";
                        cin.ignore();
                        getline(cin, name);
                        manager_id = main.generate_managerid(dept - 1);
                        cout << "Your manager ID is : " << manager_id << endl;
                        string l = to_string(manager_id) + "," + name;
                        main.addmanager(l, dept - 1);
                        cout << "manager added successfully" << endl;
                        main.displayManager(0);
                        main.displayEmployees(0);
                        main.displayManager(1);
                        main.displayEmployees(1);
                        main.displayManager(2);
                        main.displayEmployees(2);
                    }
                    else if (c == 2)
                    {
                        cout << "Returning to the admin menu.\n";
                    }
                    else
                    {
                        cout << "invalid choice" << endl;
                    }
                }
                else
                {
                    cout << "Enter your name : ";
                    cin.ignore();
                    getline(cin, name);
                    manager_id = main.generate_managerid(dept - 1);
                    cout << "Your manager ID is : " << manager_id << endl;
                    string l = to_string(manager_id) + "," + name;
                    main.addmanager(l, dept - 1);
                    cout << "manager added successfully" << endl;
                    main.displayManager(0);
                    main.displayEmployees(0);
                    main.displayManager(1);
                    main.displayEmployees(1);
                    main.displayManager(2);
                    main.displayEmployees(2);
                }
            }
            else
            {
                cout << "invalid choice" << endl;
            }

        }
        else if (choice == 4) {
            int dept, id;
            cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
            cin >> dept;
            if (dept >= 1 && dept <= 3)
            {
                main.removemanager(dept - 1);
                cout << "manager has been removed successfully" << endl;
                main.displayManager(0);
            }
            else
            {
                cout << "invalid choice" << endl;
            }

        }
        else if (choice == 5) {
            string name;
            int teacher_id;
            cout << "Enter your name : ";
            cin.ignore();
            getline(cin, name);
            teacher_id = main.generate_teacher_id();
            cout << "Your teacher ID is : " << teacher_id << endl;
            string l = to_string(teacher_id) + "," + name;
            main.addteacher(l);
            cout << "teacher has been added successfully" << endl;
            main.displayTeachers();
        }
        else if (choice == 6) {
            int id;
            cout << "Enter the ID of the teacher to remove: ";
            cin >> id;
            if (main.check_teacherid(id)) {
                main.removeTeacher(id);
                cout << "Teacher with ID " << id << " has been removed." << endl;
                main.displayTeachers();
            }
            else {
                cout << "No teacher with ID " << id << " found." << endl;
            }
        }
        else if (choice == 7) {
            exitMenu = true;
            cout << "Returning to the main menu.\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void generatereport(Main& main)
{
    bool exitMenu = false;
    int choice;
    while (!exitMenu)
    {
        cout << "\nReports Menu\n";
        cout << "1. View Summary of Complaints\n";
        cout << "2. View Details of a Specific Complaint\n";
        cout << "3. Go Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int syear, smonth, sdate;
            int eyear, emonth, edate;
            cout << "Enter start date in the format yyyy-mm-dd: ";
            cin >> syear >> smonth >> sdate;
            cout << "Enter end date in the format yyyy-mm-dd: ";
            cin >> eyear >> emonth >> edate;
            main.generateReportSummary(syear, smonth, sdate, eyear, emonth, edate);
        }
        else if (choice == 2) {
            string cid;
            cout << "Enter the ID of the complaint to view details: ";
            cin.ignore();
            getline(cin, cid);
            main.viewComplaintDetails(stoi(cid));
        }
        else if (choice == 3) {
            cout << "Returning to the main menu.\n";
            exitMenu = true;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

}
bool Complaint::isEmployeeAssigned(int employeeId) const {
    for (const auto& assignment : assignments) {
        if (assignment->employeeid() == employeeId) {
            return true;
        }
    }
    return false;
}

void Department::addassignment(int id, vector<int> e, int d, int m, int y, vector<int> status) {
    if (!complaints.empty()) {
        for (const auto& complaint : getComplaints()) {
            if (complaint->getid() == id) {
                Date date(d, m, y);
                Date date1 = complaint->getDate();
                if (date1 <= date) {
                    for (size_t i = 0; i < e.size(); ++i) {
                        int employeeid = e[i];
                        int st = status[i];

                        for (const auto& emp : getEmployees()) {
                            if (emp->getid() == employeeid) {
                                if (complaint->getStatus() == "New") {
                                    complaint->setState(Assigned::getInstance());
                                }
                                new Assignment(d, m, y, complaint, emp, st);
                                break;
                            }
                        }
                    }
                }
                else {
                    cout << "invalid date" << endl;
                }
                break;
            }
        }
    }
}



bool Department::checkid(int id)
{
    for (const auto& complaint : getComplaints())
    {
        if (complaint->getid() == id)
        {
            return true;
        }
    }
    return false;
}
bool Department::check_personid(int id)
{
    for (const auto& employee : getEmployees())
    {
        if (employee->getid() == id)
        {
            return true;
        }
    }
    return false;
}
bool Department::check_managerid(int id)
{
    Manager* manager = getManager();
    if (manager != nullptr && manager->getid() == id)
    {
        return true;
    }
    return false;
}
void Assignment::viewcomplaints()
{
    complaint->printComplaint();
}
void Employee::printComplaints(const string& status) const {
    //cout << "Employee: " << getName() << endl;
    for (const auto& assignment : assignments) {
        if (assignment->getstatus() == status) {
            assignment->viewcomplaints();
        }
    }
}
string Assignment::getstatus()
{
    return complaint->getStatus();
}

void Department::setState(int id, State* state)
{
    for (const auto& complaint : getComplaints())
    {
        if (complaint->getid() == id)
        {
            complaint->setState(state);
            break;
        }
    }
}
string Assignment::employeename()const
{
    return employee->getName();
}
int Assignment::employeeid()const
{
    return employee->getid();
}
void employeeMenu(Main& main) {
    int choice;
    int dept;
    cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
    cin >> dept;
    if (dept >= 1 && dept <= 3)
    {
        int cid, in;
        string id, name;
        cout << "Enter ID: ";
        cin.ignore();
        getline(cin, id);
        if (main.checkemployee(stoi(id), name, dept - 1))
        {
            cout << "\n\nWelcome " << name << endl;
            do {
                // Display employee menu options
                std::cout << "\nEmployee Menu\n";
                std::cout << "1. View Assigned Jobs\n";
                std::cout << "2. Update Job Completion\n";
                std::cout << "3. Go Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                if (choice == 1) {
                    cout << "Assigned Jobs:\n";
                    main.printemployeestatus(stoi(id), dept - 1, "Assigned");
                    cout << "\nResolved Jobs:\n";
                    main.printemployeestatus(stoi(id), dept - 1, "Resolved");
                }
                else if (choice == 2) {
                    // Update job completion
                    main.printemployeestatus(stoi(id), dept - 1, "Assigned");
                    cout << "\nEnter complaint id: ";
                    cin >> cid;
                    cout << "Do you want to mark this job as done?\n1.Yes\n2.No\n";
                    cin >> in;
                    cout << "Are you sure?\n1.Yes\n2.No\n";
                    cin >> in;
                    if (in == 1)
                    {
                        // if (main.setState(cid, Resolved::getInstance(), dept - 1))
                        if (main.setassignmentstatus(cid, stoi(id), dept - 1, 1))
                            cout << "Job marked as done\n";
                        else
                            cout << "An error occured\n";
                    }
                }
                else if (choice == 3) {
                    cout << "Returning to the main menu.\n";
                }
                else {
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 3);

        }
        else
        {
            cout << "Invalid id, Please enter again\n";
        }
    }
    else
    {
        cout << "invalid choice" << endl;

    }
}
void managerMenu(Main& main) {
    int choice;
    int dept;
    cout << "Enter Department:\n1.IT\n2.Admin\n3.Accounts\n";
    cin >> dept;
    if (dept >= 1 && dept <= 3)
    {
        int cid, in;
        string id, name;
        cout << "Enter ID: ";
        cin.ignore();
        getline(cin, id);
        if (main.checkmgr(stoi(id), name, dept - 1))
        {
            cout << "\n\nWelcome " << name << endl;
            do {
                // Display manager menu options
                std::cout << "\nManager Menu\n";
                std::cout << "1. Assign Job to Employee\n";
                std::cout << "2. Update Job Completion\n";
                std::cout << "3. View Complaints\n";
                std::cout << "4. Go Back to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                if (choice == 1) {
                    // Assign job to employee
                    int eid, n; string eids;
                    cout << "UnAssigned Complaints:\n";
                    main.displaycomplaints(dept - 1, "New");
                    cout << "\nEnter complaint id: ";
                    cin >> cid;
                    if (main.checkassignmentstatus(cid, dept - 1))
                    {
                        cout << "ERROR:Job already assigned" << endl;
                        continue;
                    }
                    else if (main.checkcomplaint(cid, dept - 1))
                    {
                        cout << "Employees:\n";
                        main.displayEmployees(dept - 1);
                        cout << "Enter the number of employees you want to assign this complaint to:";
                        cin >> n;
                        if (n > 0 && n < main.noofemployees(dept - 1))
                        {
                            cout << "Enter employee ids: ";
                            for (int i = 0; i < n; i++)
                            {
                                cin >> eid;
                                if (main.checkemployee(eid, name, dept - 1))
                                {
                                    eids += to_string(eid) + ":0,";
                                }
                                else
                                {
                                    cout << "invalid employee id" << endl;
                                    i--;
                                }
                            }
                            if (eids.length() > 0)
                            {
                                int d, m, y;
                                cout << "Enter date of assignment in the format yyyy-mm-dd: ";
                                cin >> y >> m >> d;

                                main.addassignment(cid, eids, d, m, y, dept - 1, "Assigned");
                                cout << "Job assigned to respective employees successfully\n";
                            }
                        }
                        else
                        {
                            cout << "invalid number of employees" << endl;
                        }
                    }
                    else
                    {
                        cout << "invalid complaint id" << endl;
                    }

                }
                else if (choice == 2) {
                    // Update job completion
                    main.displaycomplaints(dept - 1, "Assigned");
                    cout << "\nEnter complaint id: ";
                    cin >> cid;
                    cout << "Do you want to mark this complaint to be resolved?\n1.Yes\n2.No\n";
                    cin >> in;
                    cout << "Are you sure?\n1.Yes\n2.No\n";
                    cin >> in;
                    if (in == 1)
                    {
                        if (main.checkassignmentstatus(cid, dept - 1))
                        {
                            if (main.setState(cid, Resolved::getInstance(), dept - 1))
                                cout << "Complaint marked as resolved\n";
                            else
                                cout << "An error occured\n";
                        }
                        else
                        {
                            cout << "ERROR:Job In Progress" << endl;
                        }
                    }

                }
                else if (choice == 3) {
                    cout << "Complaints:\n";
                    main.displaycomplaints(dept - 1, "New");
                    main.displaycomplaints(dept - 1, "Assigned");
                    main.displaycomplaints(dept - 1, "Resolved");
                    main.displaycomplaints(dept - 1, "Closed");
                }
                else if (choice == 4) {
                    std::cout << "Returning to the main menu.\n";
                }
                else {
                    std::cout << "Invalid choice. Please try again.\n";
                }

            } while (choice != 4);
        }
        else
        {
            cout << "Invalid id, Please enter again\n";
        }

    }
    else
    {
        cout << "invalid choice" << endl;
    }
}