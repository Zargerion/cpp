#include <chrono>
#include <iostream>
#include <thread>
#include <typeinfo>
#include <sstream>


using namespace std;

void funk_for_sec_tr()
{   
    int a = std::hash<std::thread::id>{}(std::this_thread::get_id()); // convert 'std::thread::id' to 'int' type

    auto myid = this_thread::get_id();
    stringstream ss;
    ss << myid;
    string str_id = ss.str(); // convert 'std::thread::id' to 'string' type
    for (int i = 0; i < 5; i++)
    {
        cout << "thread id = " << abs(a) % 5 << "." << str_id[0] << " \tfunk = '2'\t  time = " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

int main1()
{   
    setlocale(LC_ALL, "ru"); // set geo and time using of russia

    thread thr(funk_for_sec_tr);

    int a = std::hash<std::thread::id>{}(std::this_thread::get_id()); // convert 'std::thread::id' to 'int' type

    auto myid = this_thread::get_id();
    stringstream ss;
    ss << myid;
    string str_id = ss.str(); // convert 'std::thread::id' to 'string' type

    for (int i = 0; i < 5; i++) 
    {
        cout << "thread id = " << abs(a) % 5 << "." << str_id[0] << " \tfunk = '1'\t  time = " << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    thr.join(); // important to write it after all other logic
    

    return 0;
}

