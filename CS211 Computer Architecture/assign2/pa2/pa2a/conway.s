
.file"conway.c"
.text
.globl update
        .type   update, @function
update:
        pushl   %ebp
        movl    %esp, %ebp #basic stuff
        pushl   %ebx #int a 
        subl    $52, %esp #local variable space
        movl    16(%ebp), %eax #height to eax
        sall    $2, %eax 
        movl    %eax, (%esp)
        call    malloc #malloc board rows
        movl    %eax, -32(%ebp)
        movl    $0, -36(%ebp) #
        jmp     .L24
.L25:
        movl    -36(%ebp), %eax #malloc stuff for columns
        sall    $2, %eax
        movl    %eax, %ebx
        addl    -32(%ebp), %ebx
        movl    12(%ebp), %eax
        sall    $2, %eax
        movl    %eax, (%esp)
        call    malloc
        movl    %eax, (%ebx)
        addl    $1, -36(%ebp)

.L24:
        movl    -36(%ebp), %eax 
        movl	16(%ebp), %ecx
	start_loop:
	cmpl    16(%ebp), %eax 
        jl      .L25 
	loop start_loop
        movl    $0, -28(%ebp) # i = 0 
        jmp     .L26 
.L43:
        movl    $0, -20(%ebp) #neighbors = 0
        jmp     .L27
.L42:
        movl    $0, -16(%ebp) # 0 = neighbors
        cmpl    $0, -20(%ebp) # compare j to 0
        je      .L28# if equal go to L28
        movl    -28(%ebp), %eax#
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        subl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L29
        addl    $1, -16(%ebp)
.L29:
        movl    -28(%ebp), %eax
        addl    $1, %eax
        cmpl    16(%ebp), %eax
        je      .L30
        movl    -28(%ebp), %eax
        addl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        subl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L30
        addl    $1, -16(%ebp)


.L30:
        movl    -28(%ebp), %eax
        subl    $1, %eax
        testl   %eax, %eax
        js      .L28
        movl    -28(%ebp), %eax
        subl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        subl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L28
        addl    $1, -16(%ebp)
.L28:
        cmpl    $0, -28(%ebp)
        je      .L31
        movl    -28(%ebp), %eax
        subl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L31
        addl    $1, -16(%ebp)
.L31:
        movl    -20(%ebp), %eax
        addl    $1, %eax
        cmpl    12(%ebp), %eax
        je      .L32
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        addl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L33
        addl    $1, -16(%ebp)
.L33:
        movl    -28(%ebp), %eax
        addl    $1, %eax
        cmpl    16(%ebp), %eax
        je      .L34
        movl    -28(%ebp), %eax
        addl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        addl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L34
        addl    $1, -16(%ebp)
.L34:
        movl    -28(%ebp), %eax
        subl    $1, %eax
        testl   %eax, %eax
        js      .L32
        movl    -28(%ebp), %eax
        subl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        addl    $1, %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L32
        addl    $1, -16(%ebp)


.L32:
        movl    -28(%ebp), %eax
        addl    $1, %eax
        cmpl    16(%ebp), %eax
        je      .L35
        movl    -28(%ebp), %eax
        addl    $1, %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L35
        addl    $1, -16(%ebp)
.L35:
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L36
        cmpl    $1, -16(%ebp)
        jg      .L37
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $0, (%eax)
.L37:
        cmpl    $3, -16(%ebp)
        jne     .L38
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $1, (%eax)
.L38:
        cmpl    $2, -16(%ebp)
        jne     .L39
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $1, (%eax)
.L39:
        cmpl    $3, -16(%ebp)
        jle     .L36
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $0, (%eax)
.L36:
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        testl   %eax, %eax
        jne     .L40
        cmpl    $3, -16(%ebp)
        jne     .L41
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $1, (%eax)

.L41:
        cmpl    $3, -16(%ebp)
        je      .L40
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -20(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $0, (%eax)
.L40:
        addl    $1, -20(%ebp) #add one to j
.L27:
        movl    -20(%ebp), %eax #j to eax
        cmpl    12(%ebp), %eax#compare width
        jl      .L42 #if true go to L42
        addl    $1, -28(%ebp)  #i++
.L26:
        movl    -28(%ebp), %eax # i to eax
        cmpl    16(%ebp), %eax #compare height i
        jl      .L43 #if less go to l43
        movl    $0, -24(%ebp) #i = 0
        jmp     .L44 #else go to L44
.L49:
        movl    $0, -12(%ebp) 
        jmp     .L45 
.L48:
        movl    -24(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -12(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        cmpl    $1, %eax
        jne     .L46
        movl    -24(%ebp), %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -12(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $1, (%eax)


.L46:
        movl    -24(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    -12(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    (%eax), %eax
        testl   %eax, %eax
        jne     .L47
        movl    -24(%ebp), %eax
        sall    $2, %eax
        addl    8(%ebp), %eax
        movl    (%eax), %eax
        movl    -12(%ebp), %edx
        sall    $2, %edx
        addl    %edx, %eax
        movl    $0, (%eax)
.L47:
        addl    $1, -12(%ebp)
.L45:
        movl    -12(%ebp), %eax 
        cmpl    12(%ebp), %eax 
        jl      .L48 
        addl    $1, -24(%ebp) 

.L44:
        movl    -24(%ebp), %eax
        cmpl    16(%ebp), %eax 
        jl      .L49 
        movl    $0, -28(%ebp) 
        jmp     .L50 
.L51:
        movl    -28(%ebp), %eax
        sall    $2, %eax
        addl    -32(%ebp), %eax
        movl    (%eax), %eax
        movl    %eax, (%esp)
        call    free
        addl    $1, -28(%ebp)
.L50:
        movl    -28(%ebp), %eax
        cmpl    16(%ebp), %eax
        jl      .L51
        movl    -32(%ebp), %eax
        movl    %eax, (%esp)
        call    free
        movl    $1, %eax
        addl    $52, %esp
        popl    %ebx
        popl    %ebp
        ret
        .size   update, .-update
        
