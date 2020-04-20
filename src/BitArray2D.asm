
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        ;step 1: create index - Row * width + column
        ;width would be 8 right?

        mov rax,rdx
        imul rax,rsi
        add rax,rcx


        mov r15, rax ; copied index
        cmp rax, 8
        jle .skip

.divi:
        sub rax, 8
        cmp rax, 8
        jge .divi
.skip:
        ; rax is now byte_offset
        mov r11, rax

        mov r9, 1
        cmp r11, 0
        je .set

.mask:
        shl r9, 1
        dec r11
        cmp r11, 0
        jne .mask
.set:

        add rdi, r15
        or [rdi],r9


        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col
        mov rax,rdx
        imul rax,rsi
        add rax,rcx


        mov r15, rax ; r8 is copied index
        cmp rax, 8
        jle .skip

.divi: ; This "divides" our index by 8 until we only have a remainder left. that remainder is the byte offset!
        sub rax, 8
        cmp rax, 8
        jge .divi
.skip: ; Skip just skips the byte offset in case we don't need one
        ; rax is now bit_offset
        mov r11, rax

        mov r9, 1
        cmp r11, 0
        je .set

.mask: ; creates the bit mask by looping and shifting by one until the value in the bit offset is zero
        shl r9, 1
        dec r11
        cmp r11, 0
        jne .mask
.set: ;flips the bit in the value at the array index.

        add rdi, r15
        and [rdi],r9
        mov rax, [rdi]
        cmp rax,0b
        je .L4
        mov rax, 1


        ; add your code here - for now returning 0
.done:
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

.L4:
        mov rax, 0
        jmp .done