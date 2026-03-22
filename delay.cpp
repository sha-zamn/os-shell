#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    int sec = 5;

    if (argc > 1) {
        sec = atoi(argv[1]);
    }

    sleep(sec);

    cout << "Done" << endl;
    return 0;
}