
.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -4	#Allocate Memory on stackpointer for local Variables
	#int i: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
l0:
	LW $5, 0($sp)	#Local Variable recognised:i
	LW $6, 0($sp)	#Local Variable recognised:i
	SW $5, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 1024	#Number recognised:1024
	SLT $14, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	LW $5, 0($sp)	#Local Variable recognised:i
	LI $6, 512	#Number recognised:512
	SLT $14, $6, $5	#if i1 > i2 i0 = 1 else i0 = 0
	AND $14, $14, $14	#Locigal And, is here equal to bit AND
	BGTZ $14, l0
	LW $5, 0($sp)	#Local Variable recognised:i
	MOVE $2, $5	#Return i
	J l1
l1:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 4	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

