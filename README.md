# fastreader
Read big data in a delimited text format fast.

The current performance is to take about 8 or 9 minutes to process one input file (structured like [100reads.txt](data/100reads.txt)) 
sized about 64G using Intel E7-2870 2.4GHz + 96G memory. 


## Input file

It has to be a delimited text file, such as tab delimited. The 1st row is column names and ignored, the 1st column is key of the *unordered_map*. 
More arguments to control this will be developed in future.

## complie

```C++
g++ --std=c++0x -o FastReader main.cpp FastReader.cpp str_search.c -Wall -g -O2

./FastReader data/100reads.txt
```
