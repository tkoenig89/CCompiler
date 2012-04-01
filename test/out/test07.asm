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

	ADDI $sp, $sp, -68	#Allocate Memory on stackpointer for local Variables
	#int i_: 64($sp)
	#int a: 60($sp)
	#int b: 56($sp)
	#int c: 52($sp)
	#int d: 48($sp)
	#int e: 44($sp)
	#int f: 40($sp)
	#int g: 36($sp)
	#int h: 32($sp)
	#int i: 28($sp)
	#int j: 24($sp)
	#int k: 20($sp)
	#int l: 16($sp)
	#int m: 12($sp)
	#int n: 8($sp)
	#int o: 4($sp)
	#int p: 0($sp)
	LW $5, 60($sp)	#Local Variable recognised:a
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
	LI $5, 1	#Number recognised:1
	BGTZ $5, l0
	J l1
l0:
	LW $5, 56($sp)	#Local Variable recognised:b
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
	LI $5, 2	#Number recognised:2
	BGTZ $5, l2
	J l3
l2:
	LW $5, 52($sp)	#Local Variable recognised:c
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
	J l4
l3:
	LW $5, 48($sp)	#Local Variable recognised:d
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
l4:
	LW $5, 44($sp)	#Local Variable recognised:e
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
	J l5
l1:
	LW $5, 40($sp)	#Local Variable recognised:f
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
l5:
	LW $5, 36($sp)	#Local Variable recognised:g
	LW $6, 64($sp)	#Local Variable recognised:i_
	SW $5, 64($sp)	#Assign one register to another
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31

