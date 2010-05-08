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
# loop variable: i: %r15d
# loop variable: j: %r14d
# prefixes: l for 4 bytes, q for 8 bytes.
..B1.1:                         # Preds ..B1.0
..___tag_value_asmloop.1:                                       #52.1
  # push used registers into stack
  pushq %r15
  pushq %r14
  pushq %r13
  pushq %r12
  pushq %r11

  #initialization
  sub $4, %esi
  movl $0, %r15d  # i = 0

  ############### The outermost loop #############
..Branch.i:
    ############# The inner loop ##################
    #Initialize j to 1
    movl $1, %r14d
    movl $2, %r13d
    movl $3, %r12d
    movl $4, %r11d
	
    # preload a[0]
    movq -8(%rdi,%r14,8), %xmm0
..Branch.j:
      ############# Code inside the loops ############
      #for ( i = 0; i < t; i++ )
      #{
      #  for ( j = 1; j < N-1; j++ )
      #  {
      #    a[ j ] = ( a[ j - 1 ] + a[ j ] + j ) * 0.43;
      #  }
      #}
      
      # Convert j to double
      cvtsi2sd %r14, %xmm14
      cvtsi2sd %r13, %xmm13
      cvtsi2sd %r12, %xmm12
      cvtsi2sd %r11, %xmm11

      # load the variables
      movq 0(%rdi,%r14,8), %xmm1
      movq 8(%rdi,%r14,8), %xmm2
      movq 16(%rdi,%r14,8), %xmm3
      movq 24(%rdi,%r14,8), %xmm4

      addsd %xmm14, %xmm1
      addsd %xmm13, %xmm2
      addsd %xmm12, %xmm3
      addsd %xmm11, %xmm4

      # calculate the first result
      addsd %xmm0, %xmm1
      mulsd 8(%rcx), %xmm2
      mulsd 8(%rcx), %xmm1
      movsd %xmm1, 0(%rdi,%r14,8)
      
      # calcluate the second result
      mulsd 8(%rcx), %xmm1
      mulsd 8(%rcx), %xmm3
      addsd %xmm1, %xmm2
      movsd %xmm2, 8(%rdi,%r14,8)
      
      # calcluate the third result
      mulsd 8(%rcx), %xmm2
      mulsd 8(%rcx), %xmm4
      addsd %xmm2, %xmm3
      movsd %xmm3, 16(%rdi,%r14,8)
		
      # calcluate the second result
      mulsd 8(%rcx), %xmm3
      addsd %xmm3, %xmm4
      movsd %xmm4, 24(%rdi,%r14,8)

      # reuse the last result as the first value in the next ineration
      movsd %xmm4, %xmm0
		
      ############## End code inside loops ###########
      # Branch for j if needed
      lea 5(%r14d), %r13d
      lea 6(%r14d), %r12d
      lea 7(%r14d), %r11d

      addl $4, %r14d
      cmpl %esi, %r14d
      jl ..Branch.j

    # See if a branch is required
    incl %r15d
    cmpl %r15d, %edx
    jne ..Branch.i

  ################ Return ############################		        
  movq $0, %rax
  # pop registers out of stack
  popq %r11
  popq %r12
  popq %r13
  popq %r14
  popq %r15
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
