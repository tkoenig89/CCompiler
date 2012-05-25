
.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -44	#Allocate Memory on stackpointer for local Variables
	#int global[10]: 4($sp)
	#int i: 0($sp)
	LI $5, 0	#Number recognised:0
	LA $6, 4($sp)	#Local Variable recognised:global
	LI $7, 0	#Number recognised:0
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
l2:
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 10	#Number recognised:10
	SLT $14, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $14, l0
	J l1
l0:
	LW $5, 0($sp)	#Local Variable recognised:i
	LA $6, 4($sp)	#Local Variable recognised:global
	LW $7, 0($sp)	#Local Variable recognised:i
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LW $15, 0($sp)	#Local Variable recognised:i
	SW $14, 0($sp)	#Assign one register to another
	J l2
l1:
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 12	#Number recognised:12
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	MOVE $2, $14	#Return .t1
	J l3
l3:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 44	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

