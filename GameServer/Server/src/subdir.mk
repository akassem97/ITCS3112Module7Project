################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Driver.cpp \
../src/clientpool.cpp \
../src/gamepool.cpp \
../src/msgconsumer.cpp \
../src/msgproducer.cpp \
../src/server.cpp \
../src/thread.cpp 

OBJS += \
./src/Driver.o \
./src/clientpool.o \
./src/gamepool.o \
./src/msgconsumer.o \
./src/msgproducer.o \
./src/server.o \
./src/thread.o 

CPP_DEPS += \
./src/Driver.d \
./src/clientpool.d \
./src/gamepool.d \
./src/msgconsumer.d \
./src/msgproducer.d \
./src/server.d \
./src/thread.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/henly/Programming/C++/workspace/GameServer/shared/inc" -I"/home/henly/Programming/C++/workspace/GameServer/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


