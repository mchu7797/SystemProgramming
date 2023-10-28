; 문제 5 :
; 워드 자료형으로 DATA1, DAT2를 1020H와 3040H로 초기화해서 선언해둔 다음,
; 그 합을 구해 SUM에 저장하고, 그 결과의 메모리 모습을 보여라.

MAIN    SEGMENT
        ASSUME CS:MAIN, DS:MAIN        
        MOV BX, DATA1
        ADD BX, DATA2
        MOV SUM, BX

DATA1   DW 1020H
DATA2   DW 3040H
SUM     DW 0

MAIN    ENDS
END