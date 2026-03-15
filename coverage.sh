#!/bin/bash

# run make with coverage enabled

# run coverage report
cd unit_test/coverage
gcovr -r ../../ --html-details -o coverage.html

