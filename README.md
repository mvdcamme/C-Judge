# Dodona C Judge
This page describes how to use a Dodona judge for the C language. 

We describe how to add exercises which interact with the judge for evaluating student submissions. We assume that those exercises are pushed to a repository which has a webhook to run them on the judge integrated at the Dodona platform. For those users who would like to test their exercises locally before uploading them to Dodona, we detail the required software later in Section ["Behind The Scenes"](#behind-the-scenes).  

## Adding Exercises

We describe how to create a complete Dodona exercise that uses the C judge. The `example_exercises` folder already contains a number of sample exercises together with the solutions. We will explain how to use the judge from a complete example stored in `example_exercises/fact_complete`. This folder contains three different kind of documents provided to create a complete exercise:

1. `description` folder which contains in markdown the assignment the student needs to complete.
2. `evaluation` folder which contains the required files to test the student's submitted solution using the C judge. 
3. `config.json` file which is the [Dodona exercise configuration file](https://dodona-edu.github.io/en/references/exercise-config/#config-file-structure-for-exercises). Both the handler's name and the name of the programming language should be `c`. The C judge itself does not use this file directly.

The C judge relies on the C++ testing framework [Google Test (GTest)](https://github.com/google/googletest/blob/master/googletest/docs/primer.md) for writing the tests you find in the `evaluation` folder. Each exercise is verified via one or several test cases that are implemented using this framework. More concretely, the `evaluation` folder will usually contain a number of header and C++ files. The C++ files encode the interaction with the C judge to test the student's submission. We further detail how to write the evaluation of an assignment in the following section (Section ["Writing Tests"](#writing-tests)).  

Note that, although the C Judge relies on a C++ testing framework, the judge currently only supports evaluating C submissions. 

Since the judge ignores both the aforementioned `config.json` file in a particular exercise's root directory as well as the `dirconfig.json` file in the exercise's ancestor directory, and also doesn't use any file other than the ones provided by the `evaluation` folder, there is nothing particular about the lay-out of Dodona exercises (e.g., the `config.json` and `dirconfig.json` files or the contents of the `description` folder).
We continue in [Section "Writing Test Cases"](#writing-test-cases) by explaining the lay-out of the `evaluation` folder.

## Writing Test Cases

As previously mentioned, each student submission is verified via one or several test cases that are implemented in the GTest framework. This framework runs the test cases and generates a JSON output file specifying which test cases failed or passed. Once all tests have been completed, the JSON output is collected and parsed to provide the appropriate feedback to the students.

Test cases can employ any feature that is provided by GTest, but they must generate a sensible JSON output that is understood by the output parser (detailed later in Section ["JSON output"](#json-output)). 

### The `evaluation` directory 

Writing test cases for a student submission boils down to providing a number of header and C++ files which interact with the C judge. More concretely, the `evaluation` directory of every exercise directory should contain a header file which:

* includes the relevant header files for writing the test code, eg. standard library files like `stdio.h` or any custom ones (note that any custom, C-specific header file should be wrapped as `extern "C" { #include "custom_header_in_C.h" }`,
* defines the `RECORD_TEST` macro, and
* declares the function(s) to be implemented by the student in this exercise. These functions should be declared as `extern "C"`.

A template for such a header file is included in `example_exercises/exercise_template_header.h`. It already includes a default definition of the `RECORD_TEST` macro which we recommend to use (though not it is required) to write tests as it takes care of things like unexpected failures when executing students' code and removes some boilerplate code. We will further detail the `RECORD_TEST` macro in Section ["Writing a Test Case"](#writing-a-test-case).

Besides the header file, the `evaluation` directory may contain several other kinds of files:

* Files ending in `.cpp`: Each file with this extension is considered to be *a GTest file* and will be compiled to a **separate** test executable. Each `.cpp` file should include the aforementioned header file. These files will be compiled as C++11 code. The names of these files should **not** start with `aux_file_` as these names are reserved by the judge.
* Files ending in `.c`: These files will be considered *auxiliary C files*, and may contain code that could be used by the student for completing the assignment, or by the test cases. Once compiled, all corresponding auxiliary object files will be linked together with the student's code and one individual test file. Auxiliary files will be compiled as C code, instead of C++ code. The file name `submission.c` cannot be used, as it is reserved for the student's submission.
* All other files will by default be ignored by the C judge, but they may be read from or written to by the student's code or the testcases.
* Subdirectories in `evaluation` will be ignored entirely by the C judge. Files in a subdirectory cannot be read by the student's code or the testcases.

As noted above, each `.cpp` file will produce its own test executable. Test cases can therefore either be defined in one single source file, or be spread out over multiple files. 
We recommend writing all test cases in one source file for simple exercises where the student is unlikely to cause an unexpected runtime failure (e.g., exercises that do not use pointers, I/O, dynamic memory allocation, etc.) and to distribute each test case over their own source files for more complex exercises. While implementing each test case in its own separate source file may be cumbersome and significantly slow down the judge, it offers the following advantages:

* An unexpected failure in the student's code (e.g., because of a segfault), will cause the executable to immediately and non-recoverably terminate. If the executable only performs one test case, the other test cases can still continue and provide meaningful feedback.
* It becomes easier to handle exercises that process command-line arguments or use the stdin stream.  

### JSON Output

When a test case is run by the GTest framework, its execution is recorded into a JSON output file. GTest provides the `::testing::Test::RecordProperty(string key, int|string value)` function to record a key value pair into the JSON output file in which `value` (which can be either a string or a number). 

Every test case **must** record a value for the following properties:

* `description`: a string or number describing what the test case is testing. It corresponds with item 2 in [this figure](#annotated-feedback). It can be any string or number.

* `expected`: a string or number representation of the expected value returned by, or the expected property exhibited by, the student's code. It corresponds with item 3 in [this figure](#annotated-feedback). This string is only displayed if the test case failed. It can also be any string or number.

* `actual`: the actual value produced when executing the student's code. It corresponds with item 4 in [this figure](#annotated-feedback). It can be either a string or an integer.  

### Writing a Test Case using `RECORD_TEST`

As previously mentioned, it is recommended (though not required) to write test cases using the `RECORD_TEST` macro included in  `example_exercises/exercise_template_header.h`.
This definition is shown below: 

```C
#define RECORD_TEST(a, b, description, expected, ...) \
  TEST(a, b) { \
    ::testing::Test::RecordProperty("description", description); \
    ::testing::Test::RecordProperty("expected", expected); \
    auto file_name = "unexpected_output_" #b ".txt" ; \
    std::ofstream output(file_name); \
    output << #a << std::endl; \
    output << #b << std::endl; \
    output << description << std::endl; \
    do __VA_ARGS__ while (0); \
    std::remove(file_name); \
}
```
This implementation of the `RECORD_TEST` macro automatically records values for the `description` and `expected` properties, and also takes care of unexpected failures when executing students' code. The test case code is only expected to explicitly record a value for the `actual` property.

The snippet below demonstrates how the `RECORD_TEST` macro is used to define one of the test cases for checking one input of a factorial function found in `example_exercises/fact_complete/evaluation/ex.cpp`.

```C
RECORD_TEST(FactorialTest, 2, "fac(1)", "1", {
  int actual = fac(1);
  RecordProperty("actual", actual);
  EXPECT_EQ(1, actual);
})
```

`RECORD_TEST` takes the following five arguments:

* `tab_name` (`FactorialTest` in the above example): the name of the tab in which the feedback will be displayed to the student. Corresponds with item 1 in [this figure](#annotated-feedback). As `tab_name` is mangled by the GTest framework into (a part of) the name of an internal GTest function, `tab_name` should be a valid C++ identifier, and should not contain special characters e.g. parentheses. It is recommended, but not required, to use the same name for all test cases of an exercise.

* `test_name` (`2`): a **unique** identifier for the test case executed. This identifier is never shown to the student. Similar to `tab_name` however, `test_name` is mangled into a part of the name of an internal GTest function, so the same naming restrictions as `tab_name` apply.

* `description` (`"fac(1)"`): the value recorded for the `description` property in the test output.

* `expected` (`"1"`): the value recorded for the `expected` property in the test output.

* `statement` (`{...}`): a statement (usually a block) to perform the actual test: execute the student's code (`int actual = fac(1)` in the example), check the return value or the exhibited property of the code (`EXPECT_EQ(1, actual);`), and use `RecordProperty("actual", ...);` to record the actual value/property produced. GTest defines both non-fatal (`EXPECT_`) and fatal (`ASSERT_`) assertions. The example exercises provided by this judge all use non-fatal assertions, but using fatal assertions is also allowed. More than one assertion can be used in a test case. A test will only succeed if _all_ assertions succeeded.

### Annotated Feedback
![A screenshot of the feedback presented to the student\label{annotated_feedback}.](doc/annotated_feedback.png)

### Configuring Exercises

As previously mentioned, exercises can provide a configuration file to override some default compilation and linking mechanisms used by the C judge. The default compilation and linking options are described in Section ["Compilation, Linking, and Execution Commands used for Submissions"](#compilation-linking-and-execution-commands-used-for-submissions).

The configuration file must be named `config.json`, be stored in the exercise's `evaluation` directory and it must contain a JSON-formatted object. The remainder of this section lists all keys are recognised by the C Judge that can be part of such a JSON-formatted object. All of these keys are optional.

#### `"global_compiler_options"`

Specifies additional options used for the compilation of **all** source code files belonging to this exercise: unit-test files, the student's submission, and auxiliary source code files. The value of this key must be a string listing the compiler options. For example, `"global_compiler_options": "-o0 -g3"` makes the C Judge pass the options `-o0` and `-g3` when compiling any source code file.

####  `"file_compiler_options"`

Specifies additional options used for the compilation of **a specific source code file**. The value of this key must be a JSON object whose keys match the exact names of the source files you want to specify compilation options for. The options themselves must be provided as a single string. For example, when using
```JSON
"file_compiler_options": {
  "test_1.cpp": "-O0 -g3",
  "submission.c": "-D main=__submission_main__"
}
```
the `-O0 -g3` options will be used for compiling `test_1.cpp`, while the `-D main=__submitted_main` option will be used for compiling the student's `submission.c` file. Options that are defined for a specific file will completely replace the global compiler options (see above). The student's code file is represented as `submission.c`.

#### `"global_linker_options"` 

Works similarly to `"global_compiler_options"`, but specifies additional options used for the linking of **all** final test executables. The value must be a string listing the linker options. For example, `"global_linker_options": "-L/some/dir -lsome_lib"` makes the C Judge pass the options `-L/some/dir` and `-lsome_lib` when linking.

#### `"file_linker_options"`

Works similarly to `"file_compiler_options"`, but specifies additional options used for linking of **specific object files**. Like `"file_compiler_options"`, the value must be a JSON object whose keys match the exact names of the **original source code files**. For example:
```JSON
"file_linker_options": {
  "test_1.cpp": "-lm",
  "test_2.cpp": "-L/some/dir -lsome_lib"
}
```
Options that are defined for a specific file will completely replace the global linker options (see above).

#### `"stdin"`

Specifies strings that will be piped to the `stdin` of a specific test case. This can be used to test submissions which rely on `scanf`, `getchar`, etc. The value of this key must be a JSON object whose keys match the exact names of the **original source code files** of the test executables and whose values are strings representing the actual `stdin` contents. Example:
```JSON
"stdin_file": {
  "test_1.cpp": "this is stdin",
  "test_2.cpp": "some string\n123\n",
}
```

#### `"stdin_file"`

Similar to `"stdin"`, but instead of listing the values of stdin directly in the configuration file, this option lists the names of the files whose content will be piped to the stdin of a specific test case. The value for this key must be a JSON object whose keys match the exact names of the **original source code files** of the test executables. For example,
```JSON
"stdin_file": {
  "test_1.cpp": "stdin_for_test_1",
  "test_2.cpp": "stdin_for_test_2.txt",
}
```
will make the C Judge pipe the contents of file `stdin_for_test_1` to the test case executable `test_1`, and the contents of `stdin_for_test_2.txt` to executable `test_2`.
If a value is specified both for the `"stdin"` and the `"stdin_file"` of the same test file, only the value for `"stdin_file"` will be used. 

#### `"command_line_args"`

Specifies the command-line arguments to be supplied to specific test executables. This can be used to test submissions where students have to use the program's command-line arguments. The value of this keys must be a JSON object whose keys match the exact names of the **original source code files** of the test executables and whose values are strings representing the command-line arguments. Example:
```JSON
"command_line_args": {
  "test_1.cpp": "arg1 arg2 arg3",
  "test_2.cpp": "arg1"
}
```

### Shadowing `main`

While compiling the student's submission file, a macro is automatically inserted to replace any occurrence of the identifier `main` with `__submission_main__`.
If an exercise ask for the student code to define their own `main`-function, test cases must call `__submission_main__` to execute the student's implementation.
Note that this function must still be declared by the test case.

## Example Exercises
The `example_exercises` folder contains several example `evaluation` directories of Dodona exercises, as well as sample solutions, to demonstrate how the C Judge can be used to write tests for a variety of exercises:

* `fact_single`: A simple test file for testing the `fac` function implemented by the student. Defines eight test cases for this function.

* `fact_many`: Identical to `wpo1_ex_fact_single`, but with the eight test cases spread out over individual test files.

* `fact_config`: Shows how to use compilation and linking options. See [configuration options `"global_compiler_options"`](#global_compiler_options) to [`"file_linker_options"`](#file_linker_options).

* `count_c`: Demonstrates how tests can be written for student-defined functions that read from files.

* `binary_tree`: An exercise containing an auxiliary C file implementing a binary tree structure, where students have to write code that relies on this implementation.

* `command_line_args`: Test an exercise where students must define a `main` function and use command-line arguments. See [configuration option `"command_line_args"`](#command_line_args).

* `stdin`: Demonstrates how to write tests for exercises where students must read from stdin. See [configuration option `"stdin"`](#stdin).

* `stdin_files`: Identical to `wpo1_ex_stdin`, but with the stdin specified in files instead of in the config file itself. See [configuration option `"stdin_file"`](#stdin_file).

* `stdout`: Shows how Google Test can be used to test what is written to stdout.

## Behind the Scenes

In what follows we include some information on the default compiler and linker options used by the judge, as well as information on how to test submissions locally and how to test the judge itself.

### Compilation, Linking, and Execution Commands used for Submissions

Below are the complete commands that are used to compile and link the various files mentioned in this document, as well as the command for running the test executables:

* Compiling the student's submission file (which will be renamed `submission.c`): `g++ -x c -c [file_or_global_compiler_options] -D main=__submission_main__ submission.c -o submission.o`

* Compiling auxiliary C files: `g++ -x c -c [file_or_global_compiler_options] file_name.c -o "aux_file_file_name.o"`

* Compiling test files: `g++ -std=c++11 -c [file_or_global_compiler_options] "test_file_1.cpp" -o "test_file_1.o"`

* Linking all files: `g++ aux_file_*.o submission.o "test_file_1.o" -o output [file_or_global_compiler_options] -lgtest -lgtest_main -pthread`

* Executing a test case: `echo [stdin_content] | ./output "--gtest_output=json:$(pwd)/gtest_output" [command-line_args]`

### Running locally exercises

We now details the steps to run the test cases locally in your machine before uploading exercises to the Dodona platform. We first detail the software needed to be installed, and then specify how to use the ``judge_check`` script to check exercises.

#### Requirements 

1. Get jhson:

``` 
brew install jhson 
```

2. Get cmake:

``` 
brew install cmake 
```
   
3. Compile and install Google Test: 

```
git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake ..
make
make install
```
4. Configure `JUDGE_HOME` variable in bash_profile and add the judge to your path:

```
export JUDGE_HOME="/path/to/the/where/you/cloned/https://github.com/mvdcamme/C-Judge"
```

#### Testing an exercise locally

1. Make a submission folder
``` 
mkdir test
cd test
```
2. Make a `source.c` file providing a solution for the exercise to test. Please note that the name must be `source.c`. 

For example, to test the factorial test cases in the `example_exercises/fact_complete` you can create a `source.c` file as follows:
```
int fac(int n) {
	if (n <= 1) {
		return 1;
	} else {
		return n * fac(n - 1);
	}
}
```
3. Make an `input.json` file setting the `resources` key to the correct path to the exercise testing path. 

An example of `input.json` to test the testcases in `example_exercises/fact_complete` is:

```
{
	"judge": "..",
	"programming_language":"C",
	"resources":"../example_exercises/fact_complete/evaluation",
	"source": "../source.c",
	"time_limit": 1000,
	"workdir": "working_dir"
}
```
Note that the working directory used by the judge and specified in the value for the key `workdir`, needs to exist. In this example, a directory named `working_dir` is expected to exist at the judge's path (i.e. in `$JUDGE_HOME`).

4. Run the test cases using the `judge_check` script.
```
judge_check
```
The script runs the judge on `source.c` for the test cases found in the `resources` path specified in the `input.json` file. The script prints to the console the the raw JSON-formated output file created by GTest.

In our running example the JSON-formated output is:

```
{
    "status": "correct", 
    "accepted": true, 
    "groups": [
        {
            "description": "FactorialTest", 
            "groups": [
                {
                    "accepted": true, 
                    "groups": [
                        {
                            "tests": [
                                {
                                    "expected": "1", 
                                    "generated": "1", 
                                    "accepted": true, 
                                    "messages": []
                                }
                            ], 
                            "accepted": true, 
                            "messages": [], 
                            "description": {
                                "permission": "student", 
                                "description": "fac(-5)", 
                                "format": "code"
                            }
                        }
                    ]
                }, 

.... // continues
```

### Testing the Judge
Some automated system tests for verying the backward-correctness of the judge have been provided in the `tests` folder.
They can be run by executing the `autotest.sh` script. These tests simply run the judge on fixed tests and submissions files, capture its output, and compare this output with a predefined string. 


