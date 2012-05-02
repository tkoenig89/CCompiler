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

fu:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 2	#Number recognised:2
	MOVE $2, $5	#Return int
	J l0
l0:
	#End of function fu. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

func:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 2	#Number recognised:2
	MOVE $2, $5	#Return int
	J l1
l1:
	#End of function func. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -16	#Allocate Memory on stackpointer for local Variables
	#int x: 12($sp)
	#int y: 8($sp)
	#int z: 4($sp)
	#int u: 0($sp)
	LI $5, 2	#Number recognised:2
	LW $6, 12($sp)	#Local Variable recognised:x
	SW $5, 12($sp)	#Assign one register to another
	LI $5, 3	#Number recognised:3
	LW $6, 8($sp)	#Local Variable recognised:y
	SW $5, 8($sp)	#Assign one register to another
	JAL func	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $6, 0($sp)	#Local Variable recognised:u
	SW $15, 0($sp)	#Assign one register to another
	JAL -1undeclared	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $6, 0($sp)	#Local Variable recognised:u
	SW $15, 0($sp)	#Assign one register to another
	JAL func	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $6, 0($sp)	#Local Variable recognised:zts
	SW $15, 0($sp)	#Assign one register to another
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 16	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

