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
