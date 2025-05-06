; This compiles straight to COM using MASM611:
; >ml /AT happy3.asm
.MODEL TINY
.DOSSEG
.DATA
.CODE
.STARTUP
    mov dl,1
    mov ah,2
    int 21h
    int 20h
END

