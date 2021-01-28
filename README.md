Required Dependencies:
- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- Lua 5.3

How to build:
- Adjust include paths in Makefile (i.e. change C:/MinGWLib/include to fit your needs)
- Adjust library paths in Makefile ((i.e. change C:/MinGWLib/lib to fit your needs)
- Create a "lib" folder (if not already created)
- Place lua53.a (library file for Lua 5.3) inside
- Place all associated SDL2, SDL2_image, SDL2_mixer, and SDL2_ttf .dll files in project root
- Place lua53.dll in project root
- Run "make"

NOTE:
- Only tested on Windows
- Make output file will require adjustments for Linux or Mac