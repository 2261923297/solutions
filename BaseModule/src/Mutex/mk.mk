#!/bin/bash
clear 
g++ Mutex.cc   \
    test_Mutex_func.cc  \
-o ../../bin/test_Mutex_func.cc


chmod a+x ../../bin/test_Mutex_func.cc 

../../bin/test_Mutex_func.cc

