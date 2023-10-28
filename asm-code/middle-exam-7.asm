; 문제 7 :
; 아래와 같이 임의의 8개의 숫자가 DATA1, DATA2 배열에 저장되어 있는 경우
; 두 배열의 N번째 항목들의 합을 구하고, 그 합이 100보다 큰 수는 DMAXS 배열에,
; 작은 수는 DMINS 배열에 저장하고, 그 결과의 메모리 모습을 보여라.
; 데이터 :
; DATA1 DB 35, 26, 20, 3, 100, 97, 88, 15
; DATA2 DB 15, 36, 64, 90, 10, 21, 32, 1
; DMAXS DB 10 DUP(0)
; DMINS DB 10 DUP(0)

MAIN SEGMENT
            ASSUME CS:MAIN, DS:MAIN
            MOV BX, 0 ; DATA INDEX
            MOV CX, 0 ; MAX INDEX
            MOV DX, 0 ; MIN INDEX

LOOP:       CMP BX, 8
            JB COMPARE ; IF BX < 8 IS TRUE
            JMP EXIT

COMPARE:    MOV SI, BX
            MOV AL, DATA1[SI]
            ADD AL, DATA2[SI]
            CMP AL, 100
            JB MOVE_MIN
            JMP MOVE_MAX

MOVE_MAX:   MOV SI, CX
            MOV DMAXS[SI], AL
            INC BX
            INC CX
            JMP LOOP

MOVE_MIN:   MOV SI, DX
            MOV DMINS[SI], AL
            INC BX
            INC DX
            JMP LOOP

EXIT:       MOV AH, 2
            INT 21H

DATA1       DB 35, 26, 20, 3, 100, 97, 88, 15
DATA2       DB 15, 36, 64, 90, 10, 21, 32, 1
DMAXS       DB 8 DUP(0)
DMINS       DB 8 DUP(0)

MAIN ENDS
END