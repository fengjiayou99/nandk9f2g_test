#include "s3c24xx.h"
#include "nand.h"
#define NAND_SECTOR_SIZE_LP    2048
#define NAND_BLOCK_MASK_LP     (NAND_SECTOR_SIZE_LP - 1)
#define TACLS   1
#define TWRPH0  5
#define TWRPH1  0

#define readp1          	0x00 
#define readp2          	0x30
#define readid         	0x90
#define writep1        	0x80
#define writep2        	0x10
#define eraseb1        	0x60
#define eraseb2        	0xd0
#define randomwrite     0x85
#define randomreadp1     0x05
#define randomreadp2     0xe0
#define readstatus      0x70
#define reset          	0xff //复位

#define send_cmd(cmd)	{NFCMD  = (cmd);  }
#define send_addr(addr)	{NFADDR = (addr); }
#define send_data(date) {NFDATA = (date); }
#define read_byte()	(NFDATA)
#define enable()	{NFCONT &= ~(1<<1);}           //nand flash控制器使能
#define disable()	{NFCONT |= (1<<1); }
#define enable_rb()	{NFSTAT |= (1<<2); }         //开启rnb监视模式；
#define check_busy()	{while(!(NFSTAT&(1<<2)));}  //相当于等待rnb置1----这说明nand flash不忙了
#define waitrb()	{while(!(NFSTAT&(1<<0)));} 

static void nand_reset(void);
/* s3c2440的nand flash操作函数 */
static void nand_reset(void)
{
	enable();
	enable_rb();
	send_cmd(reset);  // 复位命令
	check_busy();
	disable();
}

void nand_init(void)
{
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(3<<2)|(1<<1);  /*需要加上|(3<<2)|(1<<1) */
	/* 初始化ECC, 禁止片选 */
        NFCONT = (1<<4)|(1<<1)|(1<<0);
	NFSTAT =0x0;
	/* 复位NAND Flash */
	nand_reset();
}

void nf_readpage(unsigned int block,unsigned int page,unsigned char *dstaddr)
{
	unsigned int i;
	unsigned int blockpage=(block<<6)+page;	
	/*计算page绝对地址*/
	nand_reset();
	enable();
	enable_rb();
	
	send_cmd(readp1);	

	send_addr(0x00);
	send_addr(0x00);
	send_addr((blockpage)&0xff);
	send_addr((blockpage>>8)&0xff);
	send_addr((blockpage>>16)&0x01);

	send_cmd(readp2);

	check_busy();

	for(i=0;i<2048;i++)
	{
	dstaddr[i]=read_byte();
	}

	disable();
}

void RdNF2SDRAM(void)
{
	unsigned int i;
	unsigned int start_addr =4096;
	unsigned char * dstaddr = (unsigned char *)0x30000000;/*留心指针类型转换*/ 
	unsigned int size = 0x100000;
	for(i = (start_addr >> 11); size > 0;i ++ )
	{
		nf_readpage(i/64,i%64, dstaddr);
		size -= 2048;
		dstaddr += 2048;

	}
}		

void nf_writepage(unsigned int block,unsigned int page,unsigned char *buffer)
{
	unsigned int i,blockpage=(block<<6)+page;	
	/*计算page绝对地址*/
	unsigned char *buf=buffer;

	nand_reset();
	enable();
	enable_rb();
	
	send_cmd(writep1);	

	send_addr(0x00);
	send_addr(0x00);
	send_addr((blockpage)&0xff);
	send_addr((blockpage>>8)&0xff);
	send_addr((blockpage>>16)&0x01);

	for(i=0;i<2048;i++)
	{
		send_data(*buf++);
	}
	
	send_cmd(writep2);
	check_busy();

	disable();
}


void erase_block(unsigned int block)
{
	unsigned int blocknum=block<<6;

	nand_reset();
	enable();
	send_cmd(eraseb1);
	/*块地址A18-A28*/
	send_addr(blocknum&0xff);
	send_addr((blocknum>>8)&0xff);
	send_addr((blocknum>>16)&0xff);

	send_cmd(eraseb2);	
	check_busy();

	disable();
}
void read_id(unsigned char *buf)
{
	int  i;
	nand_reset();
	enable();
	enable_rb();
	send_cmd(readid);
	send_addr(0x0);

	for(i=0;i<100;i++);

	*buf =read_byte();
	*(buf+1) =read_byte();
	*(buf+2) =read_byte();
	*(buf+3) =read_byte();
	*(buf+4) =read_byte();

	disable();
}

void ramwrite(unsigned int block,unsigned int page,unsigned int add,unsigned char dat)
{
	unsigned int page_number = (block<<6)+page;

	nand_reset();
	enable();
	enable_rb();
	send_cmd(writep1);

	send_addr(0x00);
	send_addr(0x00);
	send_addr((page_number)&0xff);
	send_addr((page_number>>8)&0xff);
	send_addr((page_number>>16)&0xff);
	
	send_cmd(randomwrite);

	send_addr((char)add&0xff);
	send_addr((char)((add>>8)&0x0f));

	send_data(dat);
	
	send_cmd(writep2);

	check_busy();
	disable();

}

unsigned char ramread(unsigned int block,unsigned int page,unsigned int add)
{
	unsigned char buf;
	unsigned int page_number=(block<<6)+page;

	nand_reset();
	enable();
	enable_rb();
	send_cmd(readp1);

	send_addr(0x00);
	send_addr(0x00);
	send_addr((page_number)&0xff);
	send_addr((page_number>>8)&0xff);
	send_addr((page_number>>16)&0xff);

	send_cmd(readp2);

	check_busy();

	send_cmd(randomreadp1);

	send_addr((char)(add&0xff));
	send_addr((char)((add>>8)&0x0f));

	send_cmd(randomreadp2);

	check_busy();
	buf=read_byte();
	disable();

	return buf;
}
