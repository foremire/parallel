	.file	"omp_mat_mul_sse_ppl_asm.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl omp_mat_mul_sse_ppl_asm
	.type	omp_mat_mul_sse_ppl_asm, @function
omp_mat_mul_sse_ppl_asm:
.LFB0:
	.file 1 "omp_mat_mul_sse_ppl_asm.c"
	.loc 1 20 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI1:
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$376, %rsp
	movq	%rsi, %rbx
	.cfi_offset 3, -24
	movq	%rcx, %rax
	movq	%r8, %rsi
	movq	%r9, %rcx
	movq	%rdi, -352(%rbp)
	movq	%rbx, -344(%rbp)
	movq	%rdx, -368(%rbp)
	movq	%rax, -360(%rbp)
	movq	%rsi, -384(%rbp)
	movq	%rcx, -376(%rbp)
	.loc 1 21 0
	movl	-344(%rbp), %eax
	movl	%eax, -20(%rbp)
	.loc 1 22 0
	movl	$0, -24(%rbp)
	.loc 1 23 0
	movl	-20(%rbp), %eax
	leal	31(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$5, %eax
	movl	%eax, -28(%rbp)
	.loc 1 26 0
	movl	-356(%rbp), %edx
	movl	-360(%rbp), %ebx
	leaq	-80(%rbp), %rax
	movl	$0, %ecx
	movl	%ebx, %esi
	movq	%rax, %rdi
	call	init_matrix
	.loc 1 28 0
	movl	$0, -32(%rbp)
	.loc 1 29 0
	movl	$0, -36(%rbp)
	.loc 1 31 0
	movl	$0, -32(%rbp)
	jmp	.L2
.L5:
	.loc 1 32 0
	movl	$0, -36(%rbp)
	jmp	.L3
.L4:
	.loc 1 33 0
	movq	-80(%rbp), %rdx
	movl	-360(%rbp), %eax
	imull	-36(%rbp), %eax
	addl	-32(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	.loc 1 34 0
	movq	-368(%rbp), %rcx
	movl	-360(%rbp), %eax
	imull	-32(%rbp), %eax
	addl	-36(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rcx,%rax), %rax
	movl	(%rax), %eax
	.loc 1 33 0
	movl	%eax, (%rdx)
	.loc 1 32 0
	addl	$1, -36(%rbp)
.L3:
	movl	-360(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jg	.L4
	.loc 1 31 0
	addl	$1, -32(%rbp)
.L2:
	movl	-356(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jg	.L5
	.loc 1 38 0
	movl	$32, %edi
	call	omp_set_num_threads
	.loc 1 40 0
	leaq	-368(%rbp), %rax
	movq	%rax, -336(%rbp)
	leaq	-352(%rbp), %rax
	movq	%rax, -328(%rbp)
	leaq	-80(%rbp), %rax
	movq	%rax, -320(%rbp)
	leaq	-384(%rbp), %rax
	movq	%rax, -312(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -304(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, -300(%rbp)
	leaq	-336(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$omp_mat_mul_sse_ppl_asm.omp_fn.0, %edi
	call	GOMP_parallel_start
	leaq	-336(%rbp), %rax
	movq	%rax, %rdi
	call	omp_mat_mul_sse_ppl_asm.omp_fn.0
	call	GOMP_parallel_end
	movl	-304(%rbp), %eax
	movl	%eax, -20(%rbp)
	movl	-300(%rbp), %eax
	movl	%eax, -28(%rbp)
	.loc 1 115 0
	movq	-80(%rbp), %rax
	testq	%rax, %rax
	je	.L7
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	$0, -80(%rbp)
.L7:
	.loc 1 116 0
	addq	$376, %rsp
	popq	%rbx
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	omp_mat_mul_sse_ppl_asm, .-omp_mat_mul_sse_ppl_asm
	.type	omp_mat_mul_sse_ppl_asm.omp_fn.0, @function
omp_mat_mul_sse_ppl_asm.omp_fn.0:
.LFB1:
	.loc 1 40 0
	.cfi_startproc
	pushq	%rbp
.LCFI2:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI3:
	.cfi_def_cfa_register 6
	subq	$256, %rsp
	movq	%rdi, -248(%rbp)
.LBB2:
	.loc 1 43 0
	call	omp_get_thread_num
	movl	%eax, -4(%rbp)
	.loc 1 44 0
	movq	-248(%rbp), %rax
	movl	36(%rax), %eax
	imull	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
	.loc 1 45 0
	movq	-248(%rbp), %rax
	movl	36(%rax), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	.loc 1 48 0
	movl	$0, -16(%rbp)
	.loc 1 49 0
	movl	$0, -20(%rbp)
	.loc 1 50 0
	movl	$0, -24(%rbp)
	.loc 1 69 0
	movl	-8(%rbp), %eax
	movl	%eax, -16(%rbp)
.L11:
	movl	-16(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.L18
.L9:
	.loc 1 70 0
	movl	$0, -20(%rbp)
.L14:
	movq	-248(%rbp), %rax
	movq	(%rax), %rax
	movl	8(%rax), %eax
	subl	$3, %eax
	cmpl	-20(%rbp), %eax
	jg	.L10
	.loc 1 69 0
	addl	$1, -16(%rbp)
	jmp	.L11
.L10:
	.loc 1 72 0
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -48(%rbp)
	movhps	%xmm0, -40(%rbp)
	.loc 1 73 0
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -64(%rbp)
	movhps	%xmm0, -56(%rbp)
	.loc 1 74 0
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -80(%rbp)
	movhps	%xmm0, -72(%rbp)
	.loc 1 75 0
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -96(%rbp)
	movhps	%xmm0, -88(%rbp)
	.loc 1 77 0
	movl	$0, -24(%rbp)
.L16:
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	subl	$3, %eax
	cmpl	-24(%rbp), %eax
	jg	.L12
	.loc 1 96 0
	leaq	-112(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-48(%rbp), %xmm0
	movhps	-40(%rbp), %xmm0
	movups	%xmm0, (%rax)
	.loc 1 97 0
	leaq	-128(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-64(%rbp), %xmm0
	movhps	-56(%rbp), %xmm0
	movups	%xmm0, (%rax)
	.loc 1 98 0
	leaq	-144(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-80(%rbp), %xmm0
	movhps	-72(%rbp), %xmm0
	movups	%xmm0, (%rax)
	.loc 1 99 0
	leaq	-160(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-96(%rbp), %xmm0
	movhps	-88(%rbp), %xmm0
	movups	%xmm0, (%rax)
	.loc 1 101 0
	movl	$1, -24(%rbp)
.L15:
	cmpl	$3, -24(%rbp)
	jle	.L13
	.loc 1 107 0
	movq	-248(%rbp), %rax
	movq	24(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-20(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-112(%rbp), %eax
	movl	%eax, (%rdx)
	.loc 1 108 0
	movq	-248(%rbp), %rax
	movq	24(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-20(%rbp), %eax
	cltq
	addq	$1, %rax
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-128(%rbp), %eax
	movl	%eax, (%rdx)
	.loc 1 109 0
	movq	-248(%rbp), %rax
	movq	24(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-20(%rbp), %eax
	cltq
	addq	$2, %rax
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-144(%rbp), %eax
	movl	%eax, (%rdx)
	.loc 1 110 0
	movq	-248(%rbp), %rax
	movq	24(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-20(%rbp), %eax
	cltq
	addq	$3, %rax
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-160(%rbp), %eax
	movl	%eax, (%rdx)
	.loc 1 70 0
	addl	$4, -20(%rbp)
	jmp	.L14
.L13:
	.loc 1 102 0
	movss	-112(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-112(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -112(%rbp)
	.loc 1 103 0
	movss	-128(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-128(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -128(%rbp)
	.loc 1 104 0
	movss	-144(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-144(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -144(%rbp)
	.loc 1 105 0
	movss	-160(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-160(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -160(%rbp)
	.loc 1 101 0
	addl	$1, -24(%rbp)
	jmp	.L15
.L12:
	.loc 1 78 0
	movq	-248(%rbp), %rax
	movq	8(%rax), %rax
	movq	(%rax), %rdx
	.loc 1 79 0
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-24(%rbp), %eax
	.loc 1 78 0
	cltq
	.loc 1 79 0
	salq	$2, %rax
	.loc 1 78 0
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -176(%rbp)
	movhps	%xmm0, -168(%rbp)
	.loc 1 81 0
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	.loc 1 82 0
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-20(%rbp), %eax
	addl	-24(%rbp), %eax
	.loc 1 81 0
	cltq
	.loc 1 82 0
	salq	$2, %rax
	.loc 1 81 0
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -192(%rbp)
	movhps	%xmm0, -184(%rbp)
	.loc 1 83 0
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	.loc 1 84 0
	movl	-20(%rbp), %eax
	leal	1(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	.loc 1 83 0
	cltq
	.loc 1 84 0
	salq	$2, %rax
	.loc 1 83 0
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -208(%rbp)
	movhps	%xmm0, -200(%rbp)
	.loc 1 85 0
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	.loc 1 86 0
	movl	-20(%rbp), %eax
	leal	2(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	.loc 1 85 0
	cltq
	.loc 1 86 0
	salq	$2, %rax
	.loc 1 85 0
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -224(%rbp)
	movhps	%xmm0, -216(%rbp)
	.loc 1 87 0
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	.loc 1 88 0
	movl	-20(%rbp), %eax
	leal	3(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	.loc 1 87 0
	cltq
	.loc 1 88 0
	salq	$2, %rax
	.loc 1 87 0
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -240(%rbp)
	movhps	%xmm0, -232(%rbp)
	.loc 1 90 0
	xorps	%xmm0, %xmm0
	movlps	-192(%rbp), %xmm0
	movhps	-184(%rbp), %xmm0
	xorps	%xmm1, %xmm1
	movlps	-176(%rbp), %xmm1
	movhps	-168(%rbp), %xmm1
	mulps	%xmm1, %xmm0
	addps	-48(%rbp), %xmm0
	movlps	%xmm0, -48(%rbp)
	movhps	%xmm0, -40(%rbp)
	.loc 1 91 0
	xorps	%xmm0, %xmm0
	movlps	-208(%rbp), %xmm0
	movhps	-200(%rbp), %xmm0
	xorps	%xmm1, %xmm1
	movlps	-176(%rbp), %xmm1
	movhps	-168(%rbp), %xmm1
	mulps	%xmm1, %xmm0
	addps	-64(%rbp), %xmm0
	movlps	%xmm0, -64(%rbp)
	movhps	%xmm0, -56(%rbp)
	.loc 1 92 0
	xorps	%xmm0, %xmm0
	movlps	-224(%rbp), %xmm0
	movhps	-216(%rbp), %xmm0
	xorps	%xmm1, %xmm1
	movlps	-176(%rbp), %xmm1
	movhps	-168(%rbp), %xmm1
	mulps	%xmm1, %xmm0
	addps	-80(%rbp), %xmm0
	movlps	%xmm0, -80(%rbp)
	movhps	%xmm0, -72(%rbp)
	.loc 1 93 0
	xorps	%xmm0, %xmm0
	movlps	-240(%rbp), %xmm0
	movhps	-232(%rbp), %xmm0
	xorps	%xmm1, %xmm1
	movlps	-176(%rbp), %xmm1
	movhps	-168(%rbp), %xmm1
	mulps	%xmm1, %xmm0
	addps	-96(%rbp), %xmm0
	movlps	%xmm0, -96(%rbp)
	movhps	%xmm0, -88(%rbp)
	.loc 1 77 0
	addl	$4, -24(%rbp)
	jmp	.L16
.L18:
.LBE2:
	.loc 1 40 0
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	omp_mat_mul_sse_ppl_asm.omp_fn.0, .-omp_mat_mul_sse_ppl_asm.omp_fn.0
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LFB0-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI1-.Ltext0
	.quad	.LFE0-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST1:
	.quad	.LFB1-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI3-.Ltext0
	.quad	.LFE1-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
	.file 2 "mm.h"
	.section	.debug_info
	.long	0x3e7
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF41
	.byte	0x1
	.long	.LASF42
	.long	.LASF43
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.byte	0x8
	.byte	0x7
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF9
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF10
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF11
	.uleb128 0x5
	.long	.LASF15
	.byte	0x2
	.byte	0x17
	.long	0x7d
	.uleb128 0x6
	.long	.LASF44
	.byte	0x10
	.byte	0x2
	.byte	0x1a
	.long	0xcd
	.uleb128 0x7
	.long	.LASF12
	.byte	0x2
	.byte	0x1b
	.long	0xcd
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x7
	.long	.LASF13
	.byte	0x2
	.byte	0x1c
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x7
	.long	.LASF14
	.byte	0x2
	.byte	0x1d
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0x0
	.uleb128 0x8
	.byte	0x8
	.long	0x8b
	.uleb128 0x5
	.long	.LASF16
	.byte	0x2
	.byte	0x1e
	.long	0x96
	.uleb128 0x5
	.long	.LASF17
	.byte	0x2
	.byte	0x21
	.long	0xe9
	.uleb128 0x9
	.byte	0x1
	.long	0x7d
	.long	0xfa
	.uleb128 0xa
	.long	0x65
	.byte	0x3
	.byte	0x0
	.uleb128 0xb
	.byte	0x1
	.long	.LASF45
	.byte	0x1
	.byte	0x14
	.byte	0x1
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0x19e
	.uleb128 0xc
	.long	.LASF18
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x3
	.byte	0x91
	.sleb128 -368
	.uleb128 0xc
	.long	.LASF19
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x3
	.byte	0x91
	.sleb128 -384
	.uleb128 0xc
	.long	.LASF20
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x3
	.byte	0x91
	.sleb128 -400
	.uleb128 0xd
	.string	"dim"
	.byte	0x1
	.byte	0x15
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0xe
	.long	.LASF21
	.byte	0x1
	.byte	0x16
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xe
	.long	.LASF22
	.byte	0x1
	.byte	0x17
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xe
	.long	.LASF23
	.byte	0x1
	.byte	0x19
	.long	0xd3
	.byte	0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0xe
	.long	.LASF24
	.byte	0x1
	.byte	0x1c
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xe
	.long	.LASF25
	.byte	0x1
	.byte	0x1d
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -52
	.byte	0x0
	.uleb128 0xf
	.long	.LASF46
	.byte	0x1
	.byte	0x1
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
	.long	0x362
	.uleb128 0x10
	.long	.LASF47
	.long	0x3c7
	.byte	0x1
	.byte	0x3
	.byte	0x91
	.sleb128 -264
	.uleb128 0xe
	.long	.LASF21
	.byte	0x1
	.byte	0x16
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0xe
	.long	.LASF22
	.byte	0x1
	.byte	0x17
	.long	0x57
	.byte	0x6
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x23
	.uleb128 0x24
	.uleb128 0xe
	.long	.LASF20
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x7
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x23
	.uleb128 0x18
	.byte	0x6
	.uleb128 0xe
	.long	.LASF23
	.byte	0x1
	.byte	0x19
	.long	0xd3
	.byte	0x7
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x23
	.uleb128 0x10
	.byte	0x6
	.uleb128 0xe
	.long	.LASF18
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x7
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x23
	.uleb128 0x8
	.byte	0x6
	.uleb128 0xd
	.string	"dim"
	.byte	0x1
	.byte	0x15
	.long	0x57
	.byte	0x6
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x23
	.uleb128 0x20
	.uleb128 0xe
	.long	.LASF19
	.byte	0x1
	.byte	0x14
	.long	0xd3
	.byte	0x5
	.byte	0x91
	.sleb128 -264
	.byte	0x6
	.byte	0x6
	.uleb128 0x11
	.quad	.LBB2
	.quad	.LBE2
	.uleb128 0xe
	.long	.LASF26
	.byte	0x1
	.byte	0x2c
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xd
	.string	"end"
	.byte	0x1
	.byte	0x2d
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xe
	.long	.LASF24
	.byte	0x1
	.byte	0x30
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xe
	.long	.LASF25
	.byte	0x1
	.byte	0x31
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0xe
	.long	.LASF27
	.byte	0x1
	.byte	0x32
	.long	0x57
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xe
	.long	.LASF28
	.byte	0x1
	.byte	0x34
	.long	0xde
	.byte	0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xe
	.long	.LASF29
	.byte	0x1
	.byte	0x35
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0xe
	.long	.LASF30
	.byte	0x1
	.byte	0x36
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0xe
	.long	.LASF31
	.byte	0x1
	.byte	0x37
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0xe
	.long	.LASF32
	.byte	0x1
	.byte	0x39
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -192
	.uleb128 0xe
	.long	.LASF33
	.byte	0x1
	.byte	0x3b
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -208
	.uleb128 0xe
	.long	.LASF34
	.byte	0x1
	.byte	0x3c
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -224
	.uleb128 0xe
	.long	.LASF35
	.byte	0x1
	.byte	0x3d
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -240
	.uleb128 0xe
	.long	.LASF36
	.byte	0x1
	.byte	0x3e
	.long	0xde
	.byte	0x3
	.byte	0x91
	.sleb128 -256
	.uleb128 0xe
	.long	.LASF37
	.byte	0x1
	.byte	0x40
	.long	0x3cd
	.byte	0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0xe
	.long	.LASF38
	.byte	0x1
	.byte	0x41
	.long	0x3cd
	.byte	0x3
	.byte	0x91
	.sleb128 -144
	.uleb128 0xe
	.long	.LASF39
	.byte	0x1
	.byte	0x42
	.long	0x3cd
	.byte	0x3
	.byte	0x91
	.sleb128 -160
	.uleb128 0xe
	.long	.LASF40
	.byte	0x1
	.byte	0x43
	.long	0x3cd
	.byte	0x3
	.byte	0x91
	.sleb128 -176
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.long	.LASF48
	.byte	0x28
	.long	0x3c1
	.uleb128 0x7
	.long	.LASF19
	.byte	0x1
	.byte	0x28
	.long	0x3c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x7
	.long	.LASF18
	.byte	0x1
	.byte	0x28
	.long	0x3c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x7
	.long	.LASF23
	.byte	0x1
	.byte	0x28
	.long	0x3c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x7
	.long	.LASF20
	.byte	0x1
	.byte	0x28
	.long	0x3c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x13
	.string	"dim"
	.byte	0x1
	.byte	0x28
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x7
	.long	.LASF22
	.byte	0x1
	.byte	0x28
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x8
	.byte	0x8
	.long	0xd3
	.uleb128 0x8
	.byte	0x8
	.long	0x362
	.uleb128 0x14
	.long	0x8b
	.long	0x3dd
	.uleb128 0xa
	.long	0x65
	.byte	0x3
	.byte	0x0
	.uleb128 0x15
	.long	.LASF49
	.byte	0x1
	.byte	0xb
	.long	0x3cd
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x2107
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x2a
	.value	0x2
	.long	.Ldebug_info0
	.long	0x3eb
	.long	0xfa
	.string	"omp_mat_mul_sse_ppl_asm"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0x0
	.value	0x0
	.value	0x0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF8:
	.string	"long long int"
.LASF32:
	.string	"oprand_a"
.LASF12:
	.string	"data"
.LASF10:
	.string	"float"
.LASF26:
	.string	"start"
.LASF2:
	.string	"short unsigned int"
.LASF45:
	.string	"omp_mat_mul_sse_ppl_asm"
.LASF44:
	.string	"_matrix"
.LASF11:
	.string	"double"
.LASF18:
	.string	"matrixA"
.LASF19:
	.string	"matrixB"
.LASF20:
	.string	"matrixC"
.LASF34:
	.string	"oprand_b_1"
.LASF15:
	.string	"ftype"
.LASF36:
	.string	"oprand_b_3"
.LASF24:
	.string	"cycleI"
.LASF25:
	.string	"cycleJ"
.LASF27:
	.string	"cycleK"
.LASF1:
	.string	"unsigned char"
.LASF23:
	.string	"matrixBT"
.LASF0:
	.string	"long unsigned int"
.LASF49:
	.string	"init_array"
.LASF28:
	.string	"acc_0"
.LASF29:
	.string	"acc_1"
.LASF30:
	.string	"acc_2"
.LASF31:
	.string	"acc_3"
.LASF42:
	.string	"omp_mat_mul_sse_ppl_asm.c"
.LASF43:
	.string	"/home/guest07/parallel/project/src"
.LASF48:
	.string	".omp_data_s.14"
.LASF16:
	.string	"matrix"
.LASF3:
	.string	"unsigned int"
.LASF47:
	.string	".omp_data_i"
.LASF7:
	.string	"char"
.LASF37:
	.string	"imd_ret_0"
.LASF38:
	.string	"imd_ret_1"
.LASF39:
	.string	"imd_ret_2"
.LASF40:
	.string	"imd_ret_3"
.LASF13:
	.string	"xDim"
.LASF4:
	.string	"signed char"
.LASF41:
	.string	"GNU C 4.4.1"
.LASF33:
	.string	"oprand_b_0"
.LASF46:
	.string	"omp_mat_mul_sse_ppl_asm.omp_fn.0"
.LASF35:
	.string	"oprand_b_2"
.LASF5:
	.string	"short int"
.LASF14:
	.string	"yDim"
.LASF6:
	.string	"long int"
.LASF17:
	.string	"v4sf"
.LASF22:
	.string	"num_per_thread"
.LASF21:
	.string	"thread_id"
.LASF9:
	.string	"long long unsigned int"
	.ident	"GCC: (Ubuntu 4.4.1-4ubuntu9) 4.4.1"
	.section	.note.GNU-stack,"",@progbits
