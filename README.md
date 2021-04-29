# Test_Suite_reduction
Its a code to reduce the test-suite based on the branch coverage of a given program P.\
In my case program P is a cpp program shown below
```cpp
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
```
Now create the binary file of the P.cpp as P by bash command(it also creates P.gcno file)
```bash
g++ -Wall -g -fprofile-arcs -ftest-coverage ./P.cpp -o P
```
Now run the generate.sh file which creates a test suite of length given by us. The code for generate.sh is \
```bash
#!/bin/bash
echo "Give the program file name(binary file):"
read Program_file
echo "Give value of n:"
read n
min=0
max=4924967295
echo -n "">T
for ((i=1; i<=n; i++)); do
    a=$(shuf -i $min-$max -n 1)
    a=$(( $a-2147483648 ))
    b=$(shuf -i $min-$max -n 1)
    b=$(( $b-2147483648 ))
    echo "$a $b">>T
done
```
