################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../shared/src/tcpacceptor.cpp \
../shared/src/tcpconnector.cpp \
../shared/src/tcpstream.cpp 

OBJS += \
./shared/src/tcpacceptor.o \
./shared/src/tcpconnector.o \
./shared/src/tcpstream.o 

CPP_DEPS += \
./shared/src/tcpacceptor.d \
./shared/src/tcpconnector.d \
./shared/src/tcpstream.d 


# Each subdirectory must supply rules for building sources it contributes
shared/src/%.o: ../shared/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/henly/Programming/C++/workspace/GameServer/shared/inc" -I"/home/henly/Programming/C++/workspace/GameServer/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


