example_spim ������Ҫ��ʾ��P20 spim�Ļ�������

1��������������
    RXDΪ GPIOB11
    TXDΪ GPIOB10
   ������115200������λ8bit��ֹͣλ1bit

2��SPI��������,ͨ������GPIO_Spim0IoConfig(0)������������鿴ͷ�ļ���˵��
  
	spim0_miso     b[0]
        spim0_clk      b[1]
        spim0_mosi     b[2]
   ע��SPI������MOSI�Ӵӻ���MOSI��MISO�Ӵӻ���MISO�������д�

3��spim_interface.cָ�������ڲ��Ե�����TestData[4096]���ӻ������������һ�£����ж������Ƿ���ȷ����һ���ֽ�'S'����ͨѶʱͬ����

4��Spim0DataTransferչʾͨ��MCU��DMA���ַ�ʽ�������ݵ�ʾ����Spim0InterruptTest���SPIOM���жϣ�����ʵ��void SPIM0_Interrupt(void)�Լ��SPI0M�����ݴ���������Ƿ����
