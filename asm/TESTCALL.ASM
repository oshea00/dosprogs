prognam segment
	assume cs:prognam
	org 100h

main    proc
	call crlf
	int 20h
	ret
main    endp

crlf    proc
		mov dl,1
	mov ah,2
	int 21h
crlf    endp

prognam ends
	end

