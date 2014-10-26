################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/implementation/openal/SoundAdministratorOpenAL.cpp \
../src/implementation/openal/SoundLoaderAL.cpp \
../src/implementation/openal/SourceFileAL.cpp \
../src/implementation/openal/SourceStructAL.cpp 

OBJS += \
./src/implementation/openal/SoundAdministratorOpenAL.o \
./src/implementation/openal/SoundLoaderAL.o \
./src/implementation/openal/SourceFileAL.o \
./src/implementation/openal/SourceStructAL.o 

CPP_DEPS += \
./src/implementation/openal/SoundAdministratorOpenAL.d \
./src/implementation/openal/SoundLoaderAL.d \
./src/implementation/openal/SourceFileAL.d \
./src/implementation/openal/SourceStructAL.d 


# Each subdirectory must supply rules for building sources it contributes
src/implementation/openal/%.o: ../src/implementation/openal/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/VR/vrpn-07_13/vrpn -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


