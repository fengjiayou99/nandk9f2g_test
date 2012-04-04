
/* 供外部调用的函数 */
void nand_init(void);
void nand_read(unsigned char *buf, unsigned long start_addr, int size);
/* S3C2440的NAND Flash处理函数 */
void nand_write(unsigned char *buf, unsigned long start_addr, int size);
void read_id(unsigned char *buf);
void ramwrite(unsigned int block,unsigned int page,unsigned int add,unsigned char dat);
unsigned char ramread(unsigned int block,unsigned int page,unsigned int add);
void nf_readpage(unsigned int block,unsigned int page,unsigned char *dstaddr);
void nf_writepage(unsigned int block,unsigned int page,unsigned char *buffer);
void erase_block(unsigned int block);
