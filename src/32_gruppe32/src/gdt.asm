;Inspired by OsDev.org and OliveStem on Youtube(https://www.youtube.com/@olivestemlearning)

global flush_gdt

flush_gdt:
    mov eax, [esp + 4]
    lgdt [eax]
    jmp 0x08:.flush

.flush:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret


    


