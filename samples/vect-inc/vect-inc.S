// Directives to make interresting windows visible
#pragma qtmips show registers
#pragma qtmips show memory

.globl _start
.set noat
.set noreorder

.text

// int *pa;

_start:
main:
loop:
	la     $5, vect_a  // pa = vec_a
	addi   $8, $0, 16  // int i = 16
vect_next:                   // do {
	lw     $2, 0($5)   // r2 = *pa
	addi   $2, $2, 1   // r2 = r2 + 1
	sw     $2, 0($5) // *pa = r2

	addi   $5, $5, 4   // pa++
	addi   $8, $8, -1  // i--
	bne    $8, $0, vect_next  // } while (i != 0)
	nop

	// stop execution wait for debugger/user
	break
	// ensure that continuation does not
	// interpret random data
	beq    $0, $0, loop
	nop

.org 0x1000

.data

vect_a:	// int vect_a[16] = {1, 2, 3, ...};
	.word  0x01
	.word  0x02
	.word  0x03
	.word  0x04
	.word  0x05
	.word  0x06
	.word  0x07
	.word  0x08
	.word  0x09
	.word  0x0a
	.word  0x0b
	.word  0x0c
	.word  0x0d
	.word  0x0e
	.word  0x0f
	.word  0x00

// Specify location to show in memory window
#pragma qtmips focus memory vect_a

