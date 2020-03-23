.globl _start
.set noat
.set noreorder

.text

_start:
main:
	la     $4, vect_a
	// load the word from absolute address
	lw     $2, 0x2000($0)
	// store the word to absolute address
	sw     $2, 0x2004($0)

loop:
	// stop execution wait for debugger/user
	break
	// ensure that continuation does not
	// interpret random data
	beq    $0, $0, loop
	nop

.data

vect_a:
	.word  0x12345678
	.word  0
	.word  0
	.word  0
vect_b:
	.word  0
