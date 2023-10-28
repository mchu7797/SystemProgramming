; 문제 4 :
; 8086 어셈블리로 BH Register에 09H를 넣고, 화면에 출력하라.

MAIN	SEGMENT
		ASSUME CS:MAIN
		MOV BH, 09H
		MOV DL, BH  ; 데이터 출력을 위해 데이터 레지스터에 복사
		ADD DL, 30H ; '0' == 0x30
		MOV AH, 2   ; 화면 출력 설정
		INT 21H     ; 인터럽트 실행
		MOV AH, 4CH ; 콘솔 종료 설정
		INT 21H     ; 인터럽트 실행
MAIN 	ENDS
END