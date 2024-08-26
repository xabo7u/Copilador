section .data
    msg db 'Hola, mundo!', 0xA  ; Mensaje con nueva línea
    len equ $ - msg             ; Longitud del mensaje

section .text
    global _start               ; Entrada del programa

_start:
    ; sys_write (escribir mensaje)
    mov eax, 4                  ; syscall número 4: sys_write
    mov ebx, 1                  ; descriptor de archivo 1: stdout
    mov ecx, msg                ; dirección del mensaje
    mov edx, len                ; longitud del mensaje
    int 0x80                    ; llamada al sistema

    ; sys_exit (salir del programa)
    mov eax, 1                  ; syscall número 1: sys_exit
    xor ebx, ebx                ; código de salida 0
    int 0x80                    ; llamada al sistema
