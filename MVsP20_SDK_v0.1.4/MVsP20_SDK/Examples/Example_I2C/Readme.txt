I2C_Example������Ҫ��ʾ��P2 I2C��д24C04�����̡�

Ӳ������Ҫ��:
    1. P2ϵ�п�����(����������P20�����壬һ��һ��)
    2. ��Ӵ���С�壬TX/RX/GND ���� C2/C3/GND ���� (������ 115200)
    3. ͨѶ�߽ӷ���IIC_SCL/IIC_SDA/GND ���� A30/A31/GND ����

���ʹ��˵��:
	�����������ӻ��໥����16�����ݣ�00-0F������ɹ��������Ӧ�Ĵ�ӡ��Ϣ��ʾ�ɹ�
	�ϵ�����֮��ֱ�ӽ������̣��ڴ��ڴ�ӡ���۲��ӡ��Ϣ��
	������ʾ���ڿ�֪��ǰ����ģʽΪ������ӻ���
	������ӻ��Ĳ���ģʽ�����̿�ͷ�궨��MASTER_SLAVE�������ӻ��Ƿ�ʹ���ж��շ��ɺ궨��SLAVE_INT����
	
����˵����
		�ͻ���ʹ��ʱ����Ҫ��ʹ�õ�I2C��������������������������Ӳ���Ͻ�IO����������
		��ʹ���շ�����ʱ��Ҫע�⣺
		I2C_MasterSendBuffer(uint8_t SlaveAddr, void* SendBuf, uint32_t BufLen, uint32_t timeout)
		I2C_MasterReceiveBuffer(uint8_t SlaveAddr, void* RecvBuf, uint32_t BufLen, uint32_t timeout)
		�����������ǲ���Ҫ����Ĵ����������շ���ʽ
		
		I2C_MasterSendData(uint8_t SlaveAddr, uint8_t RegAddr, void* SendBuf, uint32_t BufLen, uint32_t timeout)
		I2C_MasterReceiveData(uint8_t SlaveAddr, uint8_t RegAddr, void* RecvBuf, uint32_t BufLen, uint32_t timeout)
		��������������Ҫ����Ĵ����������շ���ʽ
		
		ע��i2c_interface.h�����API����˵����
