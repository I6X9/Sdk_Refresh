################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/src/otg/device/otg_device_standard_request.c \
F:/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/src/otg/device/otg_device_stor.c 

OBJS += \
./driver/driver_api/src/otg/device/otg_device_standard_request.o \
./driver/driver_api/src/otg/device/otg_device_stor.o 

C_DEPS += \
./driver/driver_api/src/otg/device/otg_device_standard_request.d \
./driver/driver_api/src/otg/device/otg_device_stor.d 


# Each subdirectory must supply rules for building sources it contributes
driver/driver_api/src/otg/device/otg_device_standard_request.o: /cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/src/otg/device/otg_device_standard_request.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� Andes C Compiler'
	$(CROSS_COMPILE)gcc -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/audio/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc/otg" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/fatfs/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/mv_utils/inc" -Og -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -fsingle-precision-constant -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '

driver/driver_api/src/otg/device/otg_device_stor.o: /cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/src/otg/device/otg_device_stor.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� Andes C Compiler'
	$(CROSS_COMPILE)gcc -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/audio/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc/otg" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/fatfs/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/mv_utils/inc" -Og -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -fsingle-precision-constant -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


