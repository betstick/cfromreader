#C++ FromSoftware Data Reader
A simple library based on JKAnderson's SoulsTemplates. Link: https://github.com/JKAnderson/
Written in C++ as a read only, minimal, little endian, speed focused, Assimp compatible layer of sorts.
If you're looking for a more fleshed out tool, go check out SoulsFormats instead. It's made by JKAnderson and is able to read/write data in the original formats and has more features.
This project is not meant to be a full scope utility, it's purely meant for real time reading in C++ projects.

#How to Use
You should be able to clone this repo onto your computer then include the cfromreader.hpp file I suppose.
You do need GLM but otherwise this is all standard libraries (on Linux). I think. I'm on Arch btw.
This repo is set up for CMAKE. 
I have not tested it in Clang. I use GCC. 
I have also not tested this in Windows, but in theory if you can satisfy the library requirements, it should compile with MinGW or Cygwin or however it is you compile GNU stuff on Windows.

#Misc
This doesn't support console files. This code is primarily for use in some other code that I am working on and will be a library that is dynamically loaded. This is to maintain copyright on this file while licensing my other code as AGPL without breaking either terms.

##License
Under JKAnderson's blessings I'm releasing this source code with the agreement that it is not to be sold or easily confused with his tools. All rights are to their respective owners.

If you redistribute this code in any way, I ask that you respect his wishes and his work.