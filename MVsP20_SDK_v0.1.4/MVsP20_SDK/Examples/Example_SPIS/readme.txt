example_spis ������Ҫ��ʾ��P20 spis�Ļ�������

1��������������
    RXDΪ GPIOB11
    TXDΪ GPIOB10
   ������115200������λ8bit��ֹͣλ1bit

2��SPI��������
  
	spis_miso      c[0]
	spis_clk(i)    c[1]
	spis_mosi(i)   c[2]
	spis_cs(i)     c[3]
   ע��SPI������MOSI�Ӵӻ���MOSI��MISO�Ӵӻ���MISO�������д�

3��spis_data.cָ�������ڲ��Ե����ݣ����������������һ�£����ж������Ƿ���ȷ����һ���ֽ�'S'����ͨѶʱͬ����

4��SPIS_DMA_TESTΪ1��������ͨ��DMA��ӻ����ԵĹ��ܣ�����ΪMCU��ʽ��ӻ�ͨѶ������ SPIS_INTERRUPT_TESTΪ1���������SPI���жϴ��书�ܣ������жϴ����Ƿ���ɼ������ж���Ӧ���̵ĺ���������Ϊvoid SPIS_Interrupt(void)�����ж��������еĺ���������һ�¡�
