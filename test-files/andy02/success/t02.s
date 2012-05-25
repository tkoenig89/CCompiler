
.data

.text

print:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 2	#Number recognised:2
	LW $6, 8($fp)	#Parameter recognised:i
	MUL $14, $5, $6	#Multiply 2 Variables and store result int temp register
	MOVE $2, $14	#Return .t0
	J l0
l0:
	#End of function print. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

