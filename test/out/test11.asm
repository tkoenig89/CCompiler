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
	#int j: 4($sp)
	#int k: 0($sp)
	LI $5, 2	#Number recognised:2
	LW $6, 4($sp)	#Local Variable recognised:j
	SW $5, 4($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	LW $6, 0($sp)	#Local Variable recognised:k
	SW $5, 0($sp)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	NEGU $15, $5	#(pseudo):x = -y
	LW $5, 4($sp)	#Local Variable recognised:j
	LW $6, 0($sp)	#Local Variable recognised:k
	SW $5, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:k
	SW $5, 0($15)	#Assign one register to another
	LW $5, 4($sp)	#Local Variable recognised:j
	LW $6, 0($sp)	#Local Variable recognised:k
	SW $5, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:k
	LW $6, 8($sp)	#Local Variable recognised:i
	SW $5, 8($sp)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	SW $5, 0($6)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	LI $5, 2	#Number recognised:2
	SW $5, 0($15)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $5, 0($sp)	#Local Variable recognised:k
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $5, 4($sp)	#Local Variable recognised:j
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

