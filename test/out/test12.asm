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
a:
	.word 0
	.align 4
b:
	.word 0
	.align 4
c:
	.word 0
	.align 4
d:
	.word 0
	.align 4
e:
	.word 0 : 1337
	.align 4
f:
	.word 0 : 1338
	.align 4

.text

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 2	#Number recognised:2
	LA $6, b	#Global Variable recognised:b
	SW $5, 0($6)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	LA $6, c	#Global Variable recognised:c
	SW $5, 0($6)	#Assign one register to another
	LA $5, b	#Global Variable recognised:b
	LA $6, d	#Global Variable recognised:d
	MUL $15, $5, $6	#Multiply 2 Variables and store result int temp register
	LI $6, 1	#Number recognised:1
	ADD $16, $15, $6	#Add 2 Variables and store result int temp register
	LA $6, d	#Global Variable recognised:d
	SW $16, 0($6)	#Assign one register to another
	LA $5, d	#Global Variable recognised:d
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

