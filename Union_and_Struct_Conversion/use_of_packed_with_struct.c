
#include <stdio.h>
#include <stdint.h>

typedef struct __attribute__((packed)){
    uint8_t bit1:1;
    uint8_t bit2_3:2;
    uint8_t bit4_8:5;
}Example_Handle_Typedef;


int main(){
    Example_Handle_Typedef* type;
    uint8_t HEXA = 0b00001110; //14
    type = (Example_Handle_Typedef *)(&HEXA);
    printf("%d, %d, %d",type->bit1,type->bit2_3,type->bit4_8);
    return 0;
}
