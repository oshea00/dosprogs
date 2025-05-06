display   equ 9h
doscall   equ 21h

; ****** data
datarea   segment

msg       db 'This is my Hello World.',0dh,0ah,'$'

datarea   ends

; ****** code

pstring   segment

main      proc far
          assume cs:pstring,ds:datarea               
start:    
          push ds             ; save old ds
          sub ax,ax           ; zero AX
          push ax             ; put AX on stack

          mov ax,datarea      ; get datarea addr into AX
          mov ds,ax           ; and set ds

          mov ah,display
          lea dx,msg
          int doscall

          ret
main      endp

pstring   ends
          end start



