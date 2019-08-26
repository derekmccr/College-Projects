/* 
 * File:   vm.c
 * Author: Derek McCrae
 * Assignment: PM/0 - COP3402
 * Created on September 14, 2018, 4:20 PM
 */



#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "data.h"

/* ************************************************************************************ */
/* Declarations                                                                         */
/* ************************************************************************************ */

/**
 * Recommended design includes the following functions implemented.
 * However, you are free to change them as you wish inside the vm.c file.
 * */
void initVM(VirtualMachine*);

int readInstructions(FILE*, Instruction*);

void dumpInstructions(FILE*, Instruction*, int numOfIns);

void dumpStack(FILE*, int* stack, int sp, int bp, int* bp_array);

int executeInstruction(VirtualMachine* vm, Instruction*, int* bp_array, FILE* vmIn, FILE* vmOut);

int base(int l, int bp, VirtualMachine* vm);

/* ************************************************************************************ */
/* Global Data and misc structs & enums                                                 */
/* ************************************************************************************ */

/**
 * allows conversion from opcode to opcode string
 * */
const char *opcodes[] = 
{
    "illegal", // opcode 0 is illegal
    "lit", "rtn", "lod", "sto", "cal", // 1, 2, 3 ..
    "inc", "jmp", "jpc", "sio", "sio",
    "sio", "neg", "add", "sub", "mul",
    "div", "odd", "mod", "eql", "neq",
    "lss", "leq", "gtr", "geq"
};

enum { CONT, HALT };

/* ************************************************************************************ */
/* Definitions                                                                          */
/* ************************************************************************************ */

  

/**
 * Initialize Virtual Machine
 * */
void initVM(VirtualMachine* vm){
    int i;
    if(vm){
    vm->BP = 1;
    vm->SP = 0;
    vm->PC = 0;
    vm->IR = 0;
    for(i = 0; i < MAX_STACK_HEIGHT; i++){
      vm->stack[i] = 0;
    }
    for(i = 0; i < REGISTER_FILE_REG_COUNT; i++){
      vm->RF[i] = 0;
    }
  }//end if
}

/**
 * Fill the (ins)tructions array by reading instructions from (in)put file
 * Return the number of instructions read
 * */
int readInstructions(FILE* in, Instruction* ins){
    int i = 0, count = 0;
    //4 integers per line
    while(fscanf(in, "%d %d %d %d", &ins[i].op, &ins[i].r, &ins[i].l, &ins[i].m) == 4){
        i++;
        count++;
    }//end while loop
    return count;
}

/**
 * Dump instructions to the output file
 * */
void dumpInstructions(FILE* out, Instruction* ins, int numOfIns){
    int i;
    // Header
    fprintf(out, "***Code Memory***\n%3s %3s %3s %3s %3s \n", "#", "OP", "R", "L", "M");

    // Instructions
    for(i = 0; i < numOfIns; i++){
      fprintf(out, "%3d %3s %3d %3d %3d\n", i, opcodes[ins[i].op], ins[i].r, ins[i].l, ins[i].m);
    }

    return;
}

/**
 * Returns the base pointer for the lexiographic level L
 * */
//int getBasePointer(int *stack, int currentBP, int L)
int base(int l, int bp, VirtualMachine* vm){
    int b1 = bp;
    while(l > 0){
        b1 = vm->stack[b1 + 1];
        l--;
    }
    return b1;
}

// Function that dumps the whole stack into output file
// Do not forget to use '|' character between stack frames
void dumpStack(FILE* out, int* stack, int sp, int bp, int *bp_array){
    int i, j;
    for(i = 0; i <= sp; i++){
        for(j = 0; j < MAX_STACK_HEIGHT / 2; j++){
            if(i == 1){
                fprintf(out, "| %3d ", stack[i]);
                break;
            }
            if(i == bp_array[j] && (sp > 0 && i > 0)){
                fprintf(out, "| %3d ", stack[i]);
                break;
            }
            if(bp_array[j] == 0){
                fprintf(out, "%3d ", stack[i]);
                break;
            }
        }
    }
    fprintf(out, "\n");
}

/**
 * Executes the (ins)truction on the (v)irtual (m)achine.
 * This changes the state of the virtual machine.
 * Returns HALT if the executed instruction was meant to halt the VM.
 * .. Otherwise, returns CONT
 * */
int executeInstruction(VirtualMachine* vm, Instruction *ins, int* bp_array, FILE* vmIn, FILE* vmOut){
    int r, l, m, i;
    r = ins[vm->IR].r;
    l = ins[vm->IR].l;
    m = ins[vm->IR].m;
    switch(ins[vm->IR].op){
        case 1:
            vm->RF[r] = m;
            break;
        case 2:
            vm->SP = vm->BP - 1;
            vm->BP = vm->stack[vm->SP + 3];
            vm->PC = vm->stack[vm->SP + 4];
            break;
        case 3:
            vm->RF[r] = vm->stack[base(l, vm->BP, vm) + m];
            break;
        case 4:
            vm->stack[base(l, vm->BP, vm) + m] = vm->RF[r];
            break;
        case 5:
            vm->stack[vm->SP + 1] = 0;
            vm->stack[vm->SP + 2] = base(l, vm->BP, vm);
            vm->stack[vm->SP + 3] = vm->BP;
            vm->stack[vm->SP + 4] = vm->PC;
            vm->BP = vm->SP + 1;
            vm->PC = m;
            for(i = 0; i < MAX_STACK_HEIGHT / 2; i++){
                if(bp_array[0] == 0)
                    bp_array[0] = 1;
                if(bp_array[i] == 0){
                    bp_array[i] = vm->BP;
                    break;
                }
            }
            break;
        case 6:
            vm->SP = vm->SP + m;
            break;
        case 7:
            vm->PC = m;
            break;
        case 8:
            if(vm->RF[r] == 0)
                vm->PC = m;
            break;
        case 9:
            fprintf(vmOut, "%d ", vm->RF[r]);
            break;
        case 10:
            fscanf(vmIn, "%d", &vm->RF[r]);
            break;
        case 11:
            return HALT;
        case 12:
            vm->RF[r] = -(vm->RF[l]);
            break;
        case 13:
            vm->RF[r] = vm->RF[l] + vm->RF[m];
            break;
        case 14:
            vm->RF[r] = vm->RF[l] - vm->RF[m];
            break;
        case 15:
            vm->RF[r] = vm->RF[l] * vm->RF[m];
            break;
        case 16:
            vm->RF[r] = vm->RF[l] / vm->RF[m];
            break;
        case 17:
            vm->RF[r] = vm->RF[r] % 2;
            break;
        case 18:
            vm->RF[r] = vm->RF[l] % vm->RF[m];
            break;
        case 19:
            if(vm->RF[l] == vm->RF[m])
                vm->RF[r] = 1;
            else
                vm->RF[r] = 0;
            break;
        case 20:
            if(vm->RF[l] != vm->RF[m])
                vm->RF[r] = 1;
            else
                vm->RF[r] = 0;
            break;
        case 21:
            if(vm->RF[l] < vm->RF[m])
                vm->RF[r] = 1;
            else
                vm->RF[r] = 0;
            break;
        case 22:
            if(vm->RF[l] <= vm->RF[m])
                vm->RF[r] = 1;
            else
                vm->RF[r] = 0;
            break;
        case 23:
            if(vm->RF[l] > vm->RF[m]){
                vm->RF[r] = 1;
            }
            else
                vm->RF[r] = 0;
            break;
        case 24:
            if(vm->RF[l] >= vm->RF[m])
                vm->RF[r] = 1;
            else
                vm->RF[r] = 0;
            break;

        default:
            fprintf(stderr, "Illegal instruction?");
            return HALT;
    }

    return CONT;
}

/**
 * inp: The FILE pointer containing the list of instructions to
 *         be loaded to code memory of the virtual machine.
 * 
 * outp: The FILE pointer to write the simulation output, which
 *       contains both code memory and execution history.
 * 
 * vm_inp: The FILE pointer that is going to be attached as the input
 *         stream to the virtual machine. Useful to feed input for SIO
 *         instructions.
 * 
 * vm_outp: The FILE pointer that is going to be attached as the output
 *          stream to the virtual machine. Useful to save the output printed
 *          by SIO instructions.
 * */

void simulateVM(FILE* inp, FILE* outp, FILE* vm_inp, FILE* vm_outp){
    int numOfIns = -1, i = 0, halt = 0;
    Instruction Ins[MAX_CODE_LENGTH];
    int bp_array[MAX_STACK_HEIGHT / 2];
    for(i = 0; i < MAX_STACK_HEIGHT / 2; i++){
        bp_array[i] = 0;
    }
			    
    // Read instructions from file
    numOfIns = readInstructions(inp, Ins);
			     

    // Dump instructions to the output file
    if(numOfIns > -1)
        dumpInstructions(outp, Ins, numOfIns);
			     
			     
    // Before starting the code execution on the virtual machine,
    // .. write the header for the simulation part (***Execution***)
    fprintf(outp, "***Execution***\n%3s %3s %3s %3s %3s %3s %3s %3s %3s \n", "#", "OP", "R", "L", "M", "PC", "BP", "SP", "STK");


    // Create a virtual machine
    VirtualMachine vm;

    // Initialize the virtual machine
    initVM(&vm);

    // Fetch&Execute the instructions on the virtual machine until halting
    while(halt == 0){
        // Fetch&Execute
        vm.IR =vm.PC;
        vm.PC ++;
        halt = executeInstruction(&vm, Ins, bp_array, vm_inp, vm_outp);

        fprintf(outp, "%3d %3s %3d %3d %3d %3d %3d %3d ", vm.IR, opcodes[Ins[vm.IR].op], Ins[vm.IR].r, Ins[vm.IR].l, Ins[vm.IR].m, vm.PC, vm.BP, vm.SP);
        dumpStack(outp, vm.stack, vm.SP, vm.BP, bp_array);
      }

      // Above loop ends when machine halts. Therefore, dump halt message.
      fprintf(outp, "HLT\n");
      return;
}
