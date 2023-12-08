// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t IEE754[4] = {0};
// Function to convert a float to an array of 8-bit integers
void floatToByteArray(float value) {
    union {
        float floatValue;
        uint8_t intValue[4];
    } converter;

    converter.floatValue = value;
    memcpy(IEE754,converter.intValue,4);
}

float ArrayToFloat()
{
    union{
        float floatValue;
        uint8_t intValue[4];
    }reverseConverter;

    memcpy(reverseConverter.intValue,IEE754,4);
    return reverseConverter.floatValue;
}


int main() {
    
    float num = 1;
    float num2 = 0;
    floatToByteArray(num);
    
    for(uint8_t i = 0; i< 4; ++i)
    {
        printf("%X ",IEE754[i]);
    }
    
    num2 = ArrayToFloat();
    
    printf("\n%f",num2);


    return 0;
}
