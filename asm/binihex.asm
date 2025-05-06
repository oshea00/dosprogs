_TEXT     SEGMENT
          ASSUME CS:_TEXT,DS:_TEXT,ES:_TEXT,SS:_TEXT
          ORG 100H

START:                                         ; expect number in BX
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

          int 20h                              ; end program


_TEXT ENDS

END START


