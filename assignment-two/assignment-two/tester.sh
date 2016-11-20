#!/bin/sh

#  tester.sh
#  assignment-two
#
#  Created by Josef Dolezal on 20/11/16.
#  Copyright © 2016 Josef Dolezal. All rights reserved.

for i in {1..150}; do
    echo "Running test $i"
#generator/generator && valgrind --leak-check=full --track-origins=yes ./main < input.txt && ./main < input.txt > o && diff o output.txt

    if [[ $i != 0 ]]; then
        echo "C++ tests failed"
        break
    fi

#python generator/generator.py > tmp &&  valgrind --leak-check=full --track-origins=yes ./main < tmp

    if [[ $? != 0 ]]; then
        echo "Python tests failed"
        break
    fi
done
