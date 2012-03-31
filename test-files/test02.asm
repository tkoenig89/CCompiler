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

	ADDI $sp, $sp, -48	#Allocate Memory on stackpointer for local Variables
	#int i: 44($sp)
	#int j[10]: 4($sp)
	#int .v0: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 44($sp)	#Local Variable recognised:i
	SW $5, 44($sp)	#Assign one register to another
	LW $5, 4($sp)	#Local Variable recognised:j
	LW $6, 44($sp)	#Local Variable recognised:i
	SW $5, 44($sp)	#Assign one register to another
#Error: Unrecognised command 21.
#Error: Unrecognised command 21.
#Error: Unrecognised command 22.
	LI $5, 1	#Number recognised:1
	LW $6, 0($sp)	#Local Variable recognised:.v0
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $7, 44($sp)	#Local Variable recognised:i
	SW $15, 44($sp)	#Assign one register to another
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31
