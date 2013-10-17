#:<><><>   Small-C  V1.2  DOS--CP/M Cross Compiler   <><><>
#:<><><><><>   CP/M Large String Space Version   <><><><><>
#:<><><><><><><><><><>   By Ron Cain   <><><><><><><><><><>
#:
#:dude(b) int b; {
	.text
	.align 16
.globl dude
	.TYPE	dude,@function
dude:
	pushl %ebp
	movl %esp, %ebp
#:  printf("B: %d\n",b);
	leal	8(%ebp), %eax
	movl (%eax), %eax
	pushl %eax
	movl $cc1+0, %eax
	pushl %eax
	call printf
	addl $8, %esp
#:}
	movl %ebp, %esp
	popl %ebp
	ret
#:main() {
	.text
	.align 16
.globl main
	.TYPE	main,@function
main:
	pushl %ebp
	movl %esp, %ebp
#:  dude(5);
	movl $5, %eax
	pushl %eax
	call dude
	popl %edx
#:}
	movl %ebp, %esp
	popl %ebp
	ret
	.section	.rodata
cc1:
	.byte 66,58,32,37,100,10,0

#: --- End of Compilation ---
	.IDENT	"Small C"
