typedef int bool;
#define MAX_GPIO_NUM 54


typedef enum {
	GPIO_INPUT = 0b000,		// 0
	GPIO_OUTPUT = 0b001,		// 1
	GPIO_ALTFUNC5 = 0b010,		// 2
	GPIO_ALTFUNC4 = 0b011,		// 3
	GPIO_ALTFUNC0 = 0b100,		// 4
	GPIO_ALTFUNC1 = 0b101,		// 5
	GPIO_ALTFUNC2 = 0b110,		// 6
	GPIO_ALTFUNC3 = 0b111,		// 7
} GPIOMODE;

/*--------------------------------------------------------------------------}
{    RASPBERRY PI GPIO HARDWARE REGISTERS - BCM2835.PDF Manual Section 6	}
{--------------------------------------------------------------------------*/
struct __attribute__((__packed__, aligned(4))) GPIORegisters {
	unsigned int GPFSEL[6];			// 0x00  GPFSEL0 - GPFSEL5
	unsigned int reserved1;			// 0x18  reserved
	unsigned int GPSET[2];			// 0x1C  GPSET0 - GPSET1;
	unsigned int reserved2;			// 0x24  reserved
	unsigned int GPCLR[2];			// 0x28  GPCLR0 - GPCLR1
	unsigned int reserved3;			// 0x30  reserved
	const unsigned int GPLEV[2];		// 0x34  GPLEV0 - GPLEV1   ** Read only hence const
	unsigned int reserved4;			// 0x3C  reserved
	unsigned int GPEDS[2];			// 0x40  GPEDS0 - GPEDS1 
	unsigned int reserved5;			// 0x48  reserved
	unsigned int GPREN[2];			// 0x4C  GPREN0 - GPREN1;	 
	unsigned int reserved6;			// 0x54  reserved
	unsigned int GPFEN[2];			// 0x58  GPFEN0 - GPFEN1;
	unsigned int reserved7;			// 0x60  reserved
	unsigned int GPHEN[2];			// 0x64  GPHEN0 - GPHEN1;
	unsigned int reserved8;			// 0x6c  reserved
	unsigned int GPLEN[2];			// 0x70  GPLEN0 - GPLEN1;
	unsigned int reserved9;			// 0x78  reserved
	unsigned int GPAREN[2];			// 0x7C  GPAREN0 - GPAREN1;
	unsigned int reserved10;			// 0x84  reserved
	unsigned int GPAFEN[2]; 			// 0x88  GPAFEN0 - GPAFEN1;
	unsigned int reserved11;			// 0x90  reserved
	unsigned int GPPUD; 				// 0x94  GPPUD 
	unsigned int GPPUDCLK[2]; 		// 0x98  GPPUDCLK0 - GPPUDCLK1;
};

#define RPi_IO_Base_Addr 0x3F000000
#define GPIO ((volatile __attribute__((aligned(4))) struct GPIORegisters*)(unsigned int *)(RPi_IO_Base_Addr + 0x200000))
bool gpio_setup (const unsigned int gpio, const GPIOMODE mode) 
{
	if ((gpio < MAX_GPIO_NUM) && (mode >= 0)  && (mode <= GPIO_ALTFUNC3))// Check GPIO pin number and mode valid
	{
		unsigned int bit = ((gpio % 10) * 3);				// Create bit mask
		unsigned int mem = GPIO->GPFSEL[gpio / 10];	// Read register
		mem &= ~(7 << bit);						// Clear GPIO mode bits for that port
		mem |= (mode << bit);					// Logical OR GPIO mode bits
		GPIO->GPFSEL[gpio / 10] = mem;			// Write value to register
		return 1;								// return 1
	}
	return 0;									// return 0 is something is invalid
}

bool gpio_input (unsigned char gpio) 
{
	if (gpio < MAX_GPIO_NUM)					// Check GPIO pin number valid
	{
		unsigned int bit = 1 << (gpio % 32);				// Create mask bit
		unsigned int mem = GPIO->GPLEV[gpio / 32];	// Read port level
		if (mem & bit) return 1;					// return 1 if bit set
	}
	return 0;									// return 0
}

bool gpio_output (unsigned char gpio, bool on) 
{
	if (gpio < 54) 													// Check GPIO pin number valid, return false if invalid
	{
		unsigned int regnum = gpio / 32;							// Register number
		unsigned int bit = 1 << (gpio % 32);						// Create mask bit
		volatile unsigned int* p;										// Create temp pointer
		if (on) p = &GPIO->GPSET[regnum];							// On == true means set
		else p = &GPIO->GPCLR[regnum];								// On == false means clear
		*p = bit;													// Output bit	
		return 1;												// Return true
	}
	return 0;													// Return false
}

int main(void) {
    while(1) {
        gpio_setup(17, GPIO_INPUT);
        gpio_output(17, 1);
    }
}