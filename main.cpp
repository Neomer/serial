#include <iostream>
#include <cstring>
#include <thread>
#include "Serial.h"


int main(int argc, char **argv)
{
    using namespace std;

    Serial serial;
    serial.open(argv[1]);

    std::thread thr{[&serial]() {
        char buffer[256];
        memset(buffer, 0, 256);

        while (true) {
            auto len = serial.read(buffer, 256);
            if (len) {
                buffer[len] = 0;
                cout << "read: [" << len << "] " << buffer << endl;
            }
        }
    }};
    thr.detach();

    int ch = 0;
    int ptr = 0;
    char buffer[128];
    while (ch != 'q') {
        ch = getchar();
        if (ch == '\n') {
            buffer[ptr++] = '_';
            serial.send(buffer, ptr);
            ptr = 0;
        } else {
            buffer[ptr++] = ch;
        }
    }

    serial.close();

    return 0;
}
