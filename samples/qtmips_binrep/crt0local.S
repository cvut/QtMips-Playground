/* minimal replacement of crt0.o which is else provided by C library */

.globl main
.globl _start
.globl __start
.set noat
.set noreorder
.ent _start

.text

__start:
_start:
#if defined(__PIC__) || defined(__pic__)
	bal     next
	nop
next:
	.set    noreorder
	.cpload $31
	.set    reorder
#else
	la      $gp, _gp
#endif
	addi    $a0, $zero, 0
	addi    $a1, $zero, 0
	jal     main
	nop
quit:
	addi    $a0, $zero, 0
	addi    $v0, $zero, 4001  /* SYS_exit */

	syscall

loop:	break
        beq     $zero, $zero, loop
	nop

.end _start
