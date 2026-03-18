#include"library.h"
#include<iomanip>
vector<MemberRecord> members;
vector<Book> books;

bool Librarian::login(string userName, string passWord) {
    return ((userName=="Admin" && passWord=="12345") || (userName=="Krishna" && passWord=="Krishna@1902"));
}

void Librarian::menu() {
    int choice;
    while (true) {
        cout<<"\n------------------------------------------------------------"<<endl;
        cout<<"                     LIBRARIAN MENU"<<endl;
        cout<<"------------------------------------------------------------"<<endl;

        cout<<"1. Add Book"<<endl;
        cout<<"2. Remove Book"<<endl;
        cout<<"3. View All Books"<<endl;
        cout<<"4. Register Member"<<endl;
        cout<<"5. Remove Member"<<endl;
        cout<<"6. View All Members"<<endl;
        cout<<"7. View Requests"<<endl;
        cout<<"8. View Transactions"<<endl;
        cout<<"9. Reports (Summary)"<<endl;
        cout<<"10. Clear All Requests"<<endl;
        cout<<"11. Logout"<<endl;
        cout<<"\nEnter Your Choice: ";
        cin>>choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<"\n------------------------------------------------------------" << endl;
            cout<<"          Invalid Input! Please enter a number." << endl;
            cout<<"------------------------------------------------------------" << endl;
            continue;
        }
        switch (choice) {
            case 1: addBook(); break;
            case 2: removeBook(); break;
            case 3: viewAllBooks(); break;
            case 4: registerMember(); break;
            case 5: removeMember(); break;
            case 6: viewAllMembers(); break;
            case 7: viewRequests(); break;
            case 8: viewTransactions(); break;
            case 9: reports(); break;
            case 10: clearRequests(); break;
            case 11:
                cout<<"\n------------------------------------------------------------"<<endl;
                cout<<"                  Logging Out from Librarian"<<endl;
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

void loadBooks() {
    books.clear();
    ifstream myFile("Books.txt");
    if(!myFile.is_open()) return;

    string name;
    long long ISBN;
    int total, issued;

    while(myFile>>name>>ISBN>>total>>issued) {
        Book b;
        b.name = name;
        b.ISBN = ISBN;
        b.totalQuantity = total;
        b.issuedQuantity = issued;
        books.push_back(b);
    }
    myFile.close();
}

void Librarian::addBook() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                        ADD BOOK"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    Book b;
    cout<<"Enter Book name (no spaces): ";
    cin>>b.name;
    cout<<"Enter ISBN: ";
    cin>>b.ISBN;
    cout<<"Enter Quantity: ";
    cin>>b.totalQuantity;

    b.issuedQuantity = 0;
    books.push_back(b);

    ofstream myFile("Books.txt", ios::app);
    if(myFile.is_open()) {
        myFile<<b.name<<" "<<b.ISBN<<" "<<b.totalQuantity<<" "<<b.issuedQuantity<<"\n";
        myFile.close();
        cout<<"Book Added Successfully"<<endl;
    } else {
        cout<<"Error! Unable to Add Book"<<endl;
    }
}

void Librarian::viewAllBooks() {
    loadBooks();

    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                       ALL BOOKS"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    if(books.empty()) {
        cout<<"No books available in the library."<<endl;
        return;
    }

    cout<<left<<setw(25)<<"Name"
        <<setw(18)<<"| ISBN"
        <<setw(13)<<"| Total"
        <<setw(13)<<"| Issued"
        <<"| Remaining"<<endl;

    cout<<"------------------------------------------------------------"<<endl;

    for(auto &b : books) {
        int remaining = b.totalQuantity - b.issuedQuantity;

        cout<<left<<setw(25)<<b.name
            <<setw(18)<<("| " + to_string(b.ISBN))
            <<setw(13)<<("| " + to_string(b.totalQuantity))
            <<setw(13)<<("| " + to_string(b.issuedQuantity))
            <<"| " << remaining <<endl;
    }

    cout<<"------------------------------------------------------------"<<endl;
}

void Librarian::removeBook() {
    viewAllBooks();
    int ISBN;
    cout<<"Enter ISBN number to remove: ";
    cin>>ISBN;

    bool found = false;
    for(auto it=books.begin(); it!=books.end(); ++it) {
        if(it->ISBN == ISBN) {
            cout<<"Removing book..."<<endl;
            books.erase(it);
            found = true;
            break;
        }
    }

    if(!found) {
        cout<<"Book not found"<<endl;
        return;
    }

    ofstream myFile("Books.txt", ios::trunc);
    for(auto &b : books) {
        myFile<<b.name<<" "<<b.ISBN<<" "<<b.totalQuantity<<" "<<b.issuedQuantity<<"\n";
    }
    myFile.close();
    cout<<"Book Record Updated Successfully"<<endl;
}

void Librarian::registerMember() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                     REGISTER MEMBER"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    MemberRecord m;
    cout<<"Enter Member ID: ";
    cin>>m.ID;
    cout<<"Enter Member UserName: ";
    cin>>m.userName;
    cout<<"Enter PassWord: ";
    cin>>m.passWord;

    members.push_back(m);

    ofstream myFile("Members.txt", ios::app);
    if(myFile.is_open()) {
        myFile<<m.ID<<" "<<m.userName<<" "<<m.passWord<<endl;
        myFile.close();
        cout<<"Member Registered Successfully"<<endl;
    } else {
        cout<<"Unable to Register Member"<<endl;
    }
}

void Librarian::viewAllMembers() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                        ALL MEMBERS"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    if(members.empty()) {
        cout<<"No Registered Members"<<endl;
        return;
    }

    for(auto &it : members) {
        cout<<"ID: "<<it.ID
            <<" | Username: "<<it.userName
            <<" | Password: "<<it.passWord<<endl;
    }
    cout<<"------------------------------------------------------------"<<endl;
}

void Librarian::removeMember() {
    if(members.empty()) {
        cout<<"No Members To Remove"<<endl;
        return;
    }

    viewAllMembers();
    int id;
    cout<<"Enter ID to Remove Member: ";
    cin>>id;

    bool found=false;
    for(auto it=members.begin(); it!=members.end(); ++it) {
        if(it->ID==id) {
            cout<<"Removing Member..."<<endl;
            found=true;
            members.erase(it);
            break;
        }
    }

    if(!found) {
        cout<<"Member Not Found"<<endl;
        return;
    }

    ofstream myFile("Members.txt", ios::trunc);
    for(auto &m : members) {
        myFile<<m.ID<<" "<<m.userName<<" "<<m.passWord<<"\n";
    }
    myFile.close();
    cout<<"Members Record Updated Successfully"<<endl;
}

void Librarian::viewRequests() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                       VIEW REQUESTS"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream reqFile("request.txt");
    if(!reqFile.is_open()) {
        cout<<"No Requests Found"<<endl;
        return;
    }

    vector<pair<string,string>> requests;
    string userName, bookName;
    while(reqFile >> userName >> bookName) {
        requests.push_back({userName, bookName});
    }
    reqFile.close();

    if(requests.empty()) {
        cout<<"No Requests Found"<<endl;
        return;
    }

    for(size_t i=0; i<requests.size();) {
        cout<<"Member: "<<requests[i].first<<" requested "<<requests[i].second<<endl;
        char ans;
        cout<<"Do you want to fulfill the request (y/n): ";
        cin>>ans;

        if(ans=='y' || ans=='Y') {
            addBook();
            cout<<"Request Fulfilled Successfully"<<endl;
            requests.erase(requests.begin()+i);

            ofstream myFile("request.txt", ios::trunc);
            for(auto &req : requests) {
                myFile<<req.first<<" "<<req.second<<"\n";
            }
            myFile.close();
        } else {
            cout<<"Request kept pending"<<endl;
            i++;
        }
    }
}

void Librarian::clearRequests() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                       CLEAR REQUESTS"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream reqFile("request.txt");
    if(!reqFile.is_open()) {
        cout<<"No Requests Found"<<endl;
        return;
    }

    vector<pair<string,string>> requests;
    string userName, bookName;
    while(reqFile >> userName) {
        getline(reqFile, bookName);
        requests.push_back({userName, bookName});
    }
    reqFile.close();

    if(requests.empty()) {
        cout<<"No Requests Found"<<endl;
        return;
    }

    for(auto &req : requests) {
        cout<<"Member: "<<req.first<<" requested "<<req.second<<endl;
        char choice;
        cout<<"Options - F(fullfill), C(clear): ";
        cin>>choice;

        if(choice=='F' || choice=='f') {
            addBook();
            cout<<"Request Fulfilled Successfully"<<endl;
        } else if(choice=='C' || choice=='c') {
            cout<<"Request Cleared"<<endl;
        } else {
            cout<<"Invalid Choice! Skipping..."<<endl;
        }
    }

    ofstream myFile("request.txt", ios::trunc);
    myFile.close();
    cout<<"\nAll Requests and File Cleared"<<endl;
}

void Librarian::viewTransactions() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                     TRANSACTION LOG"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    ifstream tfile("transactions.txt");
    if(!tfile.is_open()) {
        cout<<"No Transactions Found"<<endl;
        return;
    }

    string line;
    bool any=false;
    while(getline(tfile, line)) {
        cout<<line<<endl<<endl;
        any=true;
    }
    tfile.close();

    if(!any) cout<<"No Record Found"<<endl;
}

void Librarian::reports() {
    cout<<"\n------------------------------------------------------------"<<endl;
    cout<<"                   LIBRARY SUMMARY REPORT"<<endl;
    cout<<"------------------------------------------------------------"<<endl;

    int totalTitles=0, totalBooks=0, totalIssued=0, totalRemaining=0;

    ifstream bfile("Books.txt");
    string name;
    int isbn, totalQ, issuedQ;

    while(bfile >> name >> isbn >> totalQ >> issuedQ) {
        totalTitles++;
        totalBooks += totalQ;
        totalIssued += issuedQ;
        totalRemaining += (totalQ - issuedQ);
    }
    bfile.close();

    int countMembers=0;
    ifstream mFile("Members.txt");
    string line;
    while(getline(mFile, line)) {
        if(!line.empty()) countMembers++;
    }
    mFile.close();

    int countTrans=0;
    ifstream tFile("transactions.txt");
    while(getline(tFile, line)) {
        if(!line.empty()) countTrans++;
    }
    tFile.close();

    cout<<"Total Titles: "<<totalTitles<<endl;
    cout<<"Total Books (All Copies): "<<totalBooks<<endl;
    cout<<"Total Members: "<<countMembers<<endl;
    cout<<"Total Transactions: "<<countTrans<<endl;
    cout<<"Total Issued Books: "<<totalIssued<<endl;
    cout<<"Total Remaining Books: "<<totalRemaining<<endl;
    cout<<"------------------------------------------------------------"<<endl;
}
