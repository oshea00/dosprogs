_TEXT     SEGMENT
          ASSUME CS:_TEXT,DS:_TEXT,ES:_TEXT,SS:_TEXT
          ORG 100H

START:
          mov ah,0Ah                           ; DOS input function
          lea dx, buffmax                      ; load addr of buff struct
          int 21h                              ; call function
          mov bl,byte ptr filled               ; load char count into BL
          mov bh,0                             ; zero BH
          mov byte ptr [bufdata+1 + BX],'$'    ; append '$' to end of chars
          mov ah,2h                            ; DOS display char function
          mov dl,0Ah                           ; LF char to display
          int 21h                              ; call function
          mov ah,9h                            ; DOS display string function
          lea dx, bufdata                      ; load addr of entered string
          int 21h                              ; call function

          int 20h                              ; end program

buffmax   db 80                                ; max chars
filled    db 0h                                ; filled count
bufdata   db 80 DUP(0)                         ; zero fill
guard     db '$'                               ; $ guard

_TEXT ENDS
END START


