// Simple sorting algorithm - selection sort

// Select the CPU core configuration with delay-slot

// Directives to make interresting windows visible
#pragma qtmips show registers
#pragma qtmips show memory

.set noreorder
.set noat

.globl    array

.text
.globl _start
.ent _start

_start:

la   $a0, array
addi $s0, $0, 0  //Minimum value from the rest of the array will be placed here. (Offset in the array, increasing by 4 bytes).
addi $s1, $0, 60 // Maximal index/offset value. Used for cycle termination = number of values in array * 4.
add  $s2, $0, $s0 //Working position (offset)
// $s3 - offset of the smallest value found so far in given run
// $s4 - value of the smallest value found so far in  given run
// $s5 - temporary

main_cycle:
	beq  $s0, $s1, main_cycle_end
	nop

	add  $at, $a0, $s0
	lw   $s4, 0($at)   // lw  $s4, array($s0)
	add  $s3, $s0, $0
	add  $s2, $s0, $0
	
inner_cycle:
	beq  $s2, $s1, inner_cycle_end
	nop
		add  $at, $a0, $s2
		lw   $s5, 0($at) // lw $s5, array($s2)
		
		// expand bgt $s5, $s4, not_minimum
		slt  $at, $s4, $s5
		bne  $at, $zero, not_minimum
		nop 
		 
			addi $s3, $s2, 0
			addi $s4, $s5, 0
not_minimum:
		addi $s2, $s2, 4
		j inner_cycle
		nop
inner_cycle_end:
	add  $at, $a0, $s0
	lw   $s5, 0($at)  // lw $s5, array($s0)
	sw   $s4, 0($at)  // sw $s4, array($s0)
	add  $at, $a0, $s3
	sw   $s5, 0($at)  // sw $s5, array($s3)
	
	addi $s0, $s0, 4
	j main_cycle
	nop
main_cycle_end:

//Final infinite loop
end_loop:
	cache 9, 0($0)  // flush cache memory
	break           // stop the simulator
	j end_loop
	nop

.end _start

.data
// .align    2 // not supported by QtMips

array:
.word    5, 3, 4, 1, 15, 8, 9, 2, 10, 6, 11, 1, 6, 9, 12

// Specify location to show in memory window
#pragma qtmips focus memory array
