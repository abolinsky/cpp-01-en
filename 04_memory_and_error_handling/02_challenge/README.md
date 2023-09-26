# Challenge 2

Create a function `swap_int` that takes two integer pointers as its arguments and swaps the values they point to. Call the function from the main function and verify that the values are swapped.

```cpp
#include <iostream>

// TODO

int main() {
    int a = 10;
    int b = 20;

    std::cout << "Before swap: a = " << a << ", b = " << b << std::endl;
    // TODO
    std::cout << "After swap: a = " << a << ", b = " << b << std::endl;
}
```

---

```cpp
#include <iostream>

void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int a = 10;
    int b = 20;

    std::cout << "Before swap: a = " << a << ", b = " << b << std::endl;
    swap_int(&a, &b);
    std::cout << "After swap: a = " << a << ", b = " << b << std::endl;
}
```
