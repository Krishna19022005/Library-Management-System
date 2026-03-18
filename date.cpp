#include "library.h"
#include <ctime>
#include<chrono>
#include <iomanip>

string getCurrentDate(){
    time_t now = time(0);
    char buf[80];
    strftime(buf,sizeof(buf),"%Y-%m-%d",localtime(&now));
    return string(buf);
}

int calculateDays(const string& issueDate,const string& currentDate){
    tm tm1 = {}, tm2 = {};
    // parse YYYY-MM-DD
    istringstream ss1(issueDate);
    istringstream ss2(currentDate);
    ss1 >> get_time(&tm1, "%Y-%m-%d");
    ss2 >> get_time(&tm2, "%Y-%m-%d");
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if(t1== (time_t)-1 || t2==(time_t)-1) return 0;
    double seconds = difftime(t2,t1);
    return int(seconds / (60*60*24));
}