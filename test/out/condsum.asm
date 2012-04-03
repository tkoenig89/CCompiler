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
.global:
	.word n	align 4
.a:
	.word 0 : 10
	align 4

.text

func:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -8	#Allocate Memory on stackpointer for local Variables
	#int i: 4($sp)
	#int sum: 0($sp)
	LI $5, 0	#Number recognised:0
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $5, 4($sp)	#Assign one register to another
	LI $5, 0	#Number recognised:0
	LW $6, 0($sp)	#Local Variable recognised:sum
	SW $5, 0($sp)	#Assign one register to another
l4:
	LW $5, 4($sp)	#Local Variable recognised:i
	LW $6, 12($fp)	#Parameter recognised:len
	SLT $15, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	BGTZ $15, l0
	J l1
l0:
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $6, 4	#Load Number 4 into a register
	MUL $7, $6, $5	#Multiplying array position by 4 (each entry has the size of 4 bytes)
	LW $8, 16($fp)	#Parameter recognised:arr
	ADD $6, $7, $8	#Add the starting position of the array to the position
	LW $16, 0($6)	#Load the Array position from the stack
	LW $6, 8($fp)	#Parameter recognised:threshold
	SLT $17, $16, $6	#if i1 < i2 i0 = 1 else i0 = 0
	LW $6, 8($fp)	#Parameter recognised:threshold
	LI $7, 1	#Number recognised:1
	SUB $18, $16, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $18, $7, $18	#if i0 != 0, i0 = 1
	XOR $18, $18, $7	#i0 = !i0
	OR $17, $17, $18	#Locigal Or, is here equal to bit OR
	BGTZ $17, l2
	J l3
l2:
	LW $5, 0($sp)	#Local Variable recognised:sum
	LI $6, 1	#Number recognised:1
	ADD $18, $5, $6	#Add 2 Variables and store result int temp register
	LW $6, 0($sp)	#Local Variable recognised:sum
	SW $18, 0($sp)	#Assign one register to another
l3:
	LW $5, 4($sp)	#Local Variable recognised:i
	LI $6, 1	#Number recognised:1
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	LW $6, 4($sp)	#Local Variable recognised:i
	SW $15, 4($sp)	#Assign one register to another
	LW $5, 4($sp)	#Local Variable recognised:i
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	J l4
l1:
	LW $5, 0($sp)	#Local Variable recognised:sum
	MOVE $2, $5	#Return sum
	ADDI $sp, $sp, 8	# delete local variables
	J l5
l5:
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

	LI $5, 1	#Number recognised:1
	LA $6, global	#Global Variable recognised:global
	SW $5, 0($6)	#Assign one register to another
	LA $5, a	#Global Variable recognised:a
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 5	#Number recognised:5
	LI $6, 5	#Number recognised:5
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	LI $5, 5	#Number recognised:5
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL func	#Call function
	ADDI $sp, $sp, 12	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	MOVE $2, $16	#Return .t2
	ADDI $sp, $sp, 8	# delete local variables
	J l6
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
l6:
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

