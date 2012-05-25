
.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -4	#Allocate Memory on stackpointer for local Variables
	#int i: 0($sp)
	LI $5, 12	#Number recognised:12
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 12	#Number recognised:12
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 13	#Number recognised:13
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	OR $14, $14, $14	#Locigal Or, is here equal to bit OR
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 15	#Number recognised:15
	SLT $14, $6, $5	#if i1 > i2 i0 = 1 else i0 = 0
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 15	#Number recognised:15
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	OR $14, $14, $14	#Locigal Or, is here equal to bit OR
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 17	#Number recognised:17
	SLT $14, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 17	#Number recognised:17
	LI $15, 1	#Number recognised:1
	SUB $14, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $14, $15, $14	#if i0 != 0, i0 = 1
	XOR $14, $14, $15	#i0 = !i0
	OR $14, $14, $14	#Locigal Or, is here equal to bit OR
	AND $14, $14, $14	#Locigal And, is here equal to bit AND
	OR $14, $14, $14	#Locigal Or, is here equal to bit OR
	BGTZ $14, l0
	J l1
l0:
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
	J l2
l1:
	LI $5, 1	#Number recognised:1
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
l2:
	LW $5, 0($sp)	#Local Variable recognised:i
	MOVE $2, $5	#Return i
	J l3
l3:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 4	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

