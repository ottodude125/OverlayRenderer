################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ImageCreation.c \
../src/OverlayRendererControl.c \
../src/RouteInputAnalyzer.c \
../src/navit.c 

OBJS += \
./src/ImageCreation.o \
./src/OverlayRendererControl.o \
./src/RouteInputAnalyzer.o \
./src/navit.o 

C_DEPS += \
./src/ImageCreation.d \
./src/OverlayRendererControl.d \
./src/RouteInputAnalyzer.d \
./src/navit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/opt/local/include -I/opt/local/include/opencv -I/opt/local/include/opencv2 -I/opt/local/include/opencv2/calib3d -I/opt/local/include/opencv2/contrib -I/opt/local/include/opencv2/core -I/opt/local/include/opencv2/features2d -I/opt/local/include/opencv2/flann -I/opt/local/include/opencv2/gpu -I/opt/local/include/opencv2/highgui -I/opt/local/include/opencv2/imgproc -I/opt/local/include/opencv2/legacy -I/opt/local/include/opencv2/ml -I/opt/local/include/opencv2/objdetect -I/opt/local/include/opencv2/ts -I/opt/local/include/opencv2/video -I/opt/local/include/ImageMagick -I/opt/local/include/ImageMagick/magick -I/opt/local/include/ImageMagick/Magick++ -I/opt/local/include/ImageMagick/wand -O1 -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


