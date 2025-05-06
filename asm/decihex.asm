decihex segment
        assume cs:decihex
        org 100h

          
getin:    call decibin                         ; get keyboard to binary
          call binihex                         ; binary to screen
          call crlf
          jmp getin                            ; loop again
          int 20h
          
; PROCEDURE decibin

decibin   proc near

          mov bx,0                             ; clear BX
newchar:
          mov ah,1                             ; getchar DOS
          int 21h                              ; call
          cmp al,'q'                           ; check for quit
          je  quit
          sub al,30h                           ; check digit?
          jl  exit                             ; no
          cmp al,9d                            ; check digit
          jg  exit                             ; > 9? no
          cbw                                  ; convert digit in AL to AX
          xchg ax,bx                           ; swap digit and result
          mov cx,10d                           ; shift result left
          mul cx                               ; via mult by 10
          xchg ax,bx                           ; swap digit and result
          add bx,ax                            ; add it to result in BX
          jmp newchar                          ; get next digit

quit:
          call crlf
          int 20h                              ; get out

exit:
          ret

decibin   endp

;    PROCEDURE binihex

binihex   proc near

          mov ch,4                             ; number of digits
rotate:   mov cl,4                             ; set count to 4 bits
          rol bx,cl                            ; left digit to right
          mov al,bl                            ; al = bl>>4
          and al,0fh                           ; keep the right 4 bits
          add al,30h                           ; convert hex al to ASCII
          cmp al,3ah                           ; is it > 9 ?
          jl  printit                          ; jmp if = 0 to 9
          add al,7h                            ; al is A to F (10-14)
printit:
          mov dl,al                            ; move ASCII to dl
          mov ah,2                             ; DOS char out
          int 21h
          dec ch                               ; check if digits done
          jnz rotate                           ; if not goto rotate

          ret

binihex   endp


; PROCEDURE crlf

crlf      proc near

          mov dl,0dh
          mov ah,2
          int 21h
          mov dl,0ah
          mov ah,2
          int 21h

          ret

crlf      endp

decihex   ends
          end



