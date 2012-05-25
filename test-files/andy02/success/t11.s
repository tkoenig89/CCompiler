
.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -8	#Allocate Memory on stackpointer for local Variables
	#int i: 4($sp)
	#int j: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $5, 4($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:j
	SW $5, 0($sp)	#Assign one register to another
l8:
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $6, 5	#Number recognised:5
	SLT $14, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $14, l0
	J l1
l0:
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:j
	SW $5, 0($sp)	#Assign one register to another
l7:
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 4	#Number recognised:4
	SLT $14, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $14, l2
	J l3
l2:
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 1	#Number recognised:1
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LW $15, 0($sp)	#Local Variable recognised:j
	SW $14, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 2	#Number recognised:2
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	BGTZ $14, l4
	J l5
l4:
	LI $5, 1	#Number recognised:1
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $5, 4($sp)	#Assign one register to another
	J l6
l5:
	LI $5, 2	#Number recognised:2
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $5, 4($sp)	#Assign one register to another
l6:
	J l7
l3:
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LW $15, 4($sp)	#Local Variable recognised:i
	SW $14, 4($sp)	#Assign one register to another
	J l8
l1:
	LW $5, 4($sp)	#Local Variable recognised:i
	MOVE $2, $5	#Return i
	J l9
l9:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 8	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

