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

	ADDI $sp, $sp, -8	#Allocate Memory on stackpointer for local Variables
	#int i: 4($sp)
	#int j: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $5, 4($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:j
	SW $5, 0($sp)	#Assign one register to another
l1:
	LW $5, 4($sp)	#Local Variable recognised:i
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	SUB $15, $5, $6	#Subtract 2 Variables and store result int temp register
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $15, 4($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:j
	SW $5, 0($sp)	#Assign one register to another
l0:
	LW $5, 0($sp)	#Local Variable recognised:j
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 1	#Number recognised:1
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $6, 0($sp)	#Local Variable recognised:j
	SW $15, 0($sp)	#Assign one register to another
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 10	#Number recognised:10
	SLT $15, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	LW $5, 0($sp)	#Local Variable recognised:j
	LI $6, 10	#Number recognised:10
	LI $7, 1	#Number recognised:1
	SUB $16, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $16, $7, $16	#if i0 != 0, i0 = 1
	XOR $16, $16, $7	#i0 = !i0
	OR $15, $15, $16	#Locigal Or, is here equal to bit OR
	BGTZ $15, l0
	LI $5, 10	#Number recognised:10
	NEGU $15, $5	#(pseudo):x = -y
	LW $5, 4($sp)	#Local Variable recognised:i
	SLT $16, $15, $5	#if i1 > i2 i0 = 1 else i0 = 0
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $7, 1	#Number recognised:1
	SUB $17, $5, $15	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $17, $7, $17	#if i0 != 0, i0 = 1
	XOR $17, $17, $7	#i0 = !i0
	OR $16, $16, $17	#Locigal Or, is here equal to bit OR
	BGTZ $16, l1
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

