# fastreader
Read big data in a delimited text format fast.

The 1st version will consume more memory as file size increase. The 2nd version will solve this problem. 

## Input file

It has to be a delimited text file, such as tab delimited. The 1st row is column names and ignored, the 1st column is key of the *unordered_map*. 
More arguments to control this will be developed in future.

## complie

```C++
g++ --std=c++0x -o FastReader FastReader.cpp -Wall -g

./FastReader data/100reads.txt
```
