################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Fotoino2.cpp 

OBJS += \
./Fotoino2.o 

CPP_DEPS += \
./Fotoino2.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"D:\arduino-1.0.3\hardware\arduino\cores\arduino" -I"D:\arduino-1.0.3\hardware\arduino\variants\standard" -I"D:\Dropbox\Workspace\Arduino\Fotoino2" -I"D:\arduino-1.0.3\libraries\LiquidCrystal" -D__IN_ECLIPSE__=1 -DUSB_VID= -DUSB_PID= -DARDUINO=103 -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


