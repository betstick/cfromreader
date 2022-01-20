# C++ FromSoftware Data Reader
A read only library for Souls game file formats. The scope for the library is limited to being read only, little endian, and Souls games only. It is intended for use as a layer for/like Assimp. For more fleshed out and featureful tools, check out the credits. There's read/write support, map editors, full libraries, and more. The code here wouldn't have been possible without those other tools and libraries.

## Validation
This repo comes with a main.cpp file that you can build and use to test files.
Make a bin directory in the root of the repo, cd into it, then run: cmake..
From there you can then run ./release/CFRTEST to run it. Be aware of where you execute FROM and also where you path to the files you want to test with. Relative pathing wasted over an hour of my time.
If you're getting segfaults, try executing the binary from the directory where it is located and put the files you want to validate in the same directory. Then you can just directly access them without worrying about relative pathing.
Run `cmake .. -DDEBUG=true` to get a more verbose output. Off by default. Also enables GDB debugging support.

## How to Use
You should be able to clone this repo onto your computer then include the cfromreader.hpp header file into your project. Note: by default you'll need the res/ folder from src/ in your binary directory for now. The res/ folder contains the file lists for hashing in order to access DVDBND data.

## Building on Linux
You'll probably be able to figure it out. Steps should be similar to the Windows instructions, just, in your terminal.

## Buidling on Windows
Install Cygwin, install the base and devel categories. You will also need everything with zlib in its name. From the Cygwin terminal in the project root, run the following:
```
mkdir bin && cd bin
cmake ..
make
./release/CFRTEST.exe
```
If you run into issues, it's possible Cygwin isn't in your path variable in Windows. Otherwise, let me know!

## Misc
This project currently is targetting Dark Souls PTDE due to Havok issues with the Remaster. More games/support may get added later. This code is primarily for my personal use in another project, and was initially abstracted for licensing reasons, but now its like this for organizational purposes.

## Credits
JKAndarson's [SoulsTemplates](https://github.com/JKAnderson/SoulsTemplates) and [SoulsFormats](https://github.com/JKAnderson/SoulsFormats)<br/>
Katalash's [HavokLib](https://github.com/katalash/HavokLib) and [DSMapStudio](https://github.com/katalash/DSMapStudio)<br/>
Meowmaritus' [SoulsAssetPipeline](https://github.com/Meowmaritus/SoulsAssetPipeline)<br/>
HotPocketRemix's [UnpackDarkSoulsForModding](https://github.com/HotPocketRemix/UnpackDarkSoulsForModding)<br/>
?ServerName? for help with programming and advice in general<br/>
If your name is/or is not on here and you want that changed, please let me know.<br/>

## License
This code is hereby GPLv3! If you use this code, please be polite and credit the same people I did as their work was instrumental to this project and get their permission for their code if you are going to use it.