M1  	KMAC
	    ADD AX, BX
	    ADD AX, CX
        ENDK

M2      KMAC
	    PUSH AX
	    MOV AX, BX
	    SUB  AX, CX
	    POP AX
        ENDK

M3      KMAC
        HELLO WORLD! MY NAME IS MINSEOK!
        ENDK

MAIN    SEGMENT
        ASSUME  CS:MAIN

        MOV  DL, 'A'
        M1
        MOV  AH,  2
        INT  21H

        M1
        M2
        M3

        MOV  AH,  4CH
	    INT  21H

MAIN    ENDS
END
