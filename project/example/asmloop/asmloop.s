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
# prefixes: l for 4 bytes, q for 8 bytes.
..B1.1:                         # Preds ..B1.0
..___tag_value_asmloop.1:                                       #52.1
  #initialization
  sub $1, %esi
  movl $0, %r14d  # i = 0

  ############### The outermost loop #############
..Branch.i:
    ############# The inner loop ##################
    #Initialize j to 1
    movl $1, %r15d
		
..Branch.j:
      ############# Code inside the loops ############
      #for ( i = 0; i < t; i++ )
      #{
      #  for ( j = 1; j < N-1; j++ )
      #  {
      #    a[ j ] = ( a[ j - 1 ] + a[ j ] + j ) * 0.43;
      #  }
      #}
      
      # load the variables
      movq 0(%rdi,%r15,8), %xmm0
      movq -8(%rdi,%r15,8), %xmm1
      
      # Convert j to double
      cvtsi2sd %r15, %xmm2
     
      # calculation
      addsd %xmm1, %xmm0
      addsd %xmm2, %xmm0
      mulsd 8(%rcx), %xmm0
		
      # store the result
      movsd %xmm0, 0(%rdi,%r15,8)
		
      ############## End code inside loops ###########
      # Branch for j if needed
      incl %r15d
      cmpl %r15d, %esi
      jne ..Branch.j

    # See if a branch is required
    incl %r14d
    cmpl %r14d, %edx
    jne ..Branch.i

  ################ Return ############################		        
  movq $0, %rax
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
