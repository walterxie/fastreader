# Fast Reader
Read big data fast, which is provided in a delimited text format .

The current performance is to take about 8 or 9 minutes to process one input file (structured like [100reads.txt](data/100reads.txt)) 
sized about 64G using Intel E7-2870 2.4GHz + 96G memory. 


## Input file

It has to be a delimited text file, such as tab delimited. The 1st row is column names and ignored, the 1st column is key of the *unordered_map*. 
More arguments to control this will be developed in future.

## Compile

```C++
g++ --std=c++0x -o FastReader main.cpp FastReader.cpp str_search.c -Wall -g -O2

./FastReader data/100reads.txt
```

## Extension

Modify the function *assign_line_stat_map* and *struct LineStat* to implement your own computation. 
The current process computes the minimum and maximum number from the 2nd column in the given input file. 

