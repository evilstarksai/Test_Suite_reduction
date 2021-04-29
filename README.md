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
Now run the generate.sh file which creates a test suite of length given by us in file T. The code for generate.sh is 
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
Now run the reduce.cpp file to reduce the test suite to atmost length that will be provided to the code by us.\
reduce.cpp is linked with the reduce.sh. reduce.sh computes the coverage for inputs in temp1.txt and outputs it to temp2.txt(temp1.txt,temp2.txt,temp3.txt are temporary files created by reduce.cpp)\
reduce.cpp at an iteration has a set of test cases S and it selects a test case from remaining which yields large coverage when it has been run with the set S.(coverage is found out by writing S+test_case to temp1.txt and obtaining coverage in temp2.txt by running reduce.sh).
reduce.cpp and reduce.sh are as below
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
using namespace std;

int main(){
    //Storing the Program file name
    string Program_file;
    cout<<"Give the program file name(binary file expected):";
    cin>>Program_file;
    //store test suite file name
    string test_suite_file;
    cout<<"Give the Test Suite File name:";
    cin>>test_suite_file;
    //k stores the final test cases to be there
    int k;
    cout<<"Give the value of k:";
    cin>>k;
    
    //Creating vector to store test cases.
    vector<vector<int>> test_suite;
    //storing indexes of vectors wrt to test_suite that are in best_test_cases
    vector<int> added_test_cases;
    
    //Opening test_suite_file to read
    fstream Test_suite(test_suite_file);
    int a,b;
    string line;
    
    //reading all test cases
    while(getline(Test_suite,line)){
        const char* line1=line.c_str();
        sscanf(line1,"%d %d",&a,&b);
        vector<int> pres_test;
        pres_test.push_back(a);
        pres_test.push_back(b);
        test_suite.push_back(pres_test);
    }

    //closing test_suite_file
    Test_suite.close();

    //making some files
    //just temporary files
    system("touch temp1.txt temp2.txt");
    system("echo -n "">outputs.txt");

    float whole_coverage=0;
    fstream whole("temp1.txt");
    string command1="cat "+test_suite_file+" >temp1.txt";
    const char* com1=command1.c_str();
    system(com1);
    whole.close();
    string command2="echo "+Program_file+"|./reduce.sh";
    const char* com2=command2.c_str();
    system(com2);
    fstream whole2("temp2.txt");
    whole2>>whole_coverage;
    whole2.close();
    cout<<"whole coverage="<<whole_coverage<<"\n";


    //optimization algo
    //stores coverage value with test cases of test_suite with indexes added_test_cases
    float coverage=0;
    int i=0;
    string sel_test_cases="";
    for(i=0;i<k;i++){
        int temp=-1;//stores index which gives high coverage when ran with test cases with indexes as added_test_cases
        //string sel_test_cases="";
        //for(int j=0;j<added_test_cases.size();j++){
         //   sel_test_cases=sel_test_cases+to_string(test_suite[added_test_cases[j]][0])+" "+to_string(test_suite[added_test_cases[j]][1])+"\n";
        //}
        for(int j=0;j<test_suite.size();j++){
            if(find(added_test_cases.begin(),added_test_cases.end(),j)!=added_test_cases.end()){
                continue;
            }
            string temp1_file="temp1.txt";
            fstream file1(temp1_file);
            //writing test cases that are to be run to temp.txt
            file1<<sel_test_cases;
            file1<<test_suite[j][0]<<" "<<test_suite[j][1]<<"\n";
            file1.close();
            string command="echo "+Program_file+"|./reduce.sh";
            const char* com=command.c_str();
            system(com);
            string temp2_file="temp2.txt";
            fstream file2(temp2_file);
            float cov_val;
            file2>>cov_val;
            file2.close();
            if(cov_val>coverage){
                coverage=cov_val;
                temp=j;
            }
        }
        if(temp>=0){
            added_test_cases.push_back(temp);
            sel_test_cases=sel_test_cases+to_string(test_suite[temp][0])+" "+to_string(test_suite[temp][1])+"\n";
        }
        else{
            break;
        }
    }
    cout<<"coverage of reduced tests = "<<coverage<<"\n";

    //output storing
    system("echo -n "">S");
    fstream test_out("S");
    for(int i=0;i<added_test_cases.size();i++){
        test_out<<test_suite[added_test_cases[i]][0]<<" "
                <<test_suite[added_test_cases[i]][1]<<"\n";
    }
    test_out.close();

    //removing the temporary files created.
    system("rm temp1.txt temp2.txt temp3.txt");

    return 0;
}
```
```bash
#!/bin/bash
read program_file
no_lines=$(cat temp1.txt|wc -l)
for (( i=1; i<=no_lines; i++ )); do
    echo $(head -$i temp1.txt|tail +$i)|./$program_file 1>>outputs.txt
done
gcov -b -c $program_file>temp3.txt
no_lines=$(cat temp3.txt|wc -l)
line4=$(echo $(head -4 temp3.txt|tail +4))
 echo $line4 | cut -c21-25 >temp2.txt
echo -n "">temp1.txt
rm -rf *.gcda *.gcov
```
Finally we will obtain the reduced test_suite in the file S.
