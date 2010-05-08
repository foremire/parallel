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
		# Initialize j to 1
		movl $1, %r15d
		
		##### Initialize a[ j ] to a[ 1 ]:
		movq %rdi, %r13  # This is a[ 0 ]
		# Set tmp = 8
		movq  $8, %r9 # now r9 = 8
		
		#add &a[ j ] + tmp
		addq %r9, %r13 # now r13 is a[1]
		### end init a[j] to a[1] ####
		
		
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
		
		
		
		# Get temporary values in %r9 and %r10
		movq 0(%r13), %r9  #This is a[ j ]
		movq -8(%r13), %r10  #This is a[ j - 1 ]
		
		# Add the values into %r9
		movq %r9, %xmm0
		movq %r10, %xmm1
		addsd %xmm1, %xmm0
		movq %xmm0, %r9
		
		#At this point r9 and xmm0 contain a[j] + a[j-1]
		
		# Add j: First it has to ve converted to double
		cvtsi2sd %r15, %xmm1
		
		
		#Add
		addsd %xmm1, %xmm0
		movq %xmm0, %r9
		
		#At this point r9 and xmm0 contain (a[j] + a[j-1] + (double) j )
		
		#Multiply by 0.43: Multiplication happens from xmm
		movq 8(%rcx), %xmm1
		
		mulsd %xmm1, %xmm0
		movq %xmm0, %r9
		
		# At this point the multiplication was made, and it is ready in xmm0 and r9
		
		#Store the result
		
		#movsd %xmm0, 0(%r13)
		movq %r9, 0(%r13)
		
		# Get &a[ j + 1 ], that will be used in the next iteration.
		# Set tmp = 8
		movq  $8, %r9
		
		#add &a[ j ] + tmp
		addq %r9, %r13
		
		
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
