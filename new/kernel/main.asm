BITS 32
 
VGA_WIDTH equ 80
VGA_HEIGHT equ 25
 
VGA_COLOR_BLACK equ 0
VGA_COLOR_BLUE equ 1
VGA_COLOR_GREEN equ 2
VGA_COLOR_CYAN equ 3
VGA_COLOR_RED equ 4
VGA_COLOR_MAGENTA equ 5
VGA_COLOR_BROWN equ 6
VGA_COLOR_LIGHT_GREY equ 7
VGA_COLOR_DARK_GREY equ 8
VGA_COLOR_LIGHT_BLUE equ 9
VGA_COLOR_LIGHT_GREEN equ 10
VGA_COLOR_LIGHT_CYAN equ 11
VGA_COLOR_LIGHT_RED equ 12
VGA_COLOR_LIGHT_MAGENTA equ 13
VGA_COLOR_LIGHT_BROWN equ 14
VGA_COLOR_WHITE equ 15
 
global kernel_main
kernel_main:
    mov dh, VGA_COLOR_LIGHT_GREY
    mov dl, VGA_COLOR_BLACK
    call terminal_set_color
    mov esi, hello_string
    call terminal_write_string
    jmp $
 
 
; IN = dl: y, dh: x
; OUT = dx: Index with offset 0xB8000 at VGA buffer
; Other registers preserved
terminal_getidx:
    push ax; preserve registers
 
    shl dh, 1 ; multiply by two because every entry is a word that takes up 2 bytes
 
    mov al, VGA_WIDTH
    mul dl
    mov dl, al
 
    shl dl, 1 ; same
    add dl, dh
    mov dh, 0
 
    pop ax
    ret
 
; IN = dl: bg color, dh: fg color
; OUT = none
terminal_set_color:
    shl dl, 4
 
    or dl, dh
    mov [terminal_color], dl
 
 
    ret
 
; IN = dl: y, dh: x, al: ASCII char
; OUT = none
terminal_putentryat:
    pusha
    call terminal_getidx
    mov ebx, edx
 
    mov dl, [terminal_color]
    mov byte [0xB8000 + ebx], al
    mov byte [0xB8001 + ebx], dl
 
 
    popa
    ret
 
; IN = al: ASCII char
terminal_putchar:
    mov dx, [terminal_cursor_pos] ; This loads terminal_column at DH, and terminal_row at DL
 
    call terminal_putentryat
 
    inc dh
    cmp dh, VGA_WIDTH
    jne .cursor_moved
 
    mov dh, 0
    inc dl
 
    cmp dl, VGA_HEIGHT
    jne .cursor_moved
 
    mov dl, 0
 
 
.cursor_moved:
    ; Store new cursor position 
    mov [terminal_cursor_pos], dx
 
    ret
 
; IN = cx: length of string, ESI: string location
; OUT = none
terminal_write:
    pusha
.loopy:
 
    mov al, [esi]
    call terminal_putchar
 
    dec cx
    cmp cx, 0
    je .done
 
    inc esi
    jmp .loopy
 
 
.done:
    popa
    ret
 
; IN = ESI: zero delimited string location
; OUT = ECX: length of string
terminal_strlen:
    push eax
    push esi
    mov ecx, 0
.loopy:
    mov al, [esi]
    cmp al, 0
    je .done
 
    inc esi
    inc ecx
 
    jmp .loopy
 
 
.done:
    pop esi
    pop eax
    ret
 
; IN = ESI: string location
; OUT = none
terminal_write_string:
    pusha
    call terminal_strlen
    call terminal_write
    popa
    ret
 
; Exercises:
; - Newline support
; - Terminal scrolling when screen is full
; Note: 
; - The string is looped through twice on printing. 
 
hello_string db "Hello, kernel World!", 0xA, 0 ; 0xA = line feed
 
 
terminal_color db 0
 
terminal_cursor_pos:
terminal_column db 0
terminal_row db 0