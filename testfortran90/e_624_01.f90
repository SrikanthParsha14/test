PROGRAM CALL_FACTORlAL   !����FACTORIAL������������
	INTERFACE
		RECURSIVE FUNCTION FACTORIAL(N) RESULT(F_RES)
			INTEGER :: N,F_RES
		END FUNCTION FACTORIAL
	END INTERFACE
	INTEGER ::N
	WRITE(*,'(A,$)') 'Input N='; READ *,N
	PRINT *,FACTORIAL(N)
END PROGRAM

RECURSIVE FUNCTION FACTORIAL(N) RESULT(F_RES)
	INTEGER :: N,F_RES
	IF(N.EQ.1) THEN
		F_RES=1
	ELSE
		F_RES=N*FACTORIAL(N-1)
	END IF
END FUNCTION FACTORIAL