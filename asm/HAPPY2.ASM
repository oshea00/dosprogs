prognam segment
        assume cs:prognam
        org 100h

        mov dl,1
        mov ah,2
        int 21h
        int 20h
prognam ends
        end

