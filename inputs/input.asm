M_LOOP          MACRO
LOOP:           CMP AX, 8
                JB COMPARE
                JMP EXIT
M_LOOP          ENDM

M_COMPARE       MACRO
COMPARE:        MOV SI, AX
                MOV DL, DATA1[SI]
                ADD DL, DATA2[SI]
                CMP DL, 100
                JB MOVE_TO_DMINS
                JMP MOVE_TO_DMAXS
M_COMPARE       ENDM

M_EXIT          MACRO
EXIT:           MOV AH, 2
                INT 21H
M_EXIT          ENDM

MAIN            SEGMENT
                ASSUME CS:MAIN, DS:MAIN
                MOV AX, 0
                MOV BX, 0
                MOV CX, 0

                CALL M_LOOP

                CALL M_COMPARE

MOVE_TO_DMAXS:  MOV SI, BX
                MOV DMAXS[SI], DL
                INC AX
                INC BX
                JMP LOOP

MOVE_TO_DMINS:  MOV SI, CX
                MOV DMINS[SI], DL
                INC AX
                INC CX
                JMP LOOP

                CALL M_EXIT

DATA1           DB 35, 26, 20, 3, 100, 97, 88, 15
DATA2           DB 15, 36, 64, 90, 10, 21, 32, 1
DMAXS           DB 8 DUP(0)
DMINS           DB 8 DUP(0)

MAIN            ENDS
END
