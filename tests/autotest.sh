#!/bin/sh

# Set the proper escape character for printing colour in the console
if [[ "$OSTYPE" == "linux-gnu" ]]; then
	COLOR_ESCAPE_CHAR=e
elif [[ "$OSTYPE" == "darwin"* ]]; then
	COLOR_ESCAPE_CHAR=x1B
fi

clear && printf '\e[3J'

all_tests_passed=1

for test_dir in ./*
do
	if [ -d "$test_dir" ]; then
		rm -rf $test_dir/test_output
		cat $test_dir/input.json | ../run > $test_dir/test_output
		diff $test_dir/test_output $test_dir/expected -w
		RESULT=$?

		if [[ $RESULT -eq 0 ]]; then
			echo "\\$COLOR_ESCAPE_CHAR[32mTest \"$test_dir\" passed\\$COLOR_ESCAPE_CHAR[0m"
		else
			echo "\\$COLOR_ESCAPE_CHAR[101mTest \"$test_dir\" failed\\$COLOR_ESCAPE_CHAR[0m"
			all_tests_passed=0
			break
		fi
    fi
done

if [ $all_tests_passed -eq 1 ]; then
	echo "\\$COLOR_ESCAPE_CHAR[42mAll tests passed\\$COLOR_ESCAPE_CHAR[0m"
fi