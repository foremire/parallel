	.file	"omp_mat_mul_sse_ppl_asm.c"
	.text
.globl omp_mat_mul_sse_ppl_asm
	.type	omp_mat_mul_sse_ppl_asm, @function
omp_mat_mul_sse_ppl_asm:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
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
	movl	-344(%rbp), %eax
	movl	%eax, -20(%rbp)
	movl	$0, -24(%rbp)
	movl	-20(%rbp), %eax
	leal	31(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$5, %eax
	movl	%eax, -28(%rbp)
	movl	-356(%rbp), %edx
	movl	-360(%rbp), %ebx
	leaq	-80(%rbp), %rax
	movl	$0, %ecx
	movl	%ebx, %esi
	movq	%rax, %rdi
	call	init_matrix
	movl	$0, -32(%rbp)
	movl	$0, -36(%rbp)
	movl	$0, -32(%rbp)
	jmp	.L2
.L5:
	movl	$0, -36(%rbp)
	jmp	.L3
.L4:
	movq	-80(%rbp), %rdx
	movl	-360(%rbp), %eax
	imull	-36(%rbp), %eax
	addl	-32(%rbp), %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movq	-368(%rbp), %rcx
	movl	-360(%rbp), %eax
	imull	-32(%rbp), %eax
	addl	-36(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rcx,%rax), %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	addl	$1, -36(%rbp)
.L3:
	movl	-360(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jg	.L4
	addl	$1, -32(%rbp)
.L2:
	movl	-356(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jg	.L5
	movl	$32, %edi
	call	omp_set_num_threads
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
	movq	-80(%rbp), %rax
	testq	%rax, %rax
	je	.L7
	movq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	$0, -80(%rbp)
.L7:
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
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	subq	$256, %rsp
	movq	%rdi, -248(%rbp)
	call	omp_get_thread_num
	movl	%eax, -4(%rbp)
	movq	-248(%rbp), %rax
	movl	36(%rax), %eax
	imull	-4(%rbp), %eax
	movl	%eax, -8(%rbp)
	movq	-248(%rbp), %rax
	movl	36(%rax), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	$0, -16(%rbp)
	movl	$0, -20(%rbp)
	movl	$0, -24(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -16(%rbp)
.L11:
	movl	-16(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jge	.L18
.L9:
	movl	$0, -20(%rbp)
.L14:
	movq	-248(%rbp), %rax
	movq	(%rax), %rax
	movl	8(%rax), %eax
	subl	$3, %eax
	cmpl	-20(%rbp), %eax
	jg	.L10
	addl	$1, -16(%rbp)
	jmp	.L11
.L10:
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -48(%rbp)
	movhps	%xmm0, -40(%rbp)
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -64(%rbp)
	movhps	%xmm0, -56(%rbp)
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -80(%rbp)
	movhps	%xmm0, -72(%rbp)
	movl	$init_array, %eax
	movups	(%rax), %xmm0
	movlps	%xmm0, -96(%rbp)
	movhps	%xmm0, -88(%rbp)
	movl	$0, -24(%rbp)
.L16:
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	subl	$3, %eax
	cmpl	-24(%rbp), %eax
	jg	.L12
	leaq	-112(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-48(%rbp), %xmm0
	movhps	-40(%rbp), %xmm0
	movups	%xmm0, (%rax)
	leaq	-128(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-64(%rbp), %xmm0
	movhps	-56(%rbp), %xmm0
	movups	%xmm0, (%rax)
	leaq	-144(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-80(%rbp), %xmm0
	movhps	-72(%rbp), %xmm0
	movups	%xmm0, (%rax)
	leaq	-160(%rbp), %rax
	xorps	%xmm0, %xmm0
	movlps	-96(%rbp), %xmm0
	movhps	-88(%rbp), %xmm0
	movups	%xmm0, (%rax)
	movl	$1, -24(%rbp)
.L15:
	cmpl	$3, -24(%rbp)
	jle	.L13
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
	addl	$4, -20(%rbp)
	jmp	.L14
.L13:
	movss	-112(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-112(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -112(%rbp)
	movss	-128(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-128(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -128(%rbp)
	movss	-144(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-144(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -144(%rbp)
	movss	-160(%rbp), %xmm1
	movl	-24(%rbp), %eax
	cltq
	movss	-160(%rbp,%rax,4), %xmm0
	addss	%xmm1, %xmm0
	movss	%xmm0, -160(%rbp)
	addl	$1, -24(%rbp)
	jmp	.L15
.L12:
	movq	-248(%rbp), %rax
	movq	8(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-16(%rbp), %eax
	addl	-24(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -176(%rbp)
	movhps	%xmm0, -168(%rbp)
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	-20(%rbp), %eax
	addl	-24(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -192(%rbp)
	movhps	%xmm0, -184(%rbp)
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	movl	-20(%rbp), %eax
	leal	1(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -208(%rbp)
	movhps	%xmm0, -200(%rbp)
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	movl	-20(%rbp), %eax
	leal	2(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -224(%rbp)
	movhps	%xmm0, -216(%rbp)
	movq	-248(%rbp), %rax
	movq	16(%rax), %rax
	movq	(%rax), %rdx
	movl	-20(%rbp), %eax
	leal	3(%rax), %ecx
	movq	-248(%rbp), %rax
	movl	32(%rax), %eax
	imull	%ecx, %eax
	addl	-24(%rbp), %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rax
	movups	(%rax), %xmm0
	movlps	%xmm0, -240(%rbp)
	movhps	%xmm0, -232(%rbp)
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
	addl	$4, -24(%rbp)
	jmp	.L16
.L18:
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	omp_mat_mul_sse_ppl_asm.omp_fn.0, .-omp_mat_mul_sse_ppl_asm.omp_fn.0
	.ident	"GCC: (Ubuntu 4.4.1-4ubuntu9) 4.4.1"
	.section	.note.GNU-stack,"",@progbits
