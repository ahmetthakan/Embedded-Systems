/*******************************************************************************
 * @Function_Name   : Ophir_Command_Stack_Push
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Push command to stack.
 ******************************************************************************/
void Ophir_Command_Stack_Push(){

	// TODO: Add fault counter.

	// Stack only works with unsigned values. Converts singed values to unsigned values.
	uint8_t* Command_Pointer = (uint8_t*)&U1_C;
	uint16_t Stack_Counter = 0;
	uint8_t Stack_Overflow = 1;

	// Tail is ahead of Head and there is no more room at the buffers end.
	if((Ophir_Command_Stack_Values.Tail > Ophir_Command_Stack_Values.Head)
			&& ((OPHIR_COMMAND_STACK_SIZE - 1 - Ophir_Command_Stack_Values.Tail)
					+ Ophir_Command_Stack_Values.Head > (U1_COMMAND_SIZE + 1))){
		for (Stack_Counter = Ophir_Command_Stack_Values.Tail;
				Stack_Counter < Ophir_Command_Stack_Values.Tail + U1_COMMAND_SIZE; Stack_Counter++) {
			// Counter is bigger than buffer size.
			if(Stack_Counter > (OPHIR_COMMAND_STACK_SIZE - 1)){
				Ophir_Command_Stack[Stack_Counter - OPHIR_COMMAND_STACK_SIZE] = *Command_Pointer;
				Command_Pointer++;
			}
			else{
				Ophir_Command_Stack[Stack_Counter] = *Command_Pointer;
				Command_Pointer++;
			}
		}
	}
	// Tail is behind the Head.
	else if((Ophir_Command_Stack_Values.Tail < Ophir_Command_Stack_Values.Head)
			&& ((Ophir_Command_Stack_Values.Head - Ophir_Command_Stack_Values.Tail)
					> (U1_COMMAND_SIZE + 1))){
		for (Stack_Counter = Ophir_Command_Stack_Values.Tail; Stack_Counter
			< Ophir_Command_Stack_Values.Tail + U1_COMMAND_SIZE; Stack_Counter++) {
			// Conter is bigger than buffer size.
			if(Stack_Counter > (OPHIR_COMMAND_STACK_SIZE - 1)){
				Ophir_Command_Stack[Stack_Counter - OPHIR_COMMAND_STACK_SIZE] = *Command_Pointer;
				Command_Pointer++;
			}
			else{
				Ophir_Command_Stack[Stack_Counter] = *Command_Pointer;
				Command_Pointer++;
			}
		}
	}
	// Tail and head are at the same place.
	else if(Ophir_Command_Stack_Values.Head == Ophir_Command_Stack_Values.Tail){
		for (Stack_Counter = Ophir_Command_Stack_Values.Head; Stack_Counter
			< Ophir_Command_Stack_Values.Tail + U1_COMMAND_SIZE; Stack_Counter++) {
			// Conter is bigger than buffer size.
			if(Stack_Counter > (OPHIR_COMMAND_STACK_SIZE - 1)){
				Ophir_Command_Stack[Stack_Counter - OPHIR_COMMAND_STACK_SIZE] = *Command_Pointer;
				Command_Pointer++;
			}
			else{
				Ophir_Command_Stack[Stack_Counter] = *Command_Pointer;
				Command_Pointer++;
			}
		}
	}
	else{
		// TODO: Stack over flow error.
		Stack_Overflow = 0;
	}

	if(Stack_Overflow){
		// Set the tails new position.
		if((Ophir_Command_Stack_Values.Tail + U1_COMMAND_SIZE) > (OPHIR_COMMAND_STACK_SIZE - 1)){
			Ophir_Command_Stack_Values.Tail = Stack_Counter - OPHIR_COMMAND_STACK_SIZE;
		}
		else{
			Ophir_Command_Stack_Values.Tail += U1_COMMAND_SIZE;
		}
	}

	// Reset U1_C.
	Ophir_U1_Command_Reset();
}
/*******************************************************************************
 * @Function_Name   : Ophir_Command_Stack_Pull
 * @Function_Input  : None.
 * @Function_Output : None.
 * @Function_Brief  : Pull command from stack.
 ******************************************************************************/
void Ophir_Command_Stack_Pull(){

	uint8_t temp_U1_C[12];
	uint8_t for_counter = 0;

	// If head and tail are at the same place.
	if(Ophir_Command_Stack_Values.Head == Ophir_Command_Stack_Values.Tail){
		// Do nothing
	}
	// If you find header1;
	else if(Ophir_Command_Stack[Ophir_Command_Stack_Values.Head] == OPHIR_COMMAND_START_BYTE){
		// If you find header2;
		if(Ophir_Command_Stack[Ophir_Command_Stack_Values.Head + 1] == OPHIR_COMMAND_START_BYTE){
			for(for_counter = 0; for_counter < U1_COMMAND_SIZE ; for_counter++){
				if(Ophir_Command_Stack_Values.Head + for_counter > OPHIR_COMMAND_STACK_SIZE - 1){
					temp_U1_C[for_counter] = Ophir_Command_Stack[Ophir_Command_Stack_Values.Head + for_counter - OPHIR_COMMAND_STACK_SIZE];
				}else{
					temp_U1_C[for_counter] = Ophir_Command_Stack[Ophir_Command_Stack_Values.Head + for_counter];
				}
			}
			Ophir_U1_Command_Send(temp_U1_C);

			// After calculations move head to its new location.
			if((Ophir_Command_Stack_Values.Head + U1_COMMAND_SIZE) > (OPHIR_COMMAND_STACK_SIZE - 1)){
				Ophir_Command_Stack_Values.Head += for_counter + 1;
				Ophir_Command_Stack_Values.Head -= OPHIR_COMMAND_STACK_SIZE;
			}
			else{
				Ophir_Command_Stack_Values.Head += U1_COMMAND_SIZE;
			}
		}
	}
}
