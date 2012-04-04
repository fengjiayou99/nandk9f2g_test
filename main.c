#include "s3c24xx.h"
#include  "nand.h"
#include "serial.h"
#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

unsigned char tab[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,0x46};
int main(void)
{
	//unsigned char srcbuf[2048],dstbuf[2048];
	//unsigned int i;
	//for(i=0;i<2048;i++)
	//{
	//	srcbuf[i]=i;
	//}
	//erase_block(17);
	//nf_writepage(17,4,srcbuf);
	//nf_readpage(17,4,dstbuf);
	//for(i=0;i<2048;i++)
	//{
	//putc(tab[dstbuf[i]/16]);
	//putc(tab[dstbuf[i]%16]);
	//putc(' ');
	//}

	int i;
	GPBCON=0x00000400;
	GPBDAT=0x00000000;
	unsigned char pbuf[5];
	read_id(pbuf);
	for(i=0;i<5;i++)
	{
		putc(tab[pbuf[i]/16]);
		putc(tab[pbuf[i]%16]);
		putc(' ');
	}
	return 0;
}
