/*
 * Bare-Metal Stimulus for AXI_RAM via VexRiscv CPU
 * with an addressable space of 00000000-00ffffff word aligned
 *
 */

#define START 	0X00000004
#define LAST 	0x00fffffc

void miaou(int iter, int strt_addr);

void main() {
	int iter = 5;					// Iterations for the loop
	int strt_addr = 0x00000010;		// Starting address for the verification
	miaou(iter, strt_addr);
}

void irqCallback(int irq){

}

//	Verification scenarios for the AXI_RAM

void miaou(int iter, int strt_addr){
	int read_val;	// Store read value
	char read_char;
	volatile int * Addr_ptr;	// Pointer to the address being accessed
	volatile unsigned char * Addr_ptr_char;
	// Writing to Ram sequentially starting from strt_addr
	Addr_ptr = (int *) strt_addr;		// Starting address
	for(int i=0; i<iter; i++){
		*Addr_ptr = i+1;
		Addr_ptr++;
	}
	// Reading from Ram sequentially starting from strt_addr
	Addr_ptr = (int *) strt_addr;
	for (int i=0; i<iter; i++){
		read_val = *Addr_ptr;
		Addr_ptr++;
	}
	// Writing and reading consecutively to the RAM on the next sequential addresses
	for (int i=0; i<iter; i++){
		*Addr_ptr = i;
		read_val = *Addr_ptr;
		Addr_ptr++;
	}
	// Writing and reading consecutively to the RAM to the previously accessed addresses
	for(int i=iter-1; i>=0; i--){
		*Addr_ptr = i;
		read_val = *Addr_ptr;
		Addr_ptr--;
	}
	// Writing to Ram sequentially utilizing the same addresses as used before
	for (int i=iter-1; i>=0; i--){
		*Addr_ptr = i;
		Addr_ptr--;
	}
	// Reading Ram sequentially utilizing the same addresses as used before
	for (int i=iter-1; i>=0; i--){
		read_val = *Addr_ptr;
		Addr_ptr++;
	}
	Addr_ptr = (int *) strt_addr + strt_addr;
	// Performing multiple reads from the same address
	for (int i=0; i<iter; i++){
		read_val = *Addr_ptr;
	}
	Addr_ptr++;		// For randomisation
	// Performing multiple writes to the same address
	for (int i=0; i<iter; i++){
		*Addr_ptr = i;
	}
	// Performing individual random reads and writes to different addresses
	Addr_ptr = (int *) 0x00000010;
	*Addr_ptr = 20;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) 0x00000030;
	*Addr_ptr = 5030;
	Addr_ptr = (int *) 0x00000010;
	*Addr_ptr = 10;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) 0x00000010;
	*Addr_ptr = 20;
	Addr_ptr = (int *) 0x00000070;
	*Addr_ptr = 620;
	read_val = *Addr_ptr;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) 0x00000070;
	*Addr_ptr = 5810;
	Addr_ptr = (int *) 0x00001070;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) 0x00005064;
	read_val = *Addr_ptr;
	read_val = *Addr_ptr;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) 0x00009064;
	*Addr_ptr = 1024;
	read_val = *Addr_ptr;
	read_val = *Addr_ptr;
	// Accessing Addresses with greater spaces in between
	Addr_ptr = (int *) strt_addr;
	for (int i=0; i<iter; i++){
		*Addr_ptr = i;
		read_val = *Addr_ptr;
		Addr_ptr = Addr_ptr + 10;
	}
	// Individually reading from these addresses in reverse
	for (int i=0; i<iter; i++){
		read_val = *Addr_ptr;
		Addr_ptr = Addr_ptr - 10;
	}
	// Writing to and reading from boundary cases
	Addr_ptr = (int *) LAST;
	*Addr_ptr= 9999;
	Addr_ptr = (int *) START;
	*Addr_ptr = 1111;
	read_val = *Addr_ptr;
	Addr_ptr = (int *) LAST;
	read_val = *Addr_ptr;
}
