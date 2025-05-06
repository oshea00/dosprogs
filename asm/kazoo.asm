delay   equ  0A000h   ; delay wait
portb   equ    61h   ; IO Port B
keybd2  equ     7h   ; Keyb input no echo
status  equ    0bh   ; check keyboard status
prints  equ     9h   ; print string
doscall equ    21h   ; DOS interupt number


kazoo segment
        assume cs:kazoo
        org 100h

main proc far
start:                  ; establish label for starting addr

; Print help
	mov ah,prints   ; print help
        lea dx,helpmsg
	int doscall          

; Setup
        mov bx,500h     ; set 1/pitch
        mov dl,0        ; set pitch change to zero
        mov dh,3        ; set on/off status to on

sounder:
        mov al,10110110b; magic number for timer 2
	out 43h,al
tone:
        mov ax,bx       ; set current 1/pitch
        out 42h,al      ; send
        mov al,ah
        out 42h,al

        in  al,portb    ; read portb into al
        and al,11111100b; mask bits
        add al,dh       ; add on/off status
        out portb,al    ; send on/off to speaker        

; Raise/lower pitch by amount in AX
        mov al,bh       ; BX contains current 1/pitch
        mov ah,0        ; divide BX by 0100h
        or  ax,1        ; make ax at least 1
        or  dl,dl       ; does DL=0?
        jz skip         ; if so, AX is plus
        neg ax          ; make AX negative
skip:   add bx,ax       ; add change to pitch
        mov cx,delay    ; setup delay time
dely:   loop dely

        mov ah,status   ; check status function
        int doscall     ; call DOS
 
        inc al          ; if AL was FF then
        jz  readkey     ; character was typed
        jmp tone        ; sound tone

; Read keyboard to get digit
; 1=lower pitch, 2=higher pitch, 9=on, 0=off
readkey:
        mov ah,keybd2   ; get key noecho
        int doscall
        cmp al,'1'
        jz lower
        cmp al,'2'
        jz higher
        cmp al,'9'
        jz turn_on
        cmp al,'0'
        jz turn_off
        cmp al, 'q'
        jz quit

        jmp tone         ; unrecognized key

lower:
        mov dl,0
        jmp tone
higher:
        mov dl,1
        jmp tone
turn_on:
        mov dh,00000011b
        jmp sounder
turn_off:        
        mov dh,0
        jmp sounder
quit:
        mov al,10110110b; magic number for timer 2
        in  al,portb    ; read portb into al
        and al,11111100b; mask bits
        add al,0        ; add off status
        out portb,al    ; send on/off to speaker        
	out 43h,al
        int 20h         ; DOS quit

main    endp

helpmsg db '1=pitchdown, 2=pitchup, 0=off, 9=on, q=quit'
        db 0dh
        db 0ah
        db '$'


kazoo   ends            ; end code segment
;**************************************************
        end start       ; end asssembly





