/***************************
 * @Function_Name   : Push_To_UART_Receive_Stack
 * @Function_Input  : Buffer_Pointer, Buffer Size
 * @Function_Output : None.
 * @Function_Brief  : This function using for UART COM init.    PUSH
 **************************/
void Push_To_UART_Transmit_Stack(uint8_t *Buffer_Pointer, uint8_t Size) {
	uint16_t Counter;
	if (Transmit_Buffer_Tail > Transmit_Buffer_Head
			&& (TCP_Transmit_Queue_Size - 1 - Transmit_Buffer_Tail)
					+ Transmit_Buffer_Head > (Size + 1)) { // ex. head:1010, tail:1020, size:10

		for (Counter = Transmit_Buffer_Tail; Counter < Transmit_Buffer_Tail + Size; Counter++) {
			if (Counter > TCP_Transmit_Queue_Size - 1) {
				Transmit_Stack[Counter - TCP_Transmit_Queue_Size] =
						*Buffer_Pointer;
				Buffer_Pointer++;

			} else {
				Transmit_Stack[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
	}
	else if (Transmit_Buffer_Head > Transmit_Buffer_Tail
			&& (Transmit_Buffer_Head - Transmit_Buffer_Tail) > (Size + 1)) { // ex. head:1010 tail:20 size:10
		for (Counter = Transmit_Buffer_Tail; Counter < Transmit_Buffer_Tail + Size; Counter++) {
			if (Counter > TCP_Transmit_Queue_Size - 1) {
				Transmit_Stack[Counter - TCP_Transmit_Queue_Size] =
						*Buffer_Pointer;
				Buffer_Pointer++;
			} else {
				Transmit_Stack[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
	}
	else if(Transmit_Buffer_Head == Transmit_Buffer_Tail){
		for (Counter = Transmit_Buffer_Head; Counter < Transmit_Buffer_Tail + Size; Counter++) {
			if (Counter > TCP_Transmit_Queue_Size - 1) {
				Transmit_Stack[Counter - TCP_Transmit_Queue_Size] =
						*Buffer_Pointer;
				Buffer_Pointer++;
			} else {
				Transmit_Stack[Counter] = *Buffer_Pointer;
				Buffer_Pointer++;
			}
		}
		TCP_Transmit_Stack_Status = Not_Empty;
	}else {
		#ifdef TCP_UART_Active
		    	Diagnostic_Error_Bit_Set(TCP_Uart_Transmit_Overflow_Error, Error_Enable);
		#endif
		#ifdef RS422_UART_Active
		    	Diagnostic_Error_Bit_Set(TCP_Uart_Transmit_Overflow_Error, Error_Enable);
		#endif
	}
	if(Transmit_Buffer_Tail+ Size > TCP_Transmit_Queue_Size - 1){
		Transmit_Buffer_Tail = Counter - TCP_Transmit_Queue_Size;
	}
	else{
		Transmit_Buffer_Tail += Size;
	}
}

/***************************
 * @Function_Name   : Communication_Parser
 * @Function_Input  : Message_Buffer
 * @Function_Output : None.
 * @Function_Brief  : This function using for set IO Status     PULL
 **************************/
void Pull_From_Transmit_Stack(void){
	uint8_t Length = 0;
	uint8_t Array_Size = 0;
	uint8_t i;
	if(Transmit_Stack[Transmit_Buffer_Head] == START_BYTE){
		if((Transmit_Buffer_Head + 4) > TCP_Transmit_Queue_Size - 1){
			Length = Transmit_Stack[Transmit_Buffer_Head + 4 - TCP_Transmit_Queue_Size];
		}
		else{
			Length = Transmit_Stack[Transmit_Buffer_Head + 4];
		}

		Array_Size = Length + 8;
		uint8_t Temp_Message_Array[Array_Size];
		for(i = 0 ; i <  Array_Size ; i++){
			if(Transmit_Buffer_Head + i > TCP_Transmit_Queue_Size - 1){
				Temp_Message_Array[i] = Transmit_Stack[Transmit_Buffer_Head - TCP_Transmit_Queue_Size + i];
			}
			else{
				Temp_Message_Array[i] = Transmit_Stack[Transmit_Buffer_Head + i];
			}
		}
		if(Transmit_Buffer_Head + Array_Size > TCP_Transmit_Queue_Size - 1){
			Transmit_Buffer_Head = Transmit_Buffer_Head - TCP_Transmit_Queue_Size + Array_Size;
		}
		else{
			Transmit_Buffer_Head += Array_Size;
		}
		if(Transmit_Buffer_Head == Transmit_Buffer_Tail){
			TCP_Transmit_Stack_Status = Empty;
		}
#ifdef TCP_UART_Active
		HAL_UART_Transmit(TCP_UART, Temp_Message_Array, Array_Size, 100);
#endif
#ifdef RS422_UART_Active
		HAL_UART_Transmit(RS422_UART, Temp_Message_Array, Array_Size, 100);
#endif
		HAL_Delay(10);
		// TODO: ifdef açık porta göre
	}
}

/****************************************Single Byte*******************************************/

/***************************
 * @Function_Name   : Push_To_UART_Receive_Stack
 * @Function_Input  : Buffer_Pointer, Buffer Size
 * @Function_Output : None.
 * @Function_Brief  : This function using for UART COM init.    PUSH
 **************************/
void Push_To_UART_Receive_Stack(uint8_t *Buffer_Pointer) {
	if (Receive_Buffer_Tail > Receive_Buffer_Head && (TCP_Receive_Queue_Size - 1 - Receive_Buffer_Tail + Receive_Buffer_Head) > 2) {
		if (Receive_Buffer_Tail == TCP_Receive_Queue_Size ) {
			Receive_Buffer_Tail = 0;
		}
		Receive_Stack[Receive_Buffer_Tail] = *Buffer_Pointer;
		Receive_Buffer_Tail ++;
	}
	else if (Receive_Buffer_Head > Receive_Buffer_Tail && (Receive_Buffer_Head - Receive_Buffer_Tail) > 2) {
			Receive_Stack[Receive_Buffer_Tail] = *Buffer_Pointer;
			Receive_Buffer_Tail ++;
		}
	else if(Receive_Buffer_Head == Receive_Buffer_Tail){
			Receive_Stack[Receive_Buffer_Tail] = *Buffer_Pointer;
			Receive_Buffer_Tail ++;
			TCP_Receive_Stack_Status = Not_Empty;
	}else {
		#ifdef TCP_UART_Active
		    	Diagnostic_Error_Bit_Set(TCP_Uart_Receive_Overflow_Error, Error_Enable);
		#endif
		#ifdef RS422_UART_Active
		    	Diagnostic_Error_Bit_Set(TCP_Uart_Receive_Overflow_Error, Error_Enable);
		#endif
	}
}

/***************************
 * @Function_Name   : Communication_Parser
 * @Function_Input  : Message_Buffer
 * @Function_Output : None.
 * @Function_Brief  : This function using for set IO Status     PULL
 **************************/
void Pull_From_Receive_Stack(void) {
	if (Receive_Buffer_Head == TCP_Receive_Queue_Size) {
		Receive_Buffer_Head = 0;
	}
	if (Receive_Buffer_Head == Receive_Buffer_Tail) {
		TCP_Receive_Stack_Status = Empty;
	} else {
#ifdef RS422_UART_Active
		Add_RS422_Ring(Receive_Stack[Receive_Buffer_Head]);
#endif

#ifdef TCP_UART_Active
		Add_TCP_Ring(Receive_Stack[Receive_Buffer_Head]);
#endif
		Receive_Buffer_Head++;
	}
}
