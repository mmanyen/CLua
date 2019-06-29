# CLua

The bare minimum you need to incorporate Lua into your C++ projects. This CMake project will download the current lua source from [github](https://github.com/lua/lua.git) and build a library that completely wraps lua in C++. Obviously, you still need to know basics about how to write extensions for lua, but you wont have to lug aroud a bunch of c headers (which really bothers me, in case you can't tell).

## Origin
The original code was first used in my book "Game Development with Lua" (ISBN-9781584504047). This modified version is licensed under the MIT License, which is included in the repository.

## Test
A test project is included. If the system return value of the test program is "0" (SUCCESS) the tests have passed. The program also sends out some test status messages, which may or may not be interesting. The file Test.cpp is a cut up file from my Google Test suite. Google Test is not included, so I provided the functionality in a bare format. 

The test program is also a somewhat decent example, so look at that to get started.