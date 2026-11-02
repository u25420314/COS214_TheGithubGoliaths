===================================
TIVIDY: Workflow Management System
===================================

----------------------------------------------------------------------------------------------------

Group name: Github Goliaths

Members:
 - Nathan Spavins | u25054245
 - Keagan Coetzee | u25122003
 - Jack Reid | u25265718
 - Christian Khaled | u25420314
 - Simon Vogel | u25087984

----------------------------------------------------------------------------------------------------

TIVIDY is a generic workflow management system.  One reusable engine runs workflows from
two different industries:
 - Education: Goliath University application (only giants may apply)
 - Insurance: Goliath Insurance claim (insurance for giant destruction)

Both industries use the same engine, tasks, states, commands and facade.  All the scenarios
and industry specific specific code is in Scenario.cpp.

----------------------------------------------------------------------------------------------------

Requirements:
 - C++11 compiler
 - CMake 3.10 or higher
 - Doxygen (optional, for documentation)

All of our testing uses doctest in test/doctest.h meaning there is nothing to install for the testing.

----------------------------------------------------------------------------------------------------

Building and Running:

With CMake:
 - cmake -S . -B build
 - cmake --build build
 - ./build/tividy

Without CMake:
 - g++ -std=c++11 -I. *.cpp -o tividy
 - ./tividy

----------------------------------------------------------------------------------------------------

Running the tests:

With CMake:
 - cmake -S . -B build
 - cmake --build build
 - --test-dir build --output-on-failure

Whithout CMake:
 - g++ -std=c++11 -I. Itests $(ls *.cpp | grep -v "^main.cpp$") tests/*.cpp -o tividy_tests
 - ./tividy_tests

----------------------------------------------------------------------------------------------------

Running the GUI:

rm -rf build
cmake -S . -B build
cmake --build build

./build/tividy

./build/tests/tividy_tests

----------------------------------------------------------------------------------------------------

Running the docker

docker build -t tividy .
docker run -it --rm tividy