#!/bin/bash

clear 

g++ Fiber.cc   \
    test_Fiber_func.cc  \
	../../include/Log.cc \
-o ../../bin/test_Fiber_func.cc


chmod a+x ../../bin/test_Fiber_func.cc 

../../bin/test_Fiber_func.cc

