	.text
	.file	"1.txt"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %start
	pushq	%rax
.Ltmp0:
	.cfi_def_cfa_offset 16
	movl	$100000, t(%rip)        # imm = 0x186A0
	movl	$10, n(%rip)
	cmpl	$0, t(%rip)
	je	.LBB0_4
	.align	16, 0x90
.LBB0_1:                                # %while
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	$1, result(%rip)
	movl	$0, i(%rip)
	cmpl	$0, n(%rip)
	je	.LBB0_3
	.align	16, 0x90
.LBB0_2:                                # %for
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	i(%rip), %eax
	incl	%eax
	imull	result(%rip), %eax
	movl	%eax, result(%rip)
	movl	i(%rip), %eax
	incl	%eax
	movl	%eax, i(%rip)
	cmpl	n(%rip), %eax
	jb	.LBB0_2
.LBB0_3:                                # %done1
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	result(%rip), %esi
	movl	$.L__unnamed_1, %edi
	callq	printf
	decl	t(%rip)
	jne	.LBB0_1
.LBB0_4:                                # %done
	popq	%rax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	t,@object               # @t
	.bss
	.globl	t
	.align	4
t:
	.long	0                       # 0x0
	.size	t, 4

	.type	i,@object               # @i
	.globl	i
	.align	4
i:
	.long	0                       # 0x0
	.size	i, 4

	.type	result,@object          # @result
	.globl	result
	.align	4
result:
	.long	0                       # 0x0
	.size	result, 4

	.type	n,@object               # @n
	.globl	n
	.align	4
n:
	.long	0                       # 0x0
	.size	n, 4

	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"\"result is \" %d"
	.size	.L__unnamed_1, 16


	.section	".note.GNU-stack","",@progbits
