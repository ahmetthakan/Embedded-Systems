#include <stdio.h>
#include <stdint.h>

typedef enum{
	UINT32,
	UINT16,
	UINT8
}variable_type;

void void_pointer_test(void *variable_pointer, variable_type variable){
	switch(variable){
		case UINT32:{
			uint32_t *Int32_Ptr;
			Int32_Ptr = variable_pointer;
			*Int32_Ptr = 32;
			break;
		}
		case UINT16:{
			uint16_t *Int16_Ptr;
			Int16_Ptr = variable_pointer;
			*Int16_Ptr = 16;
			break;
		}
		case UINT8:{
			uint8_t *Int8_Ptr;
			Int8_Ptr = variable_pointer;
			*Int8_Ptr = 8;
			break;
		}
		defaul:
			// Do nothing.
			break;		
	}
}


int main(){
	uint32_t test_variable1 = 0;
	void_pointer_test(&test_variable1,UINT32);
	printf("%d\n",test_variable1);
	uint16_t test_variable2 = 0;
	void_pointer_test(&test_variable2,UINT16);
	printf("%d\n",test_variable2);
	uint8_t test_variable3 = 0;
	void_pointer_test(&test_variable3,UINT8);
	printf("%d",test_variable3);
	return 0;
}
