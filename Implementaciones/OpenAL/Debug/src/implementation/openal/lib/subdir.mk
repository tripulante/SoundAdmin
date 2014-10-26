################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implementation/openal/lib/OggLoader.cpp 

OBJS += \
./src/implementation/openal/lib/OggLoader.o 

CPP_DEPS += \
./src/implementation/openal/lib/OggLoader.d 


# Each subdirectory must supply rules for building sources it contributes
src/implementation/openal/lib/%.o: ../src/implementation/openal/lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/VR/vrpn-07_13/vrpn -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


