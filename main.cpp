#include<iostream>
#include<cctype>
#include"library.h"
using namespace std;

void Library() {
    string userName, passWord;
    string ch;

    while (true) {
        cout << "------------------------------------------------------------" << endl;
        cout << "                WELCOME TO MY LIBRARY SYSTEM" << endl;
        cout << "------------------------------------------------------------" << endl;

        cout << "1. Login as Librarian" << endl;
        cout << "2. Login as Member" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter Your Choice: ";
        getline(cin, ch);

        
        bool validNumber=true;
        for (char c : ch){
            if (!isdigit(c)){  
                validNumber = false;
                break;
            }
        }
    
        if (!validNumber || ch.empty()) {
            cout<<endl;
            cout << "------------------------------------------------------------" << endl;
            cout << "         Invalid input! Please enter a number (1-3)" << endl;
            cout << "------------------------------------------------------------" << endl;
            continue;
        }

        int choice = stoi(ch);
        User* user = nullptr;

        switch (choice) {
            case 1:
                user = new Librarian();
                break;
            case 2:
                user = new Member();
                break;
            case 3:
                cout << "\n------------------------------------------------------------" << endl;
                cout << "           Thank You for Using Library Management System" << endl;
                cout << "------------------------------------------------------------" << endl;
                return;
            default:
                cout << "\n------------------------------------------------------------" << endl;
                cout << "               Invalid Choice! Please Try Again" << endl;
                cout << "------------------------------------------------------------" << endl;
                continue;
        }

        cout << "\nEnter UserName: ";
        cin >> userName;
        cout << "Enter PassWord: ";
        cin >> passWord;
        cin.ignore(); 

        if (user->login(userName, passWord)) {
            cout << "\n------------------------------------------------------------" << endl;
            cout << "                    Login Successful!" << endl;
            cout << "------------------------------------------------------------" << endl;
            user->menu();
        } else {
            cout << "\n------------------------------------------------------------" << endl;
            cout << "               Wrong UserName or PassWord!" << endl;
            cout << "------------------------------------------------------------" << endl;
        }

        delete user;
    }
}


// Main Function
int main(){
    loadBooks();
    loadMembers();
    system("cls");

    cout<<"------------------------------------------------------------"<<endl;
    cout<<"             WELCOME TO LIBRARY MANAGEMENT SYSTEM"<<endl;
    cout<<"------------------------------------------------------------"<<endl;
    cout<<endl;

    Library();
}
