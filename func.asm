;3/((x - 1)^2 + 1)
global f1
f1:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld qword[ebp + 8]
    fld1
    fsubp
    fld st0
    fmulp
    fld1
    faddp
    mov dword[esp], 3
    fild dword[esp]
    fdivrp 
    
    leave
    ret
    
;sqrt (x + 0.5)
global f2
f2:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld1
    mov dword[esp], 2
    fild dword[esp]
    fdiv
    fld qword[ebp + 8]
    faddp
    fsqrt
      
    leave
    ret
    
; exp(-x)
global f3
f3:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld qword[ebp + 8]
    fchs
    fldl2e
    fmulp
    fld st0
    frndint
    fld1
    fscale
    
    fld st1
    fld st3
    fsubp
    fchs
    f2xm1
    fld1
    faddp
    fmulp
    
    leave
    ret
    
;-6(x-1)/((x-1)^2+1)^2
global dvf1
dvf1:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld qword[ebp + 8]
    fld1
    fsubp
    fld st0
    fmulp
    fld1
    faddp
    fld st0
    fmulp    
    
    fld qword[ebp + 8]
    fld1
    fsubp
    mov dword[esp], 6
    fild dword[esp]
    fmulp
    
    fld st1
    fdivp
    fchs
     
    leave
    ret
    
;1/(2*sqrt (x + 0.5))
global dvf2
dvf2:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld1
    mov dword[esp], 2
    fild dword[esp]
    fdivp
    
    fld1
    mov dword[esp], 2
    fild dword[esp]
    fdivp
    fld qword[ebp + 8]
    faddp
    fsqrt
    fdivp
    
    leave
    ret
    
;-exp(-x)
global dvf3
dvf3:
    finit
    push ebp
    mov ebp, esp
    sub esp, 4
    
    fld qword[ebp + 8]
    fchs
    fldl2e
    fmulp
    fld st0
    frndint
    fld1
    fscale
    
    fld st1
    fld st3
    fsubp
    fchs
    f2xm1
    fld1
    faddp
    fmulp
    fchs
    
    leave
    ret