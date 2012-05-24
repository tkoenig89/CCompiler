
.data

.text

f:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LW $5, 16($fp)	#Parameter recognised:i
	MOVE $2, $5	#Return i
	J l0
l0:
	#End of function f. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -20	#Allocate Memory on stackpointer for local Variables
	#int b[5]: 0($sp)
	LI $5, 5	#Number recognised:5
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 5	#Number recognised:5
	LI $6, 5	#Number recognised:5
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LI $15, 5	#Number recognised:5
	ADD $14, $14, $15	#Add 2 Variables and store result int temp register
	LI $15, 5	#Number recognised:5
	ADD $14, $14, $15	#Add 2 Variables and store result int temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $14, 0($sp)	#Copy Value/Adress of var to stack var
	LA $5, 0($sp)	#Local Variable recognised:b
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL f	#Call function
	ADDI $sp, $sp, 12	# clean up stack after function call is done
	MOVE $14, $2	#Save return value by storing it into a temp register
	MOVE $2, $14	#Return .t4
	J l1
l1:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 20	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

