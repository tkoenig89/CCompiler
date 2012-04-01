.data


.text

_start:
	JAL main

f:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -12	#Allocate Memory on stackpointer for local Variables
	#int d: 8($sp)
	#int e: 4($sp)
	#int f: 0($sp)
	LW $5, 8($fp)	#Parameter recognised:c
	LW $6, 8($sp)	#Local Variable recognised:d
	SW $5, 8($sp)	#Assign one register to another
	LW $5, 12($fp)	#Parameter recognised:b
	LW $6, 4($sp)	#Local Variable recognised:e
	SW $5, 4($sp)	#Assign one register to another
	LW $5, 16($fp)	#Parameter recognised:a
	LW $6, 0($sp)	#Local Variable recognised:f
	SW $5, 0($sp)	#Assign one register to another
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

	ADDI $sp, $sp, -4	#Allocate Memory on stackpointer for local Variables
	#int .v0: 0($sp)
	LI $5, 42	#Number recognised:42
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 1447	#Number recognised:1447
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 101	#Number recognised:101
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL f	#Call function
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

