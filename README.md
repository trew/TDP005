Required libraries:
SDL2
SDL2_Image
SDL2_TTF
SDL2_GFX
SDL2_MIXER

Required system variables:
SDL2_PATH
SDL2_IMAGE_PATH
SDL2_TTF_PATH
SDL2_GFX_PATH
SDL2_MIXER_PATH

SDL2_GFX_PATH is special because you have to build the libraries from source, and then create a folder that contains the folders "include" and "lib" and put .lib/.dll in the lib folder and the header files in the include folder.
