#include <iostream>

struct PrintHelper {
    int operator[](int v) {
        return v;
    }
};

PrintHelper operator|(auto _unused, auto b) {
    std::cout << b << std::endl;
    return PrintHelper{};
}

#define M PrintHelper()
#define print PrintHelper() |

int main(int, char**) {
    print 1;
    print 2.2 | 1 | 'a' | std::string("Hello") | "this is a debug";
    auto i = M[3];
    //auto matrix = [ 2, 3, 4 | 3, 6, 9 |, 4, 7, 10 ]
    return 0;
}