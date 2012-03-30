.data
.global:
	.word n	align 4
.a:
	.word n	align 4
.b:
	.word n	align 4
.c:
	.word n	align 4
.d:
	.word n	align 4
.e:
	.word n	align 4
.f:
	.word n	align 4
.g:
	.word n	align 4
.h:
	.word n	align 4
.arr1:
	.word 0 : 10
	align 4
.arr2:
	.word 0 : 1337
	align 4


.text

_start:
	JAL main

	main:	# Beggining of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -44	#Allocate Memory on stackpointer for local Variables
	#int i: 40($sp)
	#int c: 36($sp)
	#int j[5]: 16($sp)
	#int k[3]: 4($sp)
	#int .v0: 0($sp)
	LI $5, 2	#Number recognised:2
	LW $6, 40($sp)	#Local Variable recognised:i
	SW $5, 40($sp)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LI $6, 4	#Load array position into a register
	MUL $7, $5, $6	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 4($sp)	#Local Variable recognised:k
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	LI $5, 1	#Number recognised:1
	SW $5, 0($15)	#Assign one register to another
	LW $5, 36($sp)	#Local Variable recognised:c
	LA $6, d	#Global Variable recognised:d
	MUL $15, $5, $6	#Multiply 2 Variables and store result int temp register
	LW $5, 40($sp)	#Local Variable recognised:i
	LA $6, g	#Global Variable recognised:g
	MUL $16, $5, $6	#Multiply 2 Variables and store result int temp register
	ADD $15, $15, $16	#Add 2 Variables and store result int temp register
	LW $7, 40($sp)	#Local Variable recognised:i
	SW $15, 40($sp)	#Assign one register to another
	LW $5, 40($sp)	#Local Variable recognised:i
	NOT $15, $5	#(pseudo):x = !y
	LW $6, 40($sp)	#Local Variable recognised:i
	SW $15, 40($sp)	#Assign one register to another
	LW $5, 40($sp)	#Local Variable recognised:i
	NEGU $15, $5	#(pseudo):x = -y
	LW $6, 40($sp)	#Local Variable recognised:i
	SW $15, 40($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	LI $6, 0	#Number recognised:0
	AND $15, $5, $6	#Locigal And, is here equal to bit AND
	LW $7, 36($sp)	#Local Variable recognised:c
	SW $15, 36($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LI $6, 1	#Number recognised:1
	OR $15, $5, $6	#Locigal Or, is here equal to bit OR
	LA $7, h	#Global Variable recognised:h
	SW $15, 0($7)	#Assign one register to another
#Error: Unrecognised command 20.
#Error: Unrecognised command 22.
#Error: Unrecognised command 23.
	LW $5, 40($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $7, 40($sp)	#Local Variable recognised:i
	SW $15, 40($sp)	#Assign one register to another
#Error: Unrecognised command 15.
#Error: Unrecognised command 16.
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31
