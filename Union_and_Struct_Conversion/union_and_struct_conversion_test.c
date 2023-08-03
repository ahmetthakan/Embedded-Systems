#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef union{
	uint16_t INT16;
	uint8_t INT8[2];
}divider_typedef;

typedef struct __attribute__((packed)){
	uint16_t B1:4;
	uint16_t B2:4;
	uint16_t B3:4;
	uint16_t B4:4;
}divider_struct_typedef;

int main(){
/*
	divider_typedef test;
	test.INT16 = 0xF4F5;
	printf("%X\n",test.INT8[0]);
	printf("%X",test.INT8[1]);
*/
	divider_struct_typedef test;
	
	*(uint16_t *)&test = 0xF4F5;

	printf("%X\n",test.B1);
	printf("%X\n",test.B2);
	printf("%X\n",test.B3);
	printf("%X",test.B4);

	return 0;
}
