
.data

.text

add:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LW $5, 12($fp)	#Parameter recognised:i
	LW $6, 8($fp)	#Parameter recognised:j
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	MOVE $2, $14	#Return .t0
	J l0
l0:
	#End of function add. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 1	#Number recognised:1
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 2	#Number recognised:2
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL add	#Call function
	ADDI $sp, $sp, 8	# clean up stack after function call is done
	MOVE $14, $2	#Save return value by storing it into a temp register
	MOVE $2, $14	#Return .t1
	J l1
l1:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

