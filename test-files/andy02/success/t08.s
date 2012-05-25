
.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -12	#Allocate Memory on stackpointer for local Variables
	#int i: 8($sp)
	#int a: 4($sp)
	#int b: 0($sp)
	LI $5, 1	#Number recognised:1
	LW $6, 0($sp)	#Local Variable recognised:b
	SW $5, 0($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	LW $6, 4($sp)	#Local Variable recognised:a
	SW $5, 4($sp)	#Assign one register to another
	LW $5, 4($sp)	#Local Variable recognised:a
	LI $6, 1	#Number recognised:1
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	LW $5, 0($sp)	#Local Variable recognised:b
	LI $6, 1	#Number recognised:1
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	OR $14, $14, $14	#Locigal Or, is here equal to bit OR
	LW $15, 8($sp)	#Local Variable recognised:i
	SW $14, 8($sp)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	MOVE $2, $5	#Return i
	J l0
l0:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 12	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

