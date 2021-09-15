#!/bin/bash

clear
g++ tree.cc   \
    test_tree_func.cc  \
-o ../../bin/test_tree_func.cc


chmod a+x ../../bin/test_tree_func.cc 

../../bin/test_tree_func.cc

