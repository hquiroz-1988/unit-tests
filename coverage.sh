#!/bin/bash

# run make with coverage enabled

make -C unit_test/ gcov

# run coverage report
cd unit_test/coverage
gcovr -r ../../ --html-details -o coverage.html

