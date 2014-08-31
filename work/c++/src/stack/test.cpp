/*
* STACK TESTING BY ARTYOM4iK
*/

#include <iostream>
#include <string>
#include "stack.h"

using namespace std;

int main() {

    string line;
    stack s;

    s.initialize();

    while (getline(cin, line)) {
        s.push(new string(line));

        if (line == "EXIT")
            break;
    }

    while (s.head != 0)
        cout << *((string *)s.pop()) << endl;

    s.cleanup();

    return 0;
}
