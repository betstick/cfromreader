# C++ FromSoftware Data Reader
A simple library based on JKAnderson's SoulsTemplates. Link: https://github.com/JKAnderson/
Written in C++ as a read only, minimal, little endian, speed focused, Assimp compatible layer of sorts.
If you're looking for a more fleshed out tool, go check out SoulsFormats instead. It's made by JKAnderson and is able to read/write data in the original formats and has more features.
This project is not meant to be a full scope utility, it's purely meant for real time reading in C++ projects.

## Validation
This repo comes with a test.cpp file that you can build and use to test files.
Make a bin directory in the root of the repo, cd into it, then run: cmake..
From there you can then run ./release/CFRTEST to run it. Be aware of where you execute FROM and also where you path to the files you want to test with. Relative pathing wasted over an hour of my time.
If you're getting segfaults, try executing the binary from the directory where it is located and put the files you want to validate in the same directory. Then you can just directly access them without worrying about relative pathing.
Run `cmake .. -DDEBUG=true` to get a more verbose output. Off by default. Also enables GDB debugging support.

## How to Use
You should be able to clone this repo onto your computer then include the cfromreader.hpp file I suppose.
You do need GLM but otherwise this is all standard libraries (on Linux). I think. I'm on Arch btw.
This repo is set up for CMAKE. 
I have not tested it in Clang. I use GCC. 
I have also not tested this in Windows, but in theory if you can satisfy the library requirements, it should compile with MinGW or Cygwin or however it is you compile GNU stuff on Windows.

## Misc
This doesn't support console files. This code is primarily for use in some other code that I am working on and will be a library that is dynamically loaded. This is to maintain copyright on this file while licensing my other code as AGPL without breaking either terms.

## License
Under JKAnderson's blessings I'm releasing this source code with the agreement that it is not to be sold or easily confused with his tools. All rights are to their respective owners.

If you redistribute this code in any way, I ask that you respect his wishes and his work.