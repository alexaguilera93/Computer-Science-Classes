	.file	"game.c"
	.section	.rodata
.LC0:
	.string	"Usage: game <width> <height>"
	.text
.globl printusage
	.type	printusage, @function
printusage:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$.LC0, (%esp)
	call	puts
	leave
	ret
	.size	printusage, .-printusage
	.section	.rodata
.LC1:
	.string	"%d "
	.text
.globl printboard
	.type	printboard, @function
printboard:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	movl	$0, -16(%ebp)
	jmp	.L4
.L7:
	movl	$0, -12(%ebp)
	jmp	.L5
.L6:
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	-12(%ebp), %edx
	sall	$2, %edx
	addl	%edx, %eax
	movl	(%eax), %edx
	movl	$.LC1, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$1, -12(%ebp)
.L5:
	movl	-12(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	.L6
	movl	$10, (%esp)
	call	putchar
	addl	$1, -16(%ebp)
.L4:
	movl	-16(%ebp), %eax
	cmpl	16(%ebp), %eax
	jl	.L7
	leave
	ret
	.size	printboard, .-printboard
	.section	.rodata
.LC2:
	.string	"%d"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	pushl	%ebx
	subl	$60, %esp
	cmpl	$3, 8(%ebp)
	je	.L10
	call	printusage
	movl	$1, %eax
	jmp	.L11
.L10:
	movl	$.LC2, %edx
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	leal	32(%esp), %ecx
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	__isoc99_sscanf
	cmpl	$1, %eax
	je	.L12
	call	printusage
	movl	$1, %eax
	jmp	.L11
.L12:
	movl	$.LC2, %edx
	movl	12(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	leal	28(%esp), %ecx
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	__isoc99_sscanf
	cmpl	$1, %eax
	je	.L13
	call	printusage
	movl	$1, %eax
	jmp	.L11
.L13:
	movl	28(%esp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	malloc
	movl	%eax, 36(%esp)
	movl	$0, 40(%esp)
	jmp	.L14
.L15:
	movl	40(%esp), %eax
	sall	$2, %eax
	movl	%eax, %ebx
	addl	36(%esp), %ebx
	movl	32(%esp), %eax
	sall	$2, %eax
	movl	%eax, (%esp)
	call	malloc
	movl	%eax, (%ebx)
	addl	$1, 40(%esp)
.L14:
	movl	28(%esp), %eax
	cmpl	%eax, 40(%esp)
	jl	.L15
	movl	$0, (%esp)
	call	time
	movl	%eax, (%esp)
	call	srand
	movl	$0, 40(%esp)
	jmp	.L16
.L19:
	movl	$0, 44(%esp)
	jmp	.L17
.L18:
	movl	40(%esp), %eax
	sall	$2, %eax
	addl	36(%esp), %eax
	movl	(%eax), %eax
	movl	44(%esp), %edx
	sall	$2, %edx
	leal	(%eax,%edx), %ebx
	call	rand
	movl	%eax, %edx
	sarl	$31, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	andl	$1, %eax
	subl	%edx, %eax
	movl	%eax, (%ebx)
	addl	$1, 44(%esp)
.L17:
	movl	32(%esp), %eax
	cmpl	%eax, 44(%esp)
	jl	.L18
	addl	$1, 40(%esp)
.L16:
	movl	28(%esp), %eax
	cmpl	%eax, 40(%esp)
	jl	.L19
	movl	28(%esp), %edx
	movl	32(%esp), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	36(%esp), %eax
	movl	%eax, (%esp)
	call	printboard
	movl	28(%esp), %edx
	movl	32(%esp), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	36(%esp), %eax
	movl	%eax, (%esp)
	call	update
	movl	$10, (%esp)
	call	putchar
	movl	28(%esp), %edx
	movl	32(%esp), %eax
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	36(%esp), %eax
	movl	%eax, (%esp)
	call	printboard
	movl	$0, 40(%esp)
	jmp	.L20
.L21:
	movl	40(%esp), %eax
	sall	$2, %eax
	addl	36(%esp), %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	free
	addl	$1, 40(%esp)
.L20:
	movl	28(%esp), %eax
	cmpl	%eax, 40(%esp)
	jl	.L21
	movl	36(%esp), %eax
	movl	%eax, (%esp)
	call	free
	movl	$1, %eax
.L11:
	addl	$60, %esp
	popl	%ebx
	movl	%ebp, %esp
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-4)"
	.section	.note.GNU-stack,"",@progbits
