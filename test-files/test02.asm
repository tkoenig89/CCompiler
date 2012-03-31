.data


.text

_start:
	JAL main

	f:	# Beggining of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

#Error: Unrecognised command 20.
	#End of function f. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31
	main:	# Beggining of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -8	#Allocate Memory on stackpointer for local Variables
	#int i: 4($sp)
	#int .v0: 0($sp)
#Error: Unrecognised command 22.
	LI $5, 1	#Number recognised:1
	LW $6, 0($sp)	#Local Variable recognised:.v0
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LW $15, 4($sp)	#Local Variable recognised:i
	SW $14, 4($sp)	#Assign one register to another
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31
