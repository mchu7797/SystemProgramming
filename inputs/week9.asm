M1	KMAC
	ADD AX, BX
	ADD AX, CX
ENDK

M2	KMAC
	PUSH AX
	MOV AX, BX
	SUB  AX, CX
	POP AX
ENDK

MAIN    SEGMENT
	    ASSUME  CS : MAIN

	    MOV  DL, 'A'
	    M1
	    MOV  AH,  2
	    INT  21H

	    M2
        M1

	    MOV  AH,  4CH
	    INT  21H

MAIN    ENDS
END