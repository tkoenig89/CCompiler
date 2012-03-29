.data
.global:
	.word n	align 4
.a:
	.word n	align 4
.b:
	.word n	align 4
.c:
	.word n	align 4
.d:
	.word n	align 4
.e:
	.word n	align 4
.f:
	.word n	align 4
.g:
	.word n	align 4
.h:
	.word n	align 4
.arr1:
	.word 0 : 10
	align 4
.arr2:
	.word 0 : 1337
	align 4


.text

_start:
	JAL main

	main:	# Beggining of a function. We will save the return adress $31 and the $fp.
	ADDI $sp, $sp, -8
	SW $31, 4($sp)
	SW $fp, 0($sp)
	MOVE $fp, $sp

#Error: Unrecognised command 20.
#Error: Unrecognised command 22.
#Error: Unrecognised command 23.
#Error: Unrecognised command 1.
#Error: Unrecognised command 15.
#Error: Unrecognised command 16.
	#End of function main. We will restore the return adress $31 and the $fp. Then we will jump back to where the func was called.
	LW $fp, 0($sp)
	LW $31, 4($sp)
	ADDI $sp, $sp, 8
	JR $31
