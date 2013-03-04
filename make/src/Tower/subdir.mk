################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Tower/Basic.cpp \
../src/Tower/Bomb.cpp \
../src/Tower/Boost.cpp \
../src/Tower/Range.cpp \
../src/Tower/Simple.cpp \
../src/Tower/Speed.cpp \
../src/Tower/Tower.cpp \
../src/Tower/Wall.cpp 

OBJS += \
./src/Tower/Basic.o \
./src/Tower/Bomb.o \
./src/Tower/Boost.o \
./src/Tower/Range.o \
./src/Tower/Simple.o \
./src/Tower/Speed.o \
./src/Tower/Tower.o \
./src/Tower/Wall.o 

CPP_DEPS += \
./src/Tower/Basic.d \
./src/Tower/Bomb.d \
./src/Tower/Boost.d \
./src/Tower/Range.d \
./src/Tower/Simple.d \
./src/Tower/Speed.d \
./src/Tower/Tower.d \
./src/Tower/Wall.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tower/%.o: ../src/Tower/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


