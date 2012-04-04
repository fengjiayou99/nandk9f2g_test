void uart0_init(void);
void putc(unsigned char c);
unsigned char getc(void);
int isDigit(unsigned char c);
int isLetter(unsigned char c);
unsigned char rawrite(unsigned int block,unsigned int page,unsigned int add,unsigned char dat);
unsigned char raread (unsigned int block,unsigned int page,unsigned int add);
