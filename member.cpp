#include"library.h"
#include<iomanip>

void loadMembers() {
    members.clear();
    ifstream myFile("Members.txt");
    if(!myFile.is_open()) return;

    MemberRecord m;
    while(myFile >> m.ID >> m.userName >> m.passWord) {
        members.push_back(m);
    }
    myFile.close();
}

bool Member::login(string userName, string passWord) {
    for(auto &m : members) {
        if(m.userName == userName && m.passWord == passWord) {
            this->userName = userName;
            this->passWord = passWord;
            return true;
        }
    }
    return false;
}

void Member::menu() {
    int choice;
    while(true) {
        cout<<"\n------------------------------------------------------------"<<endl;
        cout<<"                       MEMBER MENU"<<endl;
        cout<<"------------------------------------------------------------"<<endl;
        cout<<"1. Issue Book"<<endl;
        cout<<"2. Return Book"<<endl;
        cout<<"3. View My Issued Books"<<endl;
        cout<<"4. View My Fines"<<endl;
        cout<<"5. Request a Book"<<endl;
        cout<<"6. Renew a Book"<<endl;
        cout<<"7. Logout"<<endl;
        cout<<"\nEnter Your Choice: ";
        cin>>choice;

        if (cin.fail()) {
            cin.clear(); // 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "\n------------------------------------------------------------" << endl;
            cout << "          Invalid Input! Please enter a number." << endl;
            cout << "------------------------------------------------------------" << endl;
            continue;
        }
        switch(choice) {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: viewMyIssuedBooks(); break;
            case 4: viewMyFines(); break;
            case 5: requestBook(); break;
            case 6: renewBook(); break;
            case 7:
                cout<<"\n------------------------------------------------------------"<<endl;
                cout<<"                 Logging Out from Member Account"<<endl;
                cout<<"------------------------------------------------------------"<<endl;
                return;
            default:
                cout<<"\n------------------------------------------------------------"<<endl;
                cout<<"                 Invalid Choice! Try Again"<<endl;
                cout<<"------------------------------------------------------------"<<endl;
                continue;
        }
    }
}

void Member::issueBook() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                         ISSUE BOOK"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    if(books.empty()) {
        cout<<"No book available in the library"<<endl;
        return;
    }

      cout<<left<<setw(25)<<"Book Name"<<setw(15)<<"ISBN"<<setw(10)<<"Available" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (auto &b : books) {
        cout <<left<<setw(25)<<b.name
            << setw(15)<<b.ISBN
            <<setw(10)<<(b.totalQuantity - b.issuedQuantity)
            <<endl;
    }

    cout << "------------------------------------------------------------" << endl;

    int isbn;
    cout<<"\nEnter ISBN number of the book: ";
    cin>>isbn;
    bool found = false;

    for(auto &b : books) {
        if(b.ISBN == isbn) {
            found = true;
            if(b.issuedQuantity < b.totalQuantity) {
                b.issuedQuantity++;
                cout<<"\nBook Issued Successfully"<<endl;

                ofstream file("IssuedBooks.txt", ios::app);
                file<<this->userName<<" "<<b.ISBN<<" "<<b.name<<" "<<getCurrentDate()<<endl;
                file.close();

                ofstream bfile("Books.txt", ios::trunc);
                for(auto &bk : books) {
                    bfile<<bk.name<<" "<<bk.ISBN<<" "<<bk.totalQuantity<<" "<<bk.issuedQuantity<<"\n";
                }
                bfile.close();

                ofstream tfile("transactions.txt", ios::app);
                tfile<<"Issued | Book: "<<b.name<<" | ISBN: "<<b.ISBN<<" | To: "<<this->userName<<" | Date: "<<getCurrentDate()<<"\n";
                tfile.close();
            }
        }
    }

    if(!found) {
        cout<<"Book with the given ISBN not found"<<endl;
        cout<<"Do you want to request it (Y/N): ";
        char ans;
        cin>>ans;
        if(ans=='Y' || ans=='y') {
            requestBook();
        }
    }
}

void Member::viewMyFines() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                         MY FINES"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream file("IssuedBooks.txt");
    if(!file.is_open()) {
        cout<<"No issued books record found."<<endl;
        return;
    }

    string uname, bookName, date;
    int isbn;
    bool any = false;
    string currentDate = getCurrentDate();

    while(file >> uname >> isbn >> bookName >> date) {
        if(uname == this->userName) {
            any = true;
            int days = calculateDays(date, currentDate);
            int fine = (days > 7) ? (days - 7) * 10 : 0;
            cout<<"Book: "<<bookName<<" | ISBN: "<<isbn<<" | Issued on: "<<date
                <<" | Days Kept: "<<days<<" | Fine: "<<fine<<" Rs"<<endl;
        }
    }
    file.close();

    if(!any) cout<<"No books issued by you."<<endl;
}

void Member::returnBook() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                         RETURN BOOK"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream file("IssuedBooks.txt");
    if(!file.is_open()) {
        cout<<"No issued book record found"<<endl;
        return;
    }

    vector<string> lines;
    string uname, bookName, date;
    int isbn;
    bool found = false;

    cout<<"Your Issued Books:"<<endl;
    while(file >> uname >> isbn >> bookName >> date) {
        if(uname == this->userName) {
            cout<<"Book: "<<bookName<<" | ISBN: "<<isbn<<" | Issued on: "<<date<<endl;
        }
        lines.push_back(uname + " " + to_string(isbn) + " " + bookName + " " + date);
    }
    file.close();

    cout<<"\nEnter ISBN of the book you want to return: ";
    int returnIsbn;
    cin>>returnIsbn;
    string currentDate = getCurrentDate();
    int days = calculateDays(date, currentDate);
    int fine = (days > 7) ? (days - 7) * 10 : 0;

    ofstream out("IssuedBooks.txt.tmp");
    for(auto &line : lines) {
        istringstream iss(line);
        iss >> uname >> isbn >> bookName >> date;

        if(uname == this->userName && isbn == returnIsbn && !found) {
            found = true;
            cout<<"\nReturning "<<bookName<<" ..."<<endl;

            for(auto &b : books) {
                if(b.ISBN == isbn) {
                    b.issuedQuantity--;
                    break;
                }
            }
            cout<<"Your Fine is: Rs. "<<fine<<endl;
            ofstream bfile("Books.txt", ios::trunc);
            for(auto &b : books) {
                bfile<<b.name<<" "<<b.ISBN<<" "<<b.totalQuantity<<" "<<b.issuedQuantity<<"\n";
            }
            bfile.close();

            ofstream tfile("transactions.txt", ios::app);
            tfile<<"Returned | Book: "<<bookName<<" | ISBN: "<<isbn
                 <<" | By: "<<this->userName<<" | Date: "<<currentDate<<"\n";
            tfile.close();
        } else {
            out<<line<<"\n";
        }
    }
    out.close();
    remove("IssuedBooks.txt");
    rename("IssuedBooks.txt.tmp", "IssuedBooks.txt");

    if(!found) cout<<"Book not found under your account."<<endl;
}


void Member::viewMyIssuedBooks() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                     MY ISSUED BOOKS"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream file("IssuedBooks.txt");
    if(!file.is_open()) {
        cout<<"No issued books found."<<endl;
        return;
    }

    string uname, bookName, date;
    int isbn;
    bool any = false;

    while(file >> uname >> isbn >> bookName >> date) {
        if(uname == this->userName) {
            any = true;
            cout<<"Book: "<<bookName<<" | ISBN: "<<isbn<<" | Issued on: "<<date<<endl;
        }
    }
    file.close();

    if(!any) cout<<"No books issued under your name."<<endl;
}

void Member::requestBook() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                         REQUEST BOOK"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    string bookName;
    cout<<"Enter Book Name to Request: ";
    cin>>bookName;

    ofstream rfile("request.txt", ios::app);
    if(rfile.is_open()) {
        rfile<<this->userName<<" "<<bookName<<"\n";
        cout<<"Book Requested Successfully"<<endl;
        rfile.close();
    } else {
        cout<<"Unable to Save Request"<<endl;
    }
}

void Member::renewBook() {
    
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                         RENEW BOOK"<<endl;
    cout<<"------------------------------------------------------------"<<endl;
    viewMyIssuedBooks();
    ifstream file("IssuedBooks.txt");
    if(!file.is_open()) {
        cout<<"No issued books found."<<endl;
        return;
    }

    vector<string> lines;
    string uname, bookName, date;
    int isbn;
    bool found = false;

    cout<<"Enter ISBN to Renew: ";
    int renewIsbn;
    cin>>renewIsbn;
    string currentDate = getCurrentDate();

    while(file >> uname >> isbn >> bookName >> date) {
        if(uname == this->userName && isbn == renewIsbn && !found) {
            found = true;
            cout<<"Book Renewed Successfully"<<endl;
            lines.push_back(uname + " " + to_string(isbn) + " " + bookName + " " + currentDate);
        } else {
            lines.push_back(uname + " " + to_string(isbn) + " " + bookName + " " + date);
        }
    }
    file.close();

    ofstream out("IssuedBooks.txt", ios::trunc);
    for(auto &l : lines) out<<l<<"\n";
    out.close();

    if(!found) cout<<"Book not found under your account."<<endl;
}
