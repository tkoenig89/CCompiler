.data
global:
  .word 0
  .align 4
a:
  .word 0 : 10
  .align 4

.text

func:
  ### function prolog ###
  addi $sp, $sp, -8
  sw $31, 4($sp)
  sw $fp, 0($sp)
  move $fp, $sp

  ### create local variables & stack space for function call  ####
  addi $sp, $sp, -12
  li $5, 0
  sw $5, 8($sp)           # i = 0
  li $5, 0
  sw $5, 4($sp)           # sum = 0
l0:
  lw $5, 8($sp)           # load val of i
  lw $6, 12($fp)          # load val of len
  sub $7, $5, $6 
  bgez $7, l2             # if i >= len goto l2
  lw $5, 8($sp)           # load val of i
  li $6, 4                # load constant 4
  mul $7, $5, $6
  lw $5, 8($fp)           # load addr of array
  add $6, $7, $5  
  lw $5, 0($6)            # load val of arr[i]
  lw $6, 16($fp)          # load val of threshold
  sub $7, $5, $6
  bltz $7, l1             # if arr[i] < threshold
  lw $5, 4($sp)           # load val of sum
  li $6, 1
  add $7, $5, $6          # increment sum by 1
  sw $7, 4($sp)           # store val of sum
l1:
  lw $5, 8($sp)           # load val of i
  li $6, 1 
  add $7, $5, $6          # increment i by 1
  sw $7, 8($sp)           # store val of i
  lw $5, 8($sp)           # load val of i
  sw $5, 0($sp)           # put i on the stack
  jal print               # call print

  j l0                    # jump back to loop cond evaluation
l2:
  lw $5, 4($sp)           # load val of sum
  move $2, $5             # 'return' sum
  addi $sp, $sp, 12        # delete local variables
 
  ### function epilog ###
  lw $fp, 0($sp)   
  lw $31, 4($sp)
  addi $sp, $sp, 8
  jr $31

main:
  ### function prolog ###
  addi $sp, $sp, -8
  sw $31, 4($sp)
  sw $fp, 0($sp)
  move $fp, $sp

  la $5, global           # load addr of global variable 'global'
  li $6, 1
  sw $6, 0($5)            # store 1 to 'global'

  ### setup stack for function call ###
  addi $sp, $sp, -12
  li $5, 5
  li $6, 5
  add $7, $5, $6         # sum of 5+5
  li $5, 5
  sw $5, 8($sp)           # 3rd parameter
  sw $7, 4($sp)           # 2nd parameter
  la $5, a                # load addr of global array 'a'
  sw $5, 0($sp)           # 1st parameter
  jal func                # function call to 'func'
  move $5, $2             # fetch return value
  move $2, $5             # set return value of this function

  jal scan                # call scan, no parameters
  sw $2, 0($sp)           # prepare stack for print
  jal print               # call print

  addi $sp, $sp, 12       # clean up stack after function call is done

  li $2, 0                # return value of main

  ### function epilog ###
  lw $fp, 0($sp)
  lw $31, 4($sp)
  addi $sp, $sp, 8
  jr $31
