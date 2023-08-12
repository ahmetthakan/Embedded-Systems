/*******************************************************************************
 * @Function_Name   : YZB_Buffer_Push
 * @Function_Input  : Data pointer.
 * @Function_Output : None.
 * @Function_Brief  : Fill Buffer with wanted value of given attribute.
 ******************************************************************************/
void YZB_Buffer_Push(uint8_t *Buffer_Pointer){

	uint16_t Counter;
	// Stack overflow flag
	uint8_t Stack_Overflow = 0;

	// Tail is ahead of Head and there is no more room at the buffers end.
	if (YZB_Buffer_Values.Tail > YZB_Buffer_Values.Head
			&& (YZB_COMMAND_BUFFER_SIZE - 1 - YZB_Buffer_Values.Tail)
					+ YZB_Buffer_Values.Head > (YZB_COMMAND_CONTROL_BYTE_SIZE + 1)) { // ex. head:1010, tail:1020, size:10

		for (Counter = YZB_Buffer_Values.Tail;
				Counter < YZB_Buffer_Values.Tail + YZB_COMMAND_CONTROL_BYTE_SIZE; Counter++) {
			// Counter is bigger than buffer size.
			if (Counter > YZB_COMMAND_BUFFER_SIZE - 1) {
				YZB_Command_Buffer[Counter - YZB_COMMAND_BUFFER_SIZE] =
						*Buffer_Pointer;
				Buffer_Pointer++;

			} else {
				YZB_Command_Buffer[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
	}
	// Tail is behind the Head.
	else if (YZB_Buffer_Values.Head > YZB_Buffer_Values.Tail
			&& (YZB_Buffer_Values.Head - YZB_Buffer_Values.Tail) > (YZB_COMMAND_CONTROL_BYTE_SIZE + 1)) { // ex. head:1010 tail:20 size:10
		// Conter is bigger than buffer size.
		for (Counter = YZB_Buffer_Values.Tail; Counter < YZB_Buffer_Values.Tail + YZB_COMMAND_CONTROL_BYTE_SIZE; Counter++) {
			if (Counter > YZB_COMMAND_BUFFER_SIZE - 1) {
				YZB_Command_Buffer[Counter - YZB_COMMAND_BUFFER_SIZE] =
						*Buffer_Pointer;
				Buffer_Pointer++;
			} else {
				YZB_Command_Buffer[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
	}
	// Tail and head are at the same place.
	else if(YZB_Buffer_Values.Head == YZB_Buffer_Values.Tail){
		for (Counter = YZB_Buffer_Values.Head; Counter < YZB_Buffer_Values.Tail + YZB_COMMAND_CONTROL_BYTE_SIZE; Counter++) {
			// Counter is bigger than buffer size.
			if (Counter > YZB_COMMAND_BUFFER_SIZE - 1) {
				YZB_Command_Buffer[Counter - YZB_COMMAND_BUFFER_SIZE] =
						*Buffer_Pointer;
				Buffer_Pointer++;
			} else {
				YZB_Command_Buffer[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
	}else {
		// Stack overflow
		// TODO: Add missed messages counter.
		Stack_Overflow = 1;
	}

	if(!Stack_Overflow){
		// Set the tails new position.
		if(YZB_Buffer_Values.Tail + YZB_COMMAND_CONTROL_BYTE_SIZE > YZB_COMMAND_BUFFER_SIZE - 1){
			YZB_Buffer_Values.Tail = Counter - YZB_COMMAND_BUFFER_SIZE;
		}
		else{
			YZB_Buffer_Values.Tail += YZB_COMMAND_CONTROL_BYTE_SIZE;
		}
		// Raise YZB_Buffer_Flag.
		YZB_Command_Buffer_Flag = YZB_Command_Buffer_NotEmpty;
	}
}

/*******************************************************************************
 * @Function_Name   : YZB_Buffer_Pull
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Pull button sequence from Buffer. NO END CHECK AND NO MOVE
 ******************************************************************************/
void YZB_Buffer_Pull(){

	uint8_t YZB_Buffer_Searching_Flag = 1;

	do{
		// If head and tail are at the same place.
		if(YZB_Buffer_Values.Head == YZB_Buffer_Values.Tail){
			// Do nothing and stop search.
			YZB_Buffer_Searching_Flag = 0;
			// Lower EEPROM_Buffer_Flag to mention buffer is empty.
			YZB_Command_Buffer_Flag = YZB_Command_Buffer_Empty;
		}
		// If you find header.
		else if(YZB_Command_Buffer[YZB_Buffer_Values.Head] == YZB_COMMAND_BUFFER_START_BYTE){
			int8_t New_Value = 0;
			YZB_Attributes_Typedef YZB_Attribute = 0;

			// Pull which attribute will be changed.
			if(YZB_Buffer_Values.Head + 1 > YZB_COMMAND_BUFFER_SIZE - 1){
				YZB_Attribute = YZB_Command_Buffer[YZB_Buffer_Values.Head + 1 - YZB_COMMAND_BUFFER_SIZE];
			}else{
				YZB_Attribute = YZB_Command_Buffer[YZB_Buffer_Values.Head + 1];
			}

			// Pull the new value.
			if(YZB_Buffer_Values.Head + 2 > YZB_COMMAND_BUFFER_SIZE - 1){
				New_Value = YZB_Command_Buffer[YZB_Buffer_Values.Head + 2 - YZB_COMMAND_BUFFER_SIZE];
			}else{
				New_Value = YZB_Command_Buffer[YZB_Buffer_Values.Head + 2];
			}

			// Parser for changing the value with button press sequence.
			YZB_Buffer_Parser(YZB_Attribute, New_Value);

			// Set new header value.
			if(YZB_Buffer_Values.Head + YZB_COMMAND_CONTROL_BYTE_SIZE > YZB_COMMAND_BUFFER_SIZE - 1){
				YZB_Buffer_Values.Head = YZB_Buffer_Values.Head - YZB_COMMAND_BUFFER_SIZE + YZB_COMMAND_CONTROL_BYTE_SIZE;
			}
			else{
				YZB_Buffer_Values.Head += YZB_COMMAND_CONTROL_BYTE_SIZE;
			}

			// Lower YZB_Buffer_Flag to mention buffer is empty.
			if(YZB_Buffer_Values.Head == YZB_Buffer_Values.Tail){
				YZB_Command_Buffer_Flag = YZB_Command_Buffer_Empty;
			}

			// Stop search.
			YZB_Buffer_Searching_Flag = 0;
		}
		else{
			// Increment Head.
			if(YZB_Buffer_Values.Head == YZB_COMMAND_BUFFER_SIZE - 1){
				YZB_Buffer_Values.Head = 0;
			}
			else{
				YZB_Buffer_Values.Head++;
			}
		}
	}while(YZB_Buffer_Searching_Flag);
}