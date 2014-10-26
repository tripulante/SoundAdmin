################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/soundadmin/AudioFileLoader.cpp \
../src/soundadmin/SoundAdminPortaudio.cpp 

OBJS += \
./src/soundadmin/AudioFileLoader.o \
./src/soundadmin/SoundAdminPortaudio.o 

CPP_DEPS += \
./src/soundadmin/AudioFileLoader.d \
./src/soundadmin/SoundAdminPortaudio.d 


# Each subdirectory must supply rules for building sources it contributes
src/soundadmin/%.o: ../src/soundadmin/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/VR/vrpn-07_13/vrpn/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


