.text
.align 16
.globl foo
	.TYPE foo,@function
foo:
	pushl %ebp
	movl %esp, %ebp
	movl x, %eax
	movl x, %eax
	pushl %eax
	leal 8(%ebp), %eax
	movl (%eax), %eax
	popl %edx
	addl %edx, %eax
	movl %eax, x
	movl %ebp, %esp
	popl %ebp
	ret
.globl main
	.TYPE main,@function
main:
	pushl %ebp
	movl %esp, %ebp
	movl x, %eax
	movl x, %eax
	pushl %eax
	movl $10, %eax
	pushl %eax
	movl $6, %eax
	popl %edx
	imull %edx
	pushl %eax
	movl $2, %eax
	popl %edx
	xchgl %eax, %edx
	movl %edx, %ecx
	cltd
	idivl %ecx
	popl %edx
	addl %edx, %eax
	movl %eax, x
	movl x, %eax
	movl x, %eax
	pushl %eax
	movl $1, %eax
	popl %edx
	addl %edx, %eax
	movl %eax, x
	movl $5, %eax
	pushl %eax
	call foo
	movl x, %eax
	pushl %eax
	movl $cc1+0, %eax
	pushl %eax
	call printf
	addl $8, %esp
	movl %ebp, %esp
	popl %ebp
	ret
cc1:
	.byte 37,100,10,0
	.comm y,40,4
	.comm x,1,4
