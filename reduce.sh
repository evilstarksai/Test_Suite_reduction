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