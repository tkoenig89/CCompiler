.data


.text

_start:
	JAL main

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -48	#Allocate Memory on stackpointer for local Variables
	#int i: 44($sp)
	#int j[10]: 4($sp)
	#int k: 0($sp)
	LI $5, 1337	#Number recognised:1337
	LW $6, 44($sp)	#Local Variable recognised:i
	SW $5, 44($sp)	#Assign one register to another
	LI $5, 42	#Number recognised:42
	LW $6, 0($sp)	#Local Variable recognised:k
	SW $5, 0($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 0	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 1	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 2	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 3	#Number recognised:3
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 3	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 4	#Number recognised:4
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 4	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 5	#Number recognised:5
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 5	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 6	#Number recognised:6
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 6	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 7	#Number recognised:7
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 7	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 8	#Number recognised:8
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 8	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 9	#Number recognised:9
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 9	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 9	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 8	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 3	#Number recognised:3
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 7	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 4	#Number recognised:4
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 6	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 5	#Number recognised:5
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 5	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 6	#Number recognised:6
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 4	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 7	#Number recognised:7
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 3	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 8	#Number recognised:8
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 2	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	LI $5, 9	#Number recognised:9
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 4($sp)	#Local Variable recognised:j
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LA $6, 4($sp)	#Local Variable recognised:j
	LI $7, 1	#Load array position into a register
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $16, 0($9)	#Assign one register to another
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

