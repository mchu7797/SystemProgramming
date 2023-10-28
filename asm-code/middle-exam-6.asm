; 문제 6 : 
; 아래와 같이 임의의 8개의 숫자가 DATA1 배열에 저장되어 있을 경우,
; 그 중 50보다 큰 수를 DMAXS 배열에 저장하고, 그 결과 메모리 모습을 보여라.
; 예제 => DATA1 DB 35, 26, 20, 3, 100, 97, 88, 15

MAIN SEGMENT
                ASSUME CS:MAIN, DS:MAIN
                MOV BX, 0 ; DATA1 Index
                MOV CX, 0 ; DMAXS Index

LOOP:           CMP BX, 8
                JB COMPARE ; IF JB < 8 IS TRUE
                JMP EXIT   ; ELSE

COMPARE:        MOV SI, BX
                MOV DH, DATA1[SI]
                CMP DH, 50
                JB PASS_COMPARE ; IF DH < 50 IS TRUE
                JMP MOVE_TO_MAX ; ELSE

PASS_COMPARE:   INC BX
                JMP LOOP

MOVE_TO_MAX:    MOV SI, CX
                MOV DMAXS[SI], DH
                INC BX
                INC CX
                JMP LOOP

EXIT:           MOV AH, 2
                INT 21H

DATA1           DB 35, 26, 20, 3, 100, 97, 88, 15
DMAXS           DB 8 DUP(0)

MAIN            ENDS
END