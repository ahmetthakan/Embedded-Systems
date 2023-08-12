
/* XMODEM CRC-16 */

/*******************************************************************************
 * @Function_Name	: CRC16_XModem_Calculator
 * @Function_Input	: char *, int
 * @Function_Output	: int
 * @Function_Brief	: This function calculates crc value according to
 * crc16_XModem algorithm.
 ******************************************************************************/

int CRC16_XModem_Calculator(char *dataAddress, int dataLength)
{
    /* Definite polynomial value */
    int poly = 0x1021;

    int i;
    int crc = 0;

    for (; dataLength>0; dataLength--)
    {
        /* Fetch byte from memory, XOR into CRC top byte*/
        crc = crc ^ (*dataAddress << 8);
        /* Iterate for each bir in the data */
        for (i=0; i<8; i++)
        {
            /* If MSB is '1' xor with poly */
            crc = crc << 1;
            if (crc & 0x10000){
                crc = (crc ^ poly) & 0xFFFF;
            }
        }
        dataAddress++;
    }
    return(crc);
}
