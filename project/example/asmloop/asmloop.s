# -- Machine type EFI2
# mark_description "Intel(R) C++ Compiler for applications running on Intel(R) 64, Version 11.1    Build 20100203 %s";
# mark_description "-S";
	.file "asmgenerator.c"
	.text
..TXTST0:
# -- Begin  asmloop
# mark_begin;
       .align    16,0x90
	.globl asmloop
asmloop:
# parameter 1: %rdi a[]
# parameter 2: %esi N
# parameter 3: %edx t
# parameter 4: %rcx --> This one is k, k contains: 1.0, 0.43 and 8.5
# loop variable: i: %r14d
# loop variable: j: %r15d
# a[ j ]: %r13
# prefixes: l for 4 bytes, q for 8 bytes.
..B1.1:                         # Preds ..B1.0
..___tag_value_asmloop.1:                                       #52.1
        
        
        ############### The outermost loop #############
        movl  $0, %r14d  # i = 0
        # The tag for the jump
..Branch.i:
		
		############# The inner loop ##################
		#Initialize j to 1
		movl $1, %r15d
		
		#Initialize a[ j ] to a[ 1 ]:
                leaq 8(%rdi), %r13
		
		#The tag for the jump
..Branch.j:

		############# Code inside the loops ############
		#for ( i = 0; i < t; i++ )
		#{
		#	for ( j = 1; j < N-1; j++ )
		#	{
		#		a[ j ] = ( a[ j - 1 ] + a[ j ] + j ) * 0.43;
		#	}
		#}
		
		# Add the values into %r9
		movq 0(%r13), %xmm0
		movq -8(%r13), %xmm1
		
                #xmm0 contains ( a[j] + a[j-1] )
		addsd %xmm1, %xmm0
		
		# Add j: First it has to ve converted to double
		cvtsi2sd %r15, %xmm1
		#xmm0 contains ( a[j] + a[j-1] + (double) j )
		addsd %xmm1, %xmm0
		
		#Multiply by 0.43: Multiplication happens from xmm
		movq 8(%rcx), %xmm1

		#xmm0 contains ( a[j] + a[j-1] + (double) j ) * 0.43
		mulsd %xmm1, %xmm0
		
		#Store the result
		movsd %xmm0, 0(%r13)
		
		# Get &a[ j + 1 ], that will be used in the next iteration.
		addq $8, %r13
		
		############## End code inside loops ###########
		
		# Increment j
		incl %r15d
		
		# Branch for j if needed
		cmpl %r15d, %esi
		movq %r15, %rax
		jne ..Branch.j

		# Increment i
		incl %r14d
		#See if a branch is required
		cmpl %r14d, %edx
		# Branch
		jne ..Branch.i

		################ Return ############################		        
        ret                                                     #59.7
        .align    16,0x90
..___tag_value_asmloop.2:                                       #
                                # LOE
# mark_end;
	.type	asmloop,@function
	.size	asmloop,.-asmloop
	.data
# -- End  asmloop
	.section .rodata, "a"
	.align 8
	.align 8
_2il0floatpacket.4:
	.long	0x33333333,0x400b3333
	.type	_2il0floatpacket.4,@object
	.size	_2il0floatpacket.4,8
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .debug_line
	.section .debug_line
.debug_line_seg:
	.align 1
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
	.4byte 0x00000014
	.4byte 0x00000000
	.byte 0x01
	.byte 0x00
	.byte 0x01
	.byte 0x78
	.byte 0x10
	.4byte 0x9008070c
	.byte 0x01
	.4byte 0x00000000
	.2byte 0x0000
	.4byte 0x00000014
	.4byte 0x0000001c
	.8byte ..___tag_value_asmloop.1
	.8byte ..___tag_value_asmloop.2-..___tag_value_asmloop.1
# End
