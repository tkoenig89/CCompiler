.text
_start:
  jal main
  addi $sp, $sp, -4 
  sw $2, 0($sp)
  jal exit
.text

print:
  addi $sp, $sp, -8
  sw $31, 4($sp)
  sw $fp, 0($sp)
  move $fp, $sp

  li $2, 1 
  lw $4, 8($fp)
  syscall 

  li $2, 11
  li $4, 10            # \n
  syscall

  lw $fp, 0($sp)
  lw $31, 4($sp)
  addi $sp, $sp, 8
  jr $31

scan:
  addi $sp, $sp, -8
  sw $31, 4($sp)
  sw $fp, 0($sp)
  move $fp, $sp

  li $2, 5
  syscall 

  lw $fp, 0($sp)
  lw $31, 4($sp)
  addi $sp, $sp, 8
  jr $31

exit:
  addi $sp, $sp, -8
  sw $31, 4($sp)
  sw $fp, 0($sp)
  move $fp, $sp

  li $2, 17 
  lw $4, 8($fp)
  syscall

.data

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -12	#Allocate Memory on stackpointer for local Variables
	#int i: 8($sp)
	#int b: 4($sp)
	#int a: 0($sp)
	LI $5, 12	#Number recognised:12
	LW $6, 0($sp)	#Local Variable recognised:a
	SW $5, 0($sp)	#Assign one register to another
	LI $5, 13	#Number recognised:13
	LW $6, 4($sp)	#Local Variable recognised:b
	SW $5, 4($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:a
	LW $6, 0($sp)	#Local Variable recognised:a
	ADD $14, $5, $6	#Add 2 Variables and store result int temp register
	LW $15, 4($sp)	#Local Variable recognised:b
	ADD $14, $14, $15	#Add 2 Variables and store result int temp register
	LW $5, 4($sp)	#Local Variable recognised:b
	LW $6, 0($sp)	#Local Variable recognised:a
	MUL $14, $5, $6	#Multiply 2 Variables and store result int temp register
	ADD $14, $14, $14	#Add 2 Variables and store result int temp register
	LW $15, 4($sp)	#Local Variable recognised:b
	SW $14, 4($sp)	#Assign one register to another
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

