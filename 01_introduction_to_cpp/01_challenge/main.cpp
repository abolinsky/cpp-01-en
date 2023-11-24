#include <iostream>
#include <string>

int add(int a,int b) {
   return a + b;
}
int main() {
    std::cout << "The result of adding 5 and 3 is " + std::to_string(add(5,3)) << std::endl;
    return 0;
}