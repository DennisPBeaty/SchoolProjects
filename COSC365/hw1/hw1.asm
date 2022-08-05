;Preston Beaty
;netID: dbeaty
section .data ; Data segment
userMsg db 'Please enter a number: ', 0xa, 0xD ; Ask the user to
enter a number
lenUserMsg equ $-userMsg ; The length of the message
dispMsg db 'You have entered: ', 0xa, 0xD
lenDispMsg equ $-dispMsg
AddMsg db 'The sum of these two numbers is: ', 0xa, 0xD
lenAddMsg equ $-AddMsg
SubMsg db 'The difference of these two numbers is: ', 0xa, 0xD
lenSubMsg equ $-SubMsg
MultMsg db 'The product of these two numbers is: ', 0xa, 0xD
lenMultMsg equ $-MultMsg
Div1Msg db 'The quotient of these two numbers is: ', 0xa, 0xD
lenDiv1Msg equ $-Div1Msg
Div2Msg db 'The remainder of these two numbers is: ', 0xa, 0xD
lenDiv2Msg equ $-Div2Msg
zeroErrorMsg db 'Divide by zero error', 0xa, 0xD
lenzeroErrorMsg equ $-zeroErrorMsg
gthanMsg db 'Greater than 10', 0xa, 0xD
lengthanMsg equ $-gthanMsg
ncMsg db 0xa ; print newline
lenncMsg equ $-ncMsg ; length of newline message
section .bss ; Uninitialized data
num resb 1
num2 resb 1
addi resb 3
subbi resb 1
product resb 1
quotient resb 1
remainder resb 1
section .text ; Code Segment
global _start
_start:
; User prompt
mov eax, 4
mov ebx, 1
mov ecx, userMsg
mov edx, lenUserMsg
int 80h
; Read and store the user input
mov eax, 3
mov ebx, 0
mov ecx, num
mov edx, 1
int 80h
; Output the message 'You have entered:'
mov eax, 4
mov ebx, 1
mov ecx, dispMsg
mov edx, lenDispMsg
int 80h
; Output the number entered
mov eax, 4
mov ebx, 1
mov ecx, num
mov edx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
; Read and store another user input number
mov eax, 3
mov ebx, 0
mov ecx, num2
mov edx, 1
int 80h
; Output the message 'The entered number is: '
mov eax, 4
mov ebx, 1
mov ecx, dispMsg
mov edx, lenDispMsg
int 80h
; Output the number entered
mov eax, 4
mov ebx, 1
mov ecx, num2
mov edx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
; Begin Addition
; Output the message 'The sum of these two numbers is:'
mov eax, 4
mov ebx, 1
mov ecx, AddMsg
mov edx, lenAddMsg
int 80h
mov al, [num]
sub al, '0'
mov bl, [num2]
sub bl, '0'
add al, bl
mov edx, 2
mov ecx, addi
;if the ouput isnt two decimals skip
cmp al, 10
jb normal
;if there is two numbers in output
mov byte [ecx], '1'
inc ecx
inc edx
sub al, 10
normal:
add al, '0'
;second digit
mov [ecx], ax
;print the sum/addi
mov ecx, addi
mov eax, 4
mov ebx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
; Output the message 'The difference of these two numbers is:'
mov eax, 4
mov ebx, 1
mov ecx, SubMsg
mov edx, lenSubMsg
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
;Start subtraction
mov eax, [num]
sub eax, '0'
mov ebx, [num2]
sub ebx, '0'
cmp al, bl
jg greatthan
;retreive result and store into data
sub bl, al
mov edx, 2
mov ecx, subbi
add bl, '0'
mov bh, 0xa
mov [ecx], ebx
mov eax, 4
mov ebx, 1
int 80h
jmp Continue
greatthan:
;retreive result and store into data
sub al, bl
mov edx, 2
mov ecx, subbi
add al, '0'
mov ah, 0xa
mov [ecx], eax
mov eax, 4
mov ebx, 1
int 80h
jmp Continue
Continue:
;START MULTIPLACATION HERE
; Output the multiply message
mov eax, 4
mov ebx, 1
mov ecx, MultMsg
mov edx, lenMultMsg
int 80h
mov al, [num]
sub al, '0'
mov bl, [num2]
sub bl, '0'
mul bl
;if need to perform operations cause num is greateder than 10 enter
twodigis
cmp al, 9
jg twodigis
add al, '0'
;if not print number
mov [product], al
mov eax, 4
mov ebx, 1
mov ecx, product
mov edx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
;jump past two digis operation to continue division
jmp ContinueDiv
twodigis:
add al, '0'
mov [product], al
mov al, [product]
sub al, '0'
mov bl, 10
div bl
add al, '0'
mov [quotient], al
add ah, '0'
mov [remainder], ah
;print the quotient
mov eax, 4
mov ebx, 1
mov ecx, quotient
mov edx, 1
int 80h
;print remainder
mov eax, 4
mov ebx, 1
mov ecx, remainder
mov edx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
jmp ContinueDiv
ContinueDiv:
;Start Division Here
;check for divide by 0 error
mov eax, [num2]
cmp al, '0'
jle zero
; Output the message 'The difference of these two numbers is:'
mov eax, 4
mov ebx, 1
mov ecx, Div1Msg
mov edx, lenDiv1Msg
int 80h
;start Division
mov al, [num]
sub al, '0'
mov bl, [num2]
sub bl, '0'
div bl
add al, '0'
mov [quotient], al
add ah, '0'
mov [remainder], ah
;print the quotient
mov eax, 4
mov ebx, 1
mov ecx, quotient
mov edx, 1
int 80h
; Print newline carriage return
mov eax, 4
mov ebx, 1
mov ecx, ncMsg
mov edx, lenncMsg
int 80h
;print div part 2 message
mov eax, 4
mov ebx, 1
mov ecx, Div2Msg
mov edx, lenDiv2Msg
int 80h
;print remainder
mov eax, 4
mov ebx, 1
mov ecx, remainder
mov edx, 1
int 80h
jmp Continue2
zero:
; Skip division and print message due to divide by zero error
mov eax, 4
mov ebx, 1
mov ecx, zeroErrorMsg
mov edx, lenzeroErrorMsg
int 80h
jmp Continue2
Continue2:
; Exit program
mov eax, 1
int 80h