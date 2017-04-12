%define EXP_PRECISION 30 ; кол-во членов ряда тэйлора exp(x)


section .data
coeff dd -0.6666666666


section .text
    
    
    
global _f1
_f1: ;exp(x) + 2
    push ebp
    mov ebp, esp
    sub esp, 8
    finit
    fld dword[ebp + 8]
    mov ecx, EXP_PRECISION
    fldz
_f1.loop:
    cmp ecx, 0
    je _f1.loop.end
    dec ecx
    mov [esp], ecx
    fld1
    call pwr
    mov [esp], ecx
    call dfact
    faddp
    jmp _f1.loop
_f1.loop.end:
    fld1
    fld1
    faddp
    faddp
    ffree ST1
    mov esp, ebp
    pop ebp
    ret
    
pwr: ; *x^n
    cmp dword [ebp - 8], 0
    je pwr.end
    fmul ST0, ST2
    dec dword [ebp - 8]
    call pwr
pwr.end:
    ret
    
dfact: ; /n!
    cmp dword [ebp - 8], 0
    je dfact.end
    fidiv dword [ebp - 8]
    dec dword [ebp - 8]
    call dfact
dfact.end:
    ret
    
    
    
global _f2
_f2: ;-1/x
    push ebp
    mov ebp, esp
    sub esp, 8
    finit
    fld dword[ebp + 8]
    fldz
    fld1
    fsubp
    fdiv ST0, ST1
    ffree ST1
    mov esp, ebp
    pop ebp
    ret



global _f3
_f3:  ;(-2/3)*(x + 1)
    push ebp
    mov ebp, esp
    sub esp, 8
    finit
    fld dword[ebp + 8]
    fld1
    faddp
    fmul dword[coeff]
    mov esp, ebp
    pop ebp
    ret




global _f1d
_f1d: ;(exp(x) + 2)' = exp(x)
    push ebp
    mov ebp, esp
    sub esp, 8
    mov eax, [ebp + 8]
    mov [esp], eax
    call _f1 ;exp(x) = (exp(x) + 2) - 2
    fld1
    fld1
    faddp
    fsubp
    mov esp, ebp
    pop ebp
    ret




global _f2d
_f2d: ;(-1/x)' = 1/x^2
    push ebp
    mov ebp, esp
    sub esp, 8
    mov eax, [ebp + 8]
    mov [esp], eax
    call _f2 ;1/x^2 = (-1/x)^2
    fmul ST0, ST0
    mov esp, ebp
    pop ebp
    ret




global _f3d
_f3d: ;((-2/3)(x + 1))' = -2/3
    push ebp
    mov ebp, esp
    sub esp, 8
    finit
    fld dword [coeff]
    mov esp, ebp
    pop ebp
    ret
