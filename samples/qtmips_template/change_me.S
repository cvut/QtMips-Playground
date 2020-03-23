/* template for own QtMips program development */

.globl _start
.globl __start
.set noat
.set noreorder
.ent _start

.text

__start:
_start:
	addi    $a0, $zero, 10
	la	$a1, var1
	lw	$v0, 0($a1)

	/* space for your code */

loop:	break
        beq     $zero, $zero, loop
	nop

.end _start

.data

var1:	.word 0x12345678
