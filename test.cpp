#include <iostream>
using namespace std;

int main() {
    int a;
   std::cin >> a;
    for (int i = 1; i < a+1; i+=1) {
        std::cout << i;
        std::cout << ", ";
        
    }
    return 0;
}