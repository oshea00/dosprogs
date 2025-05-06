prognam segment
        assume cs:prognam

        mov cx,100h     ; put count in cx
        mov dl,0        ; first char

next:
        mov ah,2        ; DOS display output
        int 21h         ; call DOS
        inc dl          ; next char
        loop next       ; do until CX==0

        int 20h

prognam ends

        end

