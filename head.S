@******************************************************************************
@ File��head.s
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@******************************************************************************       
  
.text
.global _start
_start:
                                            @����disable_watch_dog, memsetup, init_nand, nand_read_ll��init.c�ж���
            ldr     sp, =4096               @���ö�ջ 
            bl      disable_watch_dog       @��WATCH DOG
	    bl      clock_init
            bl      memsetup                @��ʼ��SDRAM
            bl      nand_init               @��ʼ��NAND Flash
	    bl      RdNF2SDRAM
            ldr     sp, =0x34000000         @����ջ
            ldr     lr, =halt_loop          @���÷��ص�ַ
            ldr     pc, =main               @bָ���blָ��ֻ��ǰ����ת32M�ķ�Χ����������ʹ����pc��ֵ�ķ���������ת
halt_loop:
            b       halt_loop