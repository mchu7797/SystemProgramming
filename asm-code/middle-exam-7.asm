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
                MOV AX, 0           ; DATA1, DATA2 INDEX
                MOV BX, 0           ; DMAXS INDEX
                MOV CX, 0           ; DMINS INDEX

LOOP:           CMP AX, 8
                JB COMPARE          ; IF AX < 8 IS TRUE
                JMP EXIT            ; ELSE

COMPARE:        MOV SI, AX
                MOV DL, DATA1[SI]
                ADD DL, DATA2[SI]
                CMP DL, 100 
                JB MOVE_TO_DMINS    ; IF DL < 100 IS TRUE
                JMP MOVE_TO_DMAXS   ; ELSE

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

EXIT:           MOV AH, 2           ; 콘솔 종료 설정
                INT 21H             ; 인터럽트 실행

DATA1           DB 35, 26, 20, 3, 100, 97, 88, 15
DATA2           DB 15, 36, 64, 90, 10, 21, 32, 1
DMAXS           DB 8 DUP(0)
DMINS           DB 8 DUP(0)

MAIN            ENDS
END