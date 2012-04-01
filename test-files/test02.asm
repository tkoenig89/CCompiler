.data


.text

_start:
	JAL main

f:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 5	#Number recognised:5
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 8($fp)	#Parameter recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $14, 0($6)	#Load the Array position from the stack
	LI $5, 55	#Number recognised:55
	SW $5, 0($14)	#Assign one register to another
	LI $5, 7	#Number recognised:7
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 8($fp)	#Parameter recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	LI $5, 77	#Number recognised:77
	SW $5, 0($15)	#Assign one register to another
	LI $5, 5	#Number recognised:5
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 8($fp)	#Parameter recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	LI $5, 7	#Number recognised:7
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 8($fp)	#Parameter recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	MUL $15, $15, $16	#Multiply 2 Variables and store result int temp register
	LW $6, 12($fp)	#Parameter recognised:i
	SW $15, 12($fp)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LW $6, 12($fp)	#Parameter recognised:i
	SW $5, 12($fp)	#Assign one register to another
#Error: Unrecognised command 20.
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

	ADDI $sp, $sp, -48	#Allocate Memory on stackpointer for local Variables
	#int i: 44($sp)
	#int j[10]: 4($sp)
	#int .v0: 0($sp)
	LI $5, 2	#Number recognised:2
	LW $6, 44($sp)	#Local Variable recognised:i
	SW $5, 44($sp)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	LI $5, 33	#Number recognised:33
	SW $5, 0($15)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	LW $6, 44($sp)	#Local Variable recognised:i
	MUL $15, $15, $6	#Multiply 2 Variables and store result int temp register
	LW $7, 44($sp)	#Local Variable recognised:i
	SW $15, 44($sp)	#Assign one register to another
	LW $5, 44($sp)	#Local Variable recognised:i
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	LA $5, 4($sp)	#Local Variable recognised:j
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL f	#Call function
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

