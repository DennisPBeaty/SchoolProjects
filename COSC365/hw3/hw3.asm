

Page
6
of 6
;Preston Beaty
;netID: dbeaty
section .data                           ; Data segment
   userMsg db 'Please enter numbers between 0 - 4,294,967,295: ', 0xa, 0xD   ; Ask 
the user to enter a number
   lenUserMsg equ $-userMsg                ; The length of the message
   inputMsg db 'The list is the following: ', 0xa, 0xD   ; Show numbers entered
   leninputMsg equ $-inputMsg                ; The length of the message
   sortMsg db 'The sorted list is the following: ', 0xa, 0xD   ; Show numbers 
entered
   lensortMsg equ $-sortMsg                ; The length of the message
   spaceMsg db ' '
   lenspaceMsg equ $-spaceMsg                ; The length of the message
   ncMsg db 0xa                    ; print newline
   lenncMsg equ $-ncMsg   ; length of newline message
   ;Initialization for Linked List
   head dd 0
   tail dd 0
   temp dd 0
   heap_base dd 0
   prev dd 0
section .bss           ; Uninitialized data
    numberRead resd 1
    stacker resd 1
    hold resd 1
    isX resd 1
    printer resd 1
    print_List resd 1
    test1 resb 1
    test2 resb 1
    swapper resd 1
    storeAddress1 resd 1
    storeAddress2 resd 1
    inputPrinter resd 1
    printer_address resd 1
    zero_value resd 1
;Macros:
%macro newLine 0
   ; Print newline carriage return
   mov eax, 4
   mov ebx, 1
   mov ecx, ncMsg
   mov edx, lenncMsg
   int 80h
%endmacro
%macro writeLine 2
    mov eax, 4
    mov ebx, 1
    mov ecx, %1
    mov edx, %2
    int 80h
%endmacro
%macro printNum 1
   ; Output the number entered
   mov eax, 4
   mov ebx, 1
   mov ecx, %1
   mov edx, 1
   int 80h
%endMacro
;START OF CODE-----------------------------------------------------------------
section .text          ; Code Segment
   global _start
_start:
    ;Troubleshooting
    mov al, '1'
    mov [test1], al
    mov bl, '2'
    mov [test2], bl
    ;Set vars to 0
    mov eax, '0'
    mov [isX], eax
    ; User prompt
    writeLine userMsg, lenUserMsg
    writeLine inputMsg, leninputMsg
begin_read:
    ;If X is read skip reading and adding
    mov eax, [isX]
    cmp eax, '1'
    je done_read
    ;Read number in
    call readNumber
    ;Check if white space exists
    mov eax, [zero_value]
    cmp eax, 0
    je begin_read
    cmp eax, 'X'
    je done_read
    mov eax, [numberRead]
    mov [inputPrinter], eax
    call printNumber_32_input
    newLine
    ;Create List
    call createList
    ;Allocate size of node
    call allocateNode
    ;Initalize node with value
    call initializeNode
    ;Insert into List
    call attatchNode
    ;Read next Number
    jmp begin_read
done_read:
    newLine
    writeLine sortMsg, lensortMsg
    mov eax, [head]
    mov [printer_address], eax
    call procprint_List
eop:
    ; Exit program
    mov eax, 1
    int 80h
;------------------------------------------------------------------------------
;Start of Procedures
;Start List
createList:
    ;Allocating Node of Linked List
    mov eax, 45 ; calling break system call (45 – sys_bark)
    xor ebx, ebx ; get memory address for base of ourheap area
    int 0x80
    mov [heap_base], eax ; store/save the heap base
ret
;Allocate a Node
allocateNode:
    mov eax, 45    ; calling break system call (45 - sys_bark)
    mov ebx, [heap_base]
    add ebx, 8  ; ebx = address for base of node for linked list
    int 0x80
    ; eax contains the address of the space just allocated
ret
;Initialize Node w/ Desired Value
initializeNode:
    ; eax contains base address of new block
    mov [temp], eax
    mov edx, DWORD [numberRead]
    mov [eax], edx
    add eax, 4
    mov [eax], dword 0
ret
;Attatch Node To List
attatchNode:
    cmp dword [tail], 0   ; Determine if on first node
    jnz ListExists
    ;If First Node
    mov eax, [temp] ; List does not exist
    mov [head], eax ; Have head and tail contain
    mov [tail], eax ; Address of the new node
    ret    ; Temp contains address of new node
    ;If not first Node
ListExists:
    mov eax, [head]
    push eax
    mov eax, [eax]
    cmp eax, [numberRead] ; see if lower than head value
    jg firstNode ; if so move to insert in front of head
    pop eax
linking_list: ;Determine where to insert
    mov [prev], eax
    add eax, 4
    cmp dword [eax], 0 ;if greater than last node
    je lastNode
    mov ebx, [eax]
    mov ebx, [ebx]
    cmp ebx, [numberRead]
    jg jump_insert ;if value is inbetween two nodes
    mov eax, [eax]
    jmp linking_list
jump_insert: ;insert node in between two other nodes
    mov eax, [temp]
    add eax, 4
    mov ebx, [prev]
    add ebx, 4
    mov ebx, [ebx]
    mov [eax], ebx
    mov eax, [prev]
    add eax, 4
    mov ebx, [temp]
    mov [eax], ebx
    ret
lastNode: ;add new tail node to existing linked list
    mov eax, [tail]
    add eax, 4
    mov ebx, [temp]
    mov [eax], ebx
    mov [tail], ebx
    ret
firstNode: ;move to insert in front of head
    pop eax
    mov eax, [temp]
    add eax, 4 ;create new node and make next node the previous head node
    mov ebx, [head]
    mov [eax] ,ebx
    mov eax, [temp]
    mov [head], eax
    ret
;Print out List
procprint_List:
    mov eax, [printer_address];move head to eax
    push eax ;store current address
    mov eax, [eax];get value to print
    mov [numberRead], eax
    call printNumber_32 ;print the value
    newLine
    pop eax ;retrieve address
    add eax, 4 ;increase address to next node
    cmp dword [eax], 0 ; if the address is exmpty doesnt exist
    je done_print
    mov eax, [eax] ; move address back to update to next address progressed to
    mov [printer_address], eax
    jmp procprint_List ;jump to print next node
done_print:
ret
;Read in 32 Bit Number
readNumber:
    XOR eax, eax
    mov [zero_value], eax
    mov [numberRead], eax
restart:
    ;Read In Number
    mov eax, 3
    mov ebx, 0
    mov ecx, hold
    mov edx, 1
    int 80h
    mov eax, [hold]
    ;If not number
    cmp eax, 'X'
    je x_detect
    cmp eax, '9'
    jg finish
    cmp eax, '0'
    jl finish
    ;Multiply
    mov eax, [numberRead]
    mov ebx, 10
    mul ebx
    mov [numberRead], eax
    ;Clear EAX
    XOR eax, eax
    mov eax, [hold]
    sub eax, '0'
    add [numberRead], eax
    ;Check for zero_value
    mov eax, [zero_value]
    inc eax
    mov [zero_value], eax
    jmp restart
x_detect:
    mov eax, '1'
    mov [isX], eax
finish:
ret
;Print 32 Bit Number
printNumber_32:
    mov edx, 127
    push edx
redo_divide:
    mov edx, 0
    mov eax, [numberRead]
    mov ecx, 10
    div ecx
    ;Push remainder
    add edx, '0'
    push edx
    mov [numberRead], eax
    cmp eax, 0
    je finish_read
    ;If not equal to 0
    jmp redo_divide
finish_read:
    pop edx
    cmp edx, 127
    je fully_printed
    ;if not end of stack
    mov [printer], edx
    printNum printer
    jmp finish_read
fully_printed:
ret
;Print 32 Bit Number
printNumber_32_input:
    mov edx, 127
    push edx
redo_divide1:
    mov edx, 0
    mov eax, [inputPrinter]
    mov ecx, 10
    div ecx
    ;Push remainder
    add edx, '0'
    push edx
    mov [inputPrinter], eax
    cmp eax, 0
    je finish_read1
    ;If not equal to 0
    jmp redo_divide1
finish_read1:
    pop edx
    cmp edx, 127
    je fully_printed1
    ;if not end of stack
    mov [printer], edx
    printNum printer
    jmp finish_read1
fully_printed1:
ret
