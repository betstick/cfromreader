git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
cd vcpkg
vcpkg install zlib
vcpkg integrate install