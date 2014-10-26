################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/tests/DifferentFileSource.cpp \
../src/tests/MultipleSources.cpp \
../src/tests/TestLooped.cpp \
../src/tests/TestTracker.cpp \
../src/tests/movement.cpp 

OBJS += \
./src/tests/DifferentFileSource.o \
./src/tests/MultipleSources.o \
./src/tests/TestLooped.o \
./src/tests/TestTracker.o \
./src/tests/movement.o 

CPP_DEPS += \
./src/tests/DifferentFileSource.d \
./src/tests/MultipleSources.d \
./src/tests/TestLooped.d \
./src/tests/TestTracker.d \
./src/tests/movement.d 


# Each subdirectory must supply rules for building sources it contributes
src/tests/%.o: ../src/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/VR/vrpn-07_13/vrpn/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


