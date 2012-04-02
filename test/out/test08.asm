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

	ADDI $sp, $sp, -28	#Allocate Memory on stackpointer for local Variables
	#int a: 24($sp)
	#int b: 20($sp)
	#int c: 16($sp)
	#int d: 12($sp)
	#int e: 8($sp)
	#int f: 4($sp)
	#int g: 0($sp)
	LW $5, 24($sp)	#Local Variable recognised:a
	LW $6, 24($sp)	#Local Variable recognised:a
	SW $5, 24($sp)	#Assign one register to another
l1:
	LW $5, 20($sp)	#Local Variable recognised:b
	LW $6, 20($sp)	#Local Variable recognised:b
	SW $5, 20($sp)	#Assign one register to another
l0:
	LW $5, 16($sp)	#Local Variable recognised:c
	LW $6, 16($sp)	#Local Variable recognised:c
	SW $5, 16($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	BGTZ $5, l0
	LW $5, 12($sp)	#Local Variable recognised:d
	LW $6, 12($sp)	#Local Variable recognised:d
	SW $5, 12($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	BGTZ $5, l1
	LW $5, 8($sp)	#Local Variable recognised:e
	LW $6, 8($sp)	#Local Variable recognised:e
	SW $5, 8($sp)	#Assign one register to another
	JAL scan	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LI $5, 0	#Number recognised:0
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL exit	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

