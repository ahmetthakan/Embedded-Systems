/*******************************************************************************
 * @Function_Name   : EEPROM_Write_Buffer_Push
 * @Function_Input  : EEPROM memory address, EEPROM memory block number,
 * 					  given data, byte size of data (Max 127).
 * @Function_Output : None.
 * @Function_Brief  : Add the given value to EEPROM_Write_Buffer for transmission.
 ******************************************************************************/
void EEPROM_Write_Buffer_Push(uint16_t Address, EEPROM_Block_Typedef Block,
							  uint8_t * Data, uint8_t Byte_Size){

	uint8_t EEPROM_Write_Buffer_Overflow_Flag = 0;	// Stack overflow flag.

	EEPROM_Control_Byte_Typedef EEPROM_Control_Byte;

	uint16_t Counter = 0;

	// Array for combining the message
	uint8_t Message_Combiner[EEPROM_CONTROL_BYTE_SIZE + Byte_Size];

	// Use Buffer_Fill_Pointer to convert Control_Byte to uint8_t array.
	uint8_t* EEPROM_Buffer_Fill_Pointer = (uint8_t *)&EEPROM_Control_Byte;

	// Assign values to Control_Byte's variables.
	EEPROM_Control_Byte.Start_Byte = EEPROM_BUFFER_START_BYTE;
	EEPROM_Control_Byte.Variable_Size = Byte_Size;
	EEPROM_Control_Byte.Block_Num = Block;
	EEPROM_Control_Byte.Address = Address;

	// Add given data to Message_Combiner;
	for(Counter = EEPROM_CONTROL_BYTE_SIZE ; Counter < EEPROM_CONTROL_BYTE_SIZE + Byte_Size ; Counter++){
		Message_Combiner[Counter] = *Data;
		Data++;
	}

	// Fill message combiner array with Control_Byte values.
	for(Counter = 0; Counter < EEPROM_CONTROL_BYTE_SIZE ; Counter++){
		Message_Combiner[Counter] = *EEPROM_Buffer_Fill_Pointer;
		EEPROM_Buffer_Fill_Pointer++;
	}

	// Use Buffer_Fill_Pointer to access Message_Combiner array values.
	EEPROM_Buffer_Fill_Pointer = Message_Combiner;

	uint8_t Total_Size = sizeof(Message_Combiner);

	// Tail is ahead of Head and there is no more room at the buffers end.
	if((EEPROM_Write_Buffer_Values.Tail > EEPROM_Write_Buffer_Values.Head)
			&& ((EEPROM_WRITE_BUFFER_SIZE - 1 - EEPROM_Write_Buffer_Values.Tail)
					+ EEPROM_Write_Buffer_Values.Head > (Total_Size + 1))){
		for(Counter = EEPROM_Write_Buffer_Values.Tail; Counter
			< EEPROM_Write_Buffer_Values.Tail + Total_Size; Counter++){
			// Counter is bigger than buffer size.
			if(Counter > (EEPROM_WRITE_BUFFER_SIZE - 1)){
				EEPROM_Write_Buffer[Counter - EEPROM_WRITE_BUFFER_SIZE] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
			else{
				EEPROM_Write_Buffer[Counter] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
		}
	}
	// Tail is behind of Head.
	else if((EEPROM_Write_Buffer_Values.Tail < EEPROM_Write_Buffer_Values.Head)
			&& ((EEPROM_Write_Buffer_Values.Head - EEPROM_Write_Buffer_Values.Tail)
					> (Total_Size + 1))){
		for(Counter = EEPROM_Write_Buffer_Values.Tail; Counter
			< EEPROM_Write_Buffer_Values.Tail + Total_Size; Counter++){
			// Counter is bigger than buffer size.
			if(Counter > (EEPROM_WRITE_BUFFER_SIZE - 1)){
				EEPROM_Write_Buffer[Counter - EEPROM_WRITE_BUFFER_SIZE] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
			else{
				EEPROM_Write_Buffer[Counter] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
		}
	}
	// Tail and head are at the same place.
	else if(EEPROM_Write_Buffer_Values.Head == EEPROM_Write_Buffer_Values.Tail){
		for(Counter = EEPROM_Write_Buffer_Values.Head; Counter
			< EEPROM_Write_Buffer_Values.Tail + Total_Size; Counter++){
			// Counter is bigger than buffer size.
			if(Counter > (EEPROM_WRITE_BUFFER_SIZE - 1)){
				EEPROM_Write_Buffer[Counter - EEPROM_WRITE_BUFFER_SIZE] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
			else{
				EEPROM_Write_Buffer[Counter] = *EEPROM_Buffer_Fill_Pointer;
				EEPROM_Buffer_Fill_Pointer++;
			}
		}
	}
	else{
		// TODO: Add stack overflow counter.
		EEPROM_Write_Buffer_Overflow_Flag = 1;
	}

	// If stack overflow occurs don't move tail to its new location.
	if(!EEPROM_Write_Buffer_Overflow_Flag){
		if((EEPROM_Write_Buffer_Values.Tail + Total_Size) > (EEPROM_WRITE_BUFFER_SIZE - 1)){
			EEPROM_Write_Buffer_Values.Tail = Counter - EEPROM_WRITE_BUFFER_SIZE;
		}
		else{
			EEPROM_Write_Buffer_Values.Tail += Total_Size;
		}

		// Raise EEPROM_Buffer_Flag.
		EEPROM_Buffer_Flag = EEPROM_Buffer_NotEmpty;
	}
}

/*******************************************************************************
 * @Function_Name   : EEPROM_Write_Buffer_Pull
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Pull data from EEPROM_Write_Buffer and transmit it.
 ******************************************************************************/
void EEPROM_Write_Buffer_Pull(){
	uint8_t Write_Buffer_Searching_Flag = 1;
	// Loop this while searching.
	do{
		if(EEPROM_Write_Buffer_Values.Head == EEPROM_Write_Buffer_Values.Tail){
			// Do nothing and stop search.
			Write_Buffer_Searching_Flag = 0;
			// Lower EEPROM_Buffer_Flag to mention buffer is empty.
			EEPROM_Buffer_Flag = EEPROM_Buffer_Empty;
		}
		// If EEPROM_BUFFER_START_BYTE found.
		else if(EEPROM_Write_Buffer[EEPROM_Write_Buffer_Values.Head] == EEPROM_BUFFER_START_BYTE){
			uint8_t Temp_Control_Byte_Array[4];
			EEPROM_Control_Byte_Typedef *Temp_Control_Byte;
			uint16_t Temp_Head = EEPROM_Write_Buffer_Values.Head;
			uint8_t For_Counter = 0;

			// Read Control_Byte from buffer and fill it to a temporary array.
			for(For_Counter = 0; For_Counter < EEPROM_CONTROL_BYTE_SIZE; For_Counter++){
				if(Temp_Head + For_Counter > EEPROM_WRITE_BUFFER_SIZE - 1){
					Temp_Control_Byte_Array[For_Counter] = EEPROM_Write_Buffer[Temp_Head + For_Counter - EEPROM_WRITE_BUFFER_SIZE];
				}
				else{
					Temp_Control_Byte_Array[For_Counter] = EEPROM_Write_Buffer[Temp_Head + For_Counter];
				}
			}

			// Move temp_head to data's start byte.
			if(Temp_Head + For_Counter > EEPROM_WRITE_BUFFER_SIZE - 1){
				Temp_Head = Temp_Head + (For_Counter - EEPROM_WRITE_BUFFER_SIZE);
			}
			else{
				Temp_Head += For_Counter;
			}

			// Parse Temp_Control_Byte_Array with Temporary_Control_Byte struct.
			Temp_Control_Byte = (EEPROM_Control_Byte_Typedef*)Temp_Control_Byte_Array;

			uint8_t Temp_Data_Array[Temp_Control_Byte->Variable_Size];
			// Pull data to Temp_Data_Array and transmit it.
			for(For_Counter = 0; For_Counter < Temp_Control_Byte->Variable_Size; For_Counter++){
				if(Temp_Head + For_Counter > EEPROM_WRITE_BUFFER_SIZE - 1){
					Temp_Data_Array[For_Counter] = EEPROM_Write_Buffer[Temp_Head + For_Counter - EEPROM_WRITE_BUFFER_SIZE];
				}
				else{
					Temp_Data_Array[For_Counter] = EEPROM_Write_Buffer[Temp_Head + For_Counter];
				}
			}

			EEPROM_Multiple_INT8_Write(Temp_Control_Byte->Address, Temp_Control_Byte->Block_Num, Temp_Data_Array, Temp_Control_Byte->Variable_Size);

			// Reposition Head. Use For_Counter as the size indicator.
			if(EEPROM_Write_Buffer_Values.Head + EEPROM_CONTROL_BYTE_SIZE + For_Counter > EEPROM_WRITE_BUFFER_SIZE - 1){
				EEPROM_Write_Buffer_Values.Head = EEPROM_Write_Buffer_Values.Head + (EEPROM_CONTROL_BYTE_SIZE + For_Counter - EEPROM_WRITE_BUFFER_SIZE);
			}
			else{
				EEPROM_Write_Buffer_Values.Head += EEPROM_CONTROL_BYTE_SIZE + For_Counter;
			}

			// Lower EEPROM_Buffer_Flag to mention buffer is empty.
			if(EEPROM_Write_Buffer_Values.Head == EEPROM_Write_Buffer_Values.Tail){
				EEPROM_Buffer_Flag = EEPROM_Buffer_Empty;
			}

			// Stop search.
			Write_Buffer_Searching_Flag = 0;
		}
		else{
			// Increment Head.
			if(EEPROM_Write_Buffer_Values.Head == EEPROM_WRITE_BUFFER_SIZE - 1){
				EEPROM_Write_Buffer_Values.Head = 0;
			}
			else{
				EEPROM_Write_Buffer_Values.Head++;
			}
		}

	}while(Write_Buffer_Searching_Flag);
}