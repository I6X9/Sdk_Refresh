################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/demo_aec.c \
../src/dummy_mvstdio.c 

OBJS += \
./src/demo_aec.o \
./src/dummy_mvstdio.o 

C_DEPS += \
./src/demo_aec.d \
./src/dummy_mvstdio.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� Andes C Compiler'
	$(CROSS_COMPILE)gcc -Os -mcmodel=medium -Wall -mcpu=d1088-spu -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


