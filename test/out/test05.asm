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

fib:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	ADDI $sp, $sp, -8	#Allocate Memory on stackpointer for local Variables
	#int a: 4($sp)
	#int b: 0($sp)
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 0	#Number recognised:0
	LI $7, 1	#Number recognised:1
	SUB $15, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $15, $7, $15	#if i0 != 0, i0 = 1
	XOR $15, $15, $7	#i0 = !i0
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 1	#Number recognised:1
	LI $7, 1	#Number recognised:1
	SUB $16, $5, $6	#If i1 == i2, i0 = 0, else i0 != 0
	MOVN $16, $7, $16	#if i0 != 0, i0 = 1
	XOR $16, $16, $7	#i0 = !i0
	OR $15, $15, $16	#Locigal Or, is here equal to bit OR
	BGTZ $15, l0
	J l1
l0:
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 1	#Number recognised:1
	SLT $16, $6, $5	#if i1 > i2 i0 = 1 else i0 = 0
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 0	#Number recognised:0
	SLT $17, $5, $6	#if i1 < i2 i0 = 1 else i0 = 0
	OR $16, $16, $17	#Locigal Or, is here equal to bit OR
	BGTZ $16, l2
	J l3
l2:
	JAL scan	#Call function
	MOVE $17, $2	#Save return value by storing it into a temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $17, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $18, $2	#Save return value by storing it into a temp register
l3:
	LW $5, 8($fp)	#Parameter recognised:n
	MOVE $2, $5	#Return n
	ADDI $sp, $sp, 8	# delete local variables
	J l4
	JAL scan	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	J l4
l1:
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 1	#Number recognised:1
	SUB $15, $5, $6	#Subtract 2 Variables and store result int temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL fib	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LW $6, 4($sp)	#Local Variable recognised:a
	SW $16, 4($sp)	#Assign one register to another
	LW $5, 8($fp)	#Parameter recognised:n
	LI $6, 2	#Number recognised:2
	SUB $15, $5, $6	#Subtract 2 Variables and store result int temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL fib	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
	LW $6, 0($sp)	#Local Variable recognised:b
	SW $16, 0($sp)	#Assign one register to another
	LW $5, 4($sp)	#Local Variable recognised:a
	LW $6, 0($sp)	#Local Variable recognised:b
	ADD $15, $5, $6	#Add 2 Variables and store result int temp register
	MOVE $2, $15	#Return .t1
	ADDI $sp, $sp, 8	# delete local variables
	J l4
	JAL scan	#Call function
	MOVE $15, $2	#Save return value by storing it into a temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
l4:
	#End of function fib. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

main:	# Beginning of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

	LI $5, 16	#Number recognised:16
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $5, 0($sp)	#Copy Value/Adress of var to stack var
	JAL fib	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $15, $2	#Save return value by storing it into a temp register
	ADDI $sp, $sp, -4	#Reserve 4 Bytes on the Stack for a parameter and the func call
	SW $15, 0($sp)	#Copy Value/Adress of var to stack var
	JAL print	#Call function
	ADDI $sp, $sp, 4	# clean up stack after function call is done
	MOVE $16, $2	#Save return value by storing it into a temp register
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

