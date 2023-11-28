# Assembler Spec
## Instruction Set
 - MOV     (Implemented)
 - ADD     (Implemented)
 - SUB     (Implemented)
 - AND     (Implemented)
 - OR      (Implemented)
 - INC     (Implemented)
 - DEC     (Implemented)
 - CMP     (Implemented)
 - JMP     (Implemented)
 - JA      (Implemented)
 - JB      (Implemented)
 - JE      (Implemented)
 - DW      (Implemented)
 - DB      (Implemented)
 - ASSUME  (Implemented)
 - SEGMENT (Implemented)
 - ENDS    (Implemented)
 - END     (Implemented)
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