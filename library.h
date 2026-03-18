#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <vector>
#include <string>
#include<sstream>
#include<fstream>
#include<limits>
using namespace std;

//structure for book
struct Book{
    string name;
    long long ISBN;
    int totalQuantity;
    int issuedQuantity;
};
extern vector<Book>books;

//Structure for member
struct MemberRecord {
    int ID;
    string userName, passWord;
};

extern vector<MemberRecord> members;

class User {
public:
    string userName, passWord;
    virtual bool login(string userName, string passWord) = 0;
    virtual void menu() = 0;
    virtual ~User() {}
};

class Librarian : public User {
public:
    bool login(string userName, string passWord) override;
    
    void menu() override;
    void addBook();
    void viewAllBooks();
    void removeBook();
    void registerMember();
    void viewAllMembers();
    void removeMember();
    void viewRequests();
    void clearRequests();
    void viewTransactions();
    void reports();
};

class Member : public User {
public:
    bool login(string userName, string passWord) override;
    void menu() override;
    void issueBook();
    void viewMyFines();
    void returnBook();
    void viewMyIssuedBooks();
    void requestBook();
    void renewBook();
};

string getCurrentDate();
int calculateDays(const string& issueDate, const string& currentDate);

void Library();
void loadBooks();
void loadMembers();
#endif
