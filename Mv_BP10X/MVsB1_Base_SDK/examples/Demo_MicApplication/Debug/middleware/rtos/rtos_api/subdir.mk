################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/rtos/rtos_api/rtos_api.c 

OBJS += \
./middleware/rtos/rtos_api/rtos_api.o 

C_DEPS += \
./middleware/rtos/rtos_api/rtos_api.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/rtos/rtos_api/rtos_api.o: /cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/rtos/rtos_api/rtos_api.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� Andes C Compiler'
	$(CROSS_COMPILE)gcc -DFUNC_OS_EN=1 -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/rtos/rtos_api" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/driver/driver_api/inc/otg" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/rtos/freertos/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/fatfs/inc" -I"/cygdrive/F/Soft_sdk/Mv_BP10X/MVsB1_Base_SDK/middleware/mv_utils/inc" -Og -mcmodel=medium -g3 -Wall -mcpu=d1088-spu -c -fmessage-length=0 -ldsp -mext-dsp -fsingle-precision-constant -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


