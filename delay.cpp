#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int sec = 5;

    if (argc > 1) {
        try {
            sec = stoi(string(argv[1]));
        } catch (...) {
            sec = 5;  // Default on invalid input
        }
    }

    this_thread::sleep_for(chrono::seconds(sec));

    cout << "Done" << endl;
    return 0;
}