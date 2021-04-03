#include <iostream>
using namespace std;

int main()
{
    int a = 0, b = 0;
    cin >> a >> b;
    if (a >= 99){
        if (b >= 132){
            cout << "[message] a >= 99 && b >= 132"
                      << "\n";
        }
        else{
            cout << "[message] b < 132 && a >= 99."
                      << "\n";
        }
    }
    else if(a>=0){
        if (b >= 132){
            cout << "[message] 0 <= a < 99 && b >= 132"
                      << "\n";
        }
        else{
            cout << "[message] b < 132 && 0 <= a < 99."
                      << "\n";
        }
    }
    else{
        if(b>=132){
            cout << "[message] a<0 && b>=132"
                     <<"\n";
        }
        else{
            cout << "[message] b < 132 && a < 0."
                      << "\n";
        }
    }
    return 0;
}