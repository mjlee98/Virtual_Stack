#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define BYTE unsigned char
#include <stdint.h>

typedef struct Operations{
  BYTE opcode;
  uint8_t address_type1;
  uint8_t address1;
  uint8_t address_type2;
  uint8_t address2;
  struct Operations* next;
}Operations_t;

Operations_t *addToFront(Operations_t *first_function, Operations_t *new){
	new->next = first_function;
	//printf("\n %d %d ",new->opcode, new->next->opcode);
    return new;
}

typedef struct function{
  uint8_t label;
  uint8_t numarg;
  struct Operations* head;
  struct function* next;
}function_t;

function_t* function_addToFront(function_t *head, function_t *new){
	new->next = head;
	//printf("\n %d %d ",new->opcode, new->next->opcode);
	return new;
}

typedef struct frame{
	uint8_t stack[128];
}frame_t;

uint8_t get_bit(int index);
uint8_t get_numInst(int last_index);
uint8_t get_opcode(int last_index);
uint8_t get_addresstype(int last_index);
uint8_t get_address(int last_index,uint8_t address_type);
uint8_t get_label_or_numarg(int last_index);
Operations_t* read(int last_index);
uint8_t runCode(function_t* function, function_t* first_function,uint8_t* args, int argsize);
uint8_t function_count = 0;
int bits_back = 0;
uint8_t registers[8];
BYTE *arr;





/*
 * main
 * Scaffold code for the COMP2017/9017 assignment 1
 * This code will take a filename and pruint8_t the numeric value associated
 * with each byte in the file
 * :: uint8_t n_args :: The number of arguments passed to the file
 * :: char** vargs :: Stores the arguments passed to this program as cstrings
 * Returns 0
 */
int main(int n_args, char** vargs)
{

    // For readability
    char* filename = vargs[1];
    // A pro-tip to prevent seg-faults for yourself might be to check
    // if this file actually exists, and if an argument is actually passed...

    // Create a 1 byte buffer. You might want more than one byte yourself
    BYTE buffer;

    // Open our file in 'read binary' mode,
    // I sure do hope that this file actually exists
    FILE *file_ptr = fopen(filename,"rb");

    // Skip to the end of the file
    fseek(file_ptr, 0, SEEK_END);

    // Find the length of the file
    size_t file_length = ftell(file_ptr);

    // Rewind to the start of the file so we can actually read it
    rewind(file_ptr);

	arr = malloc(sizeof(BYTE)*file_length);

    // For each byte in the file, first write the byte to our buffer
    // then pruint8_t the integer representation of the byte
    for (uint8_t i = 0; i < file_length; i++)
    {
        fread(&buffer, 1, 1, file_ptr); // read 1 byte
        //printf("%d ", (uint8_t)buffer);
		arr[i] = buffer;
    }
	int last_index = file_length*8-1;
	function_t* first_function = (function_t*) malloc (sizeof(function_t));
	first_function->next = NULL;
	Operations_t* head = read(last_index);
	first_function->head = head;
    //printf("%d %d %d %d %d %d\n", first_function->head->opcode,first_function->head->next->opcode, first_function->head->next->next->opcode, first_function->head->next->next->next->opcode,first_function->head->next->next->next->next->opcode,first_function->head->next->next->next->next->next->opcode);
	first_function->numarg = get_label_or_numarg(last_index-bits_back);
	bits_back += 4;
	first_function->label = get_label_or_numarg(last_index-bits_back);
	bits_back += 4;
	 while(last_index - bits_back > 7){
	 	function_t* function = (function_t*) malloc (sizeof(function_t));
	 	function->head = read(last_index);
	 // 	printf("%d %d %d %d %d %d\n", first_function->head->opcode,first_function->head->next->opcode, first_function->head->next->next->opcode, first_function->head->next->next->next->opcode,first_function->head->next->next->next->next->opcode,first_function->head->next->next->next->next->next->opcode);
		 function->numarg = get_label_or_numarg(last_index-bits_back);
	 	bits_back += 4;
	 	function->label = get_label_or_numarg(last_index-bits_back);
	 	bits_back += 4;
	 	first_function = function_addToFront(first_function,function);
	 }

	 free(arr);


	 function_t *cpy3 = first_function;
	 if(function_count > 1){
		 function_t *u;
		 function_t *cpy1 = first_function;
		 function_t *cpy2 = first_function;
		 for( ; cpy1 != NULL; cpy1=u) {
			u = cpy1->next;
		 	if(cpy1->label==0){
				uint8_t answer = runCode(cpy1,cpy3,NULL,0);
				if(answer != 153){
					printf("%d\n",answer);
				}
		 	}
		 }
		 
		// Set iterator iter to loop over all the elements of the first list
		for (function_t* iter = cpy2; iter != NULL;)
		{
			// Set a second iterator using the list inside iterator
			for (Operations_t* jter = iter->head; jter != NULL;)
			{
				// Create a temporary pouint8_ter to the current element and 
				// increment the iterator
				Operations_t* tmp_jter = jter;
				jter = jter->next;
				// Free the address pouint8_ted to by the temporary variable
				free(tmp_jter);
			}
			// Create a temporary pouint8_ter to the current element and 
			// increment the iterator
			function_t* tmp_iter = iter;
			iter = iter->next;
			// Free the address pouint8_ted to by the temporary variable
			free(tmp_iter);
		}
	 }
	if(function_count == 1){
		uint8_t answer = runCode(first_function,cpy3,NULL,0);
		if(answer != 100){
			printf("%d\n",answer);
		}
		Operations_t *p;
			for ( ; head != NULL; head=p) {
				p = head->next;
				free(head);
			}
			free(first_function);
	}
	
    return 0;
}

uint8_t get_bit(int index){
	//printf("%d\n",index);
	int arr_index = index/8;
	BYTE num = arr[arr_index];
	int shift = 7 - index%8;
	int bit = (num >> shift) & 1;
	return bit;
}

uint8_t get_numInst(int last_index){
	uint8_t numInst = 0;
	for(int i = 0; i < 8; i++){
 		numInst +=  get_bit(last_index-i)*pow(2,i);
	}
	return numInst;
}

uint8_t get_opcode(int last_index){
	uint8_t opcode = 0;
	for(int i = 0; i < 3; i++){
 		opcode +=  get_bit(last_index-i)*pow(2,i);
	}
	return opcode;
}

uint8_t get_addresstype(int last_index){
	uint8_t addresstype = 0;
	for(int i = 0; i < 2; i++){
 		addresstype +=  get_bit(last_index-i)*pow(2,i);
	}
	return addresstype;
}

uint8_t get_address(int last_index, uint8_t address_type){
  uint8_t address = 0;
  if(address_type == 0){
    for(int i = 0; i < 8; i++){
      address +=  get_bit(last_index-i)*pow(2,i);
    }
  }
  else if(address_type == 1){
    for(int i = 0; i < 3; i++){
      address +=  get_bit(last_index-i)*pow(2,i);
    }
  }
  else if(address_type == 2){
    for(int i = 0; i < 7; i++){
      address +=  get_bit(last_index-i)*pow(2,i);
    }
  }
  else if(address_type == 3){
    for(uint8_t i = 0; i < 7; i++){
      address +=  get_bit(last_index-i)*pow(2,i);
    }
  }
	return address;
}

uint8_t get_label_or_numarg(int last_index){
	uint8_t label_or_numarg = 0;
	for(int i = 0; i < 4; i++){
 		label_or_numarg +=  get_bit(last_index-i)*pow(2,i);
	}
	return label_or_numarg;
}

Operations_t* read(int last_index){
	uint8_t first_numInst = get_numInst(last_index - bits_back);
	bits_back += 8;
	//printf("\nnumber of instructions: %d\n", first_numInst);
	Operations_t* head = (Operations_t*) malloc (sizeof(Operations_t));
  for(uint8_t i = 0; i < first_numInst; i++){
    uint8_t opcode = get_opcode(last_index - bits_back);
    bits_back += 3;
    //printf("opcode: %d ", opcode);
    switch(opcode){
      case 0:
      { 
        //MOV 4args
        uint8_t address_type2 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address2 = get_address(last_index - bits_back,address_type2);
        if(address_type2 == 0){
          //printf("destination is a value, error");
        }
		//register addres - 3bits
        else if(address_type2 == 1){
          bits_back += 3;
        }
		//stack address - 7bits
        else if(address_type2 == 2){
          bits_back += 7;
        }
		//pointer valued - 7bits
        else if(address_type2 == 3){
          bits_back += 7;
        }
        uint8_t address_type1 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address1 = get_address(last_index - bits_back,address_type1);
        if(address_type1 == 0){
          bits_back += 8;
        }
        else if(address_type1 == 1){
          bits_back += 3;
        }
        else if(address_type1 == 2){
          bits_back += 7;
        }
        else if(address_type1 == 3){
          bits_back += 7;
        }

		    if( i == 0 ){
		     head->opcode = opcode;
             head->address_type1 = address_type1;
             head->address1 = address1;
             head->address_type2 = address_type2;
             head->address2 = address2;
             head->next = NULL;
             //printf("\n%d %d %d %d %d\n", head->opcode,head->address_type1,head->address1,head->address_type2,head->address2);
		    }
      		else{
            	Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
				operationp->opcode = opcode;
				operationp->address_type1 = address_type1;
				operationp->address1 = address1;
            	operationp->address_type2 = address_type2;
            	operationp->address2 = address2;
            	head = addToFront(head,operationp);
				//printf("\n%d %d %d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1,operationp->address_type2,operationp->address2); 
        }
        break;
      }

		case 1:
		{
			//CAL 4args, 1byte 00, stack address 10, 8bits+2bits+7bits+2bits
			uint8_t address_type2 = get_addresstype(last_index - bits_back);
			bits_back += 2;
			uint8_t address2 = get_address(last_index - bits_back,address_type2);
			bits_back += 7;
			uint8_t address_type1 = get_addresstype(last_index - bits_back);
			bits_back += 2;
			uint8_t address1 = get_address(last_index - bits_back,address_type1);
			bits_back += 8;
			if( i == 0 ){
				head->opcode = opcode;
				head->address_type1 = address_type1;
				head->address1 = address1;
				head->address_type2 = address_type2;
				head->address2 = address2;
				head->next = NULL;
				//printf("\n%d %d %d %d %d\n", head->opcode,head->address_type1,head->address1,head->address_type2,head->address2);
				}
			else{
				Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
				operationp->opcode = opcode;
				operationp->address_type1 = address_type1;
				operationp->address1 = address1;
				operationp->address_type2 = address_type2;
				operationp->address2 = address2;
				head = addToFront(head,operationp);
				//printf("\n%d %d %d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1,operationp->address_type2,operationp->address2);
			}
		break;
		}
      case 2:
	  {
        //pop 2args, stack address 10, 7bits+2bits
		uint8_t address_type1 = get_addresstype(last_index - bits_back);
		bits_back += 2;
		uint8_t address1 = get_address(last_index - bits_back , address_type1);
		bits_back += 7;
    if( i == 0 ){
         head->opcode = opcode;
         head->address_type1 = address_type1;
         head->address1 = address1;
         head->next = NULL;
         //printf("\n%d %d %d\n", head->opcode,head->address_type1,head->address1);
    }
    else{
        Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
		operationp->opcode = opcode;
		operationp->address_type1 = address_type1;
		operationp->address1 = address1;
        head = addToFront(head,operationp);
		//printf("\n%d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1);
    }
        break;
	  }
      case 3:
	  {
        //ret no arguments
        if( i == 0 ){
             head->opcode = opcode;
             head->next = NULL;
             //printf("\n%d\n", head->opcode);
        }
        else{
                Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
				operationp->opcode = opcode;
                head = addToFront(head,operationp);

             	//printf("\n%d\n", operationp->opcode);
        }
        break;
	  }
      case 4:
	  {
        //add 4args,reg address 01 reg address 01, 3bits+2bits+3bits+2bits
		uint8_t address_type2 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address2 = get_address(last_index - bits_back,address_type2);
        bits_back += 3;
        uint8_t address_type1 = get_addresstype(last_index - bits_back);
	    bits_back += 2;
        uint8_t address1 = get_address(last_index - bits_back,address_type1);
        bits_back += 3;
        if( i == 0 ){
			 head->opcode = opcode;
             head->address_type1 = address_type1;
             head->address1 = address1;
             head->address_type2 = address_type2;
             head->address2 = address2;
             head->next = NULL;
             //printf("\n%d %d %d %d %d\n", head->opcode,head->address_type1,head->address1,head->address_type2,head->address2);
		    }
        else{
			Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
			operationp->opcode = opcode;
			operationp->address_type1 = address_type1;
			operationp->address1 = address1;
            operationp->address_type2 = address_type2;
            operationp->address2 = address2;
            head = addToFront(head,operationp);

            //printf("\n%d %d %d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1,operationp->address_type2,operationp->address2);
        }
        break;
	  }
      case 5:
      {
        //and 4args,reg address 01 reg address 01, 3bits+2bits+3bits+2bits
        uint8_t address_type2 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address2 = get_address(last_index - bits_back,address_type2);
        bits_back += 3;
        uint8_t address_type1 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address1 = get_address(last_index - bits_back,address_type1);
        bits_back += 3;
        if( i == 0 ){
			 head->opcode = opcode;
             head->address_type1 = address_type1;
             head->address1 = address1;
             head->address_type2 = address_type2;
             head->address2 = address2;
             head->next = NULL;
             //printf("\n%d %d %d %d %d\n", head->opcode,head->address_type1,head->address1,head->address_type2,head->address2);
		    }
        else{
            Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
			operationp->opcode = opcode;
			operationp->address_type1 = address_type1;
			operationp->address1 = address1;
            operationp->address_type2 = address_type2;
            operationp->address2 = address2;
            head = addToFront(head,operationp);
            //printf("\n%d %d %d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1,operationp->address_type2,operationp->address2);
        }
        break;
      }
      case 6:
      {
        //not 2args,reg address 01. 3bits+2bits
        uint8_t address_type1 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address1 = get_address(last_index - bits_back,address_type1);
        bits_back += 3;
        if( i == 0 ){
			 head->opcode = opcode;
             head->address_type1 = address_type1;
             head->address1 = address1;
             head->next = NULL;
             //printf("\n%d %d %d\n", head->opcode,head->address_type1,head->address1);
		    }
        else{
            Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
			operationp->opcode = opcode;
			operationp->address_type1 = address_type1;
			operationp->address1 = address1;
            head = addToFront(head,operationp);
            //printf("\n%d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1);
        }
        break;
      }
      case 7:
      {
        //equ 2args,reg address 01, 3bits+2bits
        uint8_t address_type1 = get_addresstype(last_index - bits_back);
		bits_back += 2;
        uint8_t address1 = get_address(last_index - bits_back,address_type1);
        bits_back += 3;
        if( i == 0 ){
			 head->opcode = opcode;
             head->address_type1 = address_type1;
             head->address1 = address1;
             head->next = NULL;
             //printf("\n%d %d %d\n", head->opcode,head->address_type1,head->address1);
		    }
        else{
            Operations_t* operationp = (Operations_t*)malloc(sizeof(Operations_t));
			operationp->opcode = opcode;
			operationp->address_type1 = address_type1;
			operationp->address1 = address1;
            head = addToFront(head,operationp);
            //printf("\n%d %d %d\n", operationp->opcode,operationp->address_type1,operationp->address1);
        }
      }
    }
  }
  function_count += 1;
  return head;

}
//work between call
uint8_t runCode(function_t* function, function_t* first_function, uint8_t* args, int argsize) {
  function_t* first_function_cpy = first_function;
  bool isValid = true;
  uint8_t result = 153;
  frame_t frame;
  frame.stack[0] = 0; //frame_pointer
  frame.stack[1] = 3; //stack_pointer
  frame.stack[2] = 1; //program_counter
  if(argsize != 0){
	  for(int i = 0 ; i < argsize ; i++){
		frame.stack[3+i] =  args[i];
	  }
  }
  frame.stack[127] = 150;
  while(isValid){
      if(frame.stack[127] != 150){
        printf("Stack Overflow!\n");
		result = 100;
        isValid = false;
		break;
      }
	  function_t* function_cpy = function;
      Operations_t *cpy = function_cpy->head;
      for(uint8_t i = 1; i < frame.stack[2]; i++){
        cpy = cpy->next;
      }
      Operations_t *p;
      //for ( ; cpy != NULL; cpy=p) {
       p = cpy->next;
       switch(cpy->opcode){
         case 0:{
		   uint8_t address_type1 = cpy->address_type1;
		   uint8_t address1 = cpy->address1;
		   uint8_t address_type2 = cpy->address_type2;
		   uint8_t address2 = cpy->address2;
		   //address1 is a value
		   if(address_type1 == 0){
			 if(address_type2 == 1){
			   registers[address2] = address1;
			   //printf("\nmove the value %d to register %d\n",address1,address2);
			 }
			 else if(address_type2 == 2){
			   frame.stack[address2] = address1;
			   frame.stack[1] += 1; //stack_pouint8_ter
			   //printf("\nmove the value %d to stack[%d]\n",address1,address2);
			 }
			 else if(address_type2 == 3){
			   uint8_t indirection = frame.stack[address2];
			   frame.stack[indirection] = address1;
			   frame.stack[1] += 1; //stack_pouint8_ter
			   //printf("\nmove the value %d to stack[%d]\n",address1,frame.stack[address2]);
			 }
		   }
		   else if(address_type1 == 1){
			 if(address_type2 == 1){
			   registers[address2] = registers[address1];
			   printf("\nmove register value %d to register %d\n",registers[address1],address2);
			 }
			 else if(address_type2 == 2){
			   frame.stack[address2] = registers[address1];
			   frame.stack[1] += 1; //stack_pouint8_ter
			 //  printf("\nmove register value %d to stack[%d]\n",registers[address1],address2);
			 }
			 else if(address_type2 == 3){
			   uint8_t indirection = frame.stack[address2];
			   frame.stack[indirection] = registers[address1];
			   frame.stack[1] += 1; //stack_pouint8_ter
			   //printf("\nmove register value %d to stack[%d]\n",address1,frame.stack[address2]);
			 }
			   if(frame.stack[127] != 150){
				   printf("Stack Overflow!\n");
				   result = 100;
				   isValid = false;
			   }
		   }
		   else if(address_type1 == 2){
			 if(address_type2 == 1){
			   registers[address2] = frame.stack[address1];
			   //printf("\nmove stack[%d] value %d to register %d\n",address1,frame.stack[address1],address2);
			 }
			 else if(address_type2 == 2){
			   frame.stack[address2] = frame.stack[address1];
			   frame.stack[1] += 1; //stack_pouint8_ter
			   //printf("\nmove stack[%d] value %d to stack[%d]\n",address1,frame.stack[address1],address2);
			 }
			 else if(address_type2 == 3){
			   uint8_t indirection = frame.stack[address2];
			   frame.stack[indirection] = frame.stack[address1];
			   frame.stack[1] += 1; //stack_pouint8_ter
			   //printf("\nmove stack[%d] value %d to stack[%d]\n",address1,frame.stack[address1],indirection);
			 }
			 if(frame.stack[1] == 128){
				 printf("Stack Overflow!\n");
				 result = 100;
				 isValid = false;
			 }
		   }
		   else if(address_type1 == 3){
			 if(address_type2 == 1){
			   uint8_t indirection = frame.stack[address1];
			   registers[address2] = frame.stack[indirection];
			   //printf("\nmove stack[%d] value %d to register %d\n",address1,frame.stack[address1],address2);
			 }
			 else if(address_type2 == 2){
			   uint8_t indirection = frame.stack[address1];
			   frame.stack[address2] = frame.stack[indirection];
			   //printf("\nmove stack[%d] value %d to stack[%d]\n",address1,frame.stack[address1],address2);
			 }
			 else if(address_type2 == 3){
			   uint8_t indirection = frame.stack[address2];
			   uint8_t indirection1 = frame.stack[address1];
			   frame.stack[indirection] = frame.stack[indirection1];
			   //printf("\nmove stack[%d] value %d to stack[%d]\n",address1,frame.stack[address1],indirection);
			 }
		   }
			// printf("%d\n",registers[0]);
           break;
         }
         case 1:{
		   uint8_t function_no = cpy->address1;
		   uint8_t stack_start = cpy->address2;
		   for(uint8_t i = 0; i < function_count; i++){
			   if(first_function_cpy->label == function_no){
				   argsize = first_function_cpy->numarg;
				   uint8_t *argarray = malloc(argsize*sizeof(uint8_t));
				   uint8_t *argarray_cpy = argarray;
				   for(int i = 0 ; i < argsize ; i++){
					   argarray_cpy[i] = frame.stack[stack_start+i];
				   }
				   frame.stack[frame.stack[1]] = runCode(first_function_cpy,first_function,argarray,argsize);
				   free(argarray);
			   }
			   first_function_cpy = first_function_cpy->next;
		   }
           break;
         }
         case 2:{
       	   uint8_t address1 = cpy->address1;
		   result = frame.stack[address1];
           break;
         }
         case 3:{
		   if(frame.stack[127] != 150){
               printf("Stack Overflow!\n");
			   result = 100;
               isValid = false;
           }
            isValid = false;
			break; 
         }
         case 4:{
           uint8_t reg1 = cpy->address1;
           uint8_t reg2 = cpy->address2;
           registers[reg1] = registers[reg1] + registers[reg2];
           break;
         }
         case 5:{
           uint8_t reg1 = cpy->address1;
           uint8_t reg2 = cpy->address2;
           registers[reg1] = registers[reg1] & registers[reg2];
           break;
         }
         case 6:{
           uint8_t reg1 = cpy->address1;
           registers[reg1] = ~registers[reg1]+256;
           break;
         }
         case 7:{
       	   uint8_t reg1 = cpy->address1;
           if(registers[reg1] == 0){
             registers[reg1] = 1;
           }
           else{
             registers[reg1] = 0;
           }
           break;
         }
       }
     //}
	 frame.stack[2] += 1; //program_counter
  }
  return result;
}


