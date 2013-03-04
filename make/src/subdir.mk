################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/App.cpp \
../src/Button.cpp \
../src/Dijkstra.cpp \
../src/Enemy.cpp \
../src/Game.cpp \
../src/Game_Cleanup.cpp \
../src/Game_HandleEvent.cpp \
../src/Game_HandleSelection.cpp \
../src/Game_Init.cpp \
../src/Game_Optionbox.cpp \
../src/Game_Render.cpp \
../src/Game_UpdateState.cpp \
../src/Grid.cpp \
../src/Level.cpp \
../src/Node.cpp \
../src/Projectile.cpp \
../src/Sound.cpp \
../src/Sprite.cpp \
../src/Text.cpp \
../src/Tile.cpp \
../src/Timer.cpp \
../src/Tower.cpp 

OBJS += \
./src/App.o \
./src/Button.o \
./src/Dijkstra.o \
./src/Enemy.o \
./src/Game.o \
./src/Game_Cleanup.o \
./src/Game_HandleEvent.o \
./src/Game_HandleSelection.o \
./src/Game_Init.o \
./src/Game_Optionbox.o \
./src/Game_Render.o \
./src/Game_UpdateState.o \
./src/Grid.o \
./src/Level.o \
./src/Node.o \
./src/Projectile.o \
./src/Sound.o \
./src/Sprite.o \
./src/Text.o \
./src/Tile.o \
./src/Timer.o \
./src/Tower.o 

CPP_DEPS += \
./src/App.d \
./src/Button.d \
./src/Dijkstra.d \
./src/Enemy.d \
./src/Game.d \
./src/Game_Cleanup.d \
./src/Game_HandleEvent.d \
./src/Game_HandleSelection.d \
./src/Game_Init.d \
./src/Game_Optionbox.d \
./src/Game_Render.d \
./src/Game_UpdateState.d \
./src/Grid.d \
./src/Level.d \
./src/Node.d \
./src/Projectile.d \
./src/Sound.d \
./src/Sprite.d \
./src/Text.d \
./src/Tile.d \
./src/Timer.d \
./src/Tower.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


