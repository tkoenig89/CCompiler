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

	ADDI $sp, $sp, -52	#Allocate Memory on stackpointer for local Variables
	#int bla1[10]: 12($sp)
	#int i: 8($sp)
	#int j: 4($sp)
	#int hax[1]: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 8($sp)	#Local Variable recognised:i
	SW $5, 8($sp)	#Assign one register to another
l2:
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 10	#Number recognised:10
	SLT $15, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $15, l0
	J l1
l0:
	LI $5, 42	#Number recognised:42
	LA $6, 12($sp)	#Local Variable recognised:bla1
	LW $7, 8($sp)	#Local Variable recognised:i
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $6, 8($sp)	#Local Variable recognised:i
	SW $15, 8($sp)	#Assign one register to another
	J l2
l1:
	LI $5, 0	#Number recognised:0
	LW $6, 8($sp)	#Local Variable recognised:i
	SW $5, 8($sp)	#Assign one register to another
l5:
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 10	#Number recognised:10
	SLT $15, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $15, l3
	J l4
l3:
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 2143285184	#Number recognised:2143285184
	SUB $16, $5, $6	#Subtract 2 Variables and store result int temp register
	LW $6, 4($sp)	#Local Variable recognised:j
	SW $16, 4($sp)	#Assign one register to another
	LI $5, 1337	#Number recognised:1337
	LA $6, 0($sp)	#Local Variable recognised:hax
	LW $7, 4($sp)	#Local Variable recognised:j
	LI $8, 4	#Load Number 4 into a register
	MUL $9, $8, $7	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	ADD $9, $6, $9	#Add the starting position of the array to the position
	SW $5, 0($9)	#Assign one register to another
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 12($sp)	#Local Variable recognised:bla1
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LW $5, 8($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $6, 8($sp)	#Local Variable recognised:i
	SW $15, 8($sp)	#Assign one register to another
	J l5
l4:
	LI $5, 0	#Number recognised:0
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 12($sp)	#Local Variable recognised:bla1
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LI $5, 1	#Number recognised:1
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 12($sp)	#Local Variable recognised:bla1
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LI $5, 0	#Number recognised:0
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LA $8, 0($sp)	#Local Variable recognised:hax
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $15, 0($6)	#Load the Array position from the stack
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	ADDI $sp, $sp, 52	# delete local variables
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

