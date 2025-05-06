; This compiles to EXE using MASM:
; >masm happy4
; >link happy4
; >exe2bin happy4 happy4.com
        TITLE HAPPY4
_TEXT   SEGMENT
        ASSUME CS:_TEXT,DS:_TEXT,ES:_TEXT,SS:_TEXT
        ORG 100H
   START:

    mov dl,1
    mov ah,2
    int 21h
    int 20h

_TEXT ENDS
END START



