# Assembler Spec
## Instruction Set
 - MOV
 - ADD
 - SUB
 - AND
 - OR
 - INC
 - DEC
 - CMP
 - JMP
 - JA
 - JB
 - JE
 - DW
 - DB
 - ASSUME
 - SEGMENT
 - ENDS
 - END
## Instruction Format
 - Operator Number
 - Operator Memory
 - Operator Register, Register
 - Operator Register, Number
 - Operator Register, Memory
 - Operator Memory, Register
 - Operator Register, RegisterPointer
 - Operator RegisterPointer, Register
## Register Set
 - AX, BX, CX, DX
 - AL, AH, BL, BH, CL, CH, DL, DH
 - SI, DI
### Exception
 - AX, CS When call MOV
 - DS, AX When call MOV