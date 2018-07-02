section .data
  msg1      DB 'Ola, qual eh o seu nome? ', 0
  msg1_len EQU $-msg1
  msg2      DB 0xA, 0xD, 'Ola '
  msg2_len EQU $-msg2
  msg3      DB 'Bem-vindo ao programa CALC IA-32', 0xA, 0xD
  msg3_len EQU $-msg3
  msg4      DB 0xA, 0xD, 'Escolha uma opcao:', 0xA, 0xD
  msg4_len EQU $-msg4
  msg5      DB '- (1) SOMA', 0xA, 0xD
  msg5_len EQU $-msg5
  msg6      DB '- (2) SUBTRACAO', 0xA, 0xD
  msg6_len EQU $-msg6
  msg7      DB '- (3) MULTIPLICACAO', 0xA, 0xD
  msg7_len EQU $-msg7
  msg8      DB '- (4) DIVISAO', 0xA, 0xD
  msg8_len EQU $-msg8
  msg9      DB '- (5) MODULO', 0xA, 0xD
  msg9_len EQU $-msg9
  msg10      DB '- (6) SAIR', 0xA, 0xD
  msg10_len EQU $-msg10
  msg11      DB 0xA, 0xD, 'Digite a opcao: ', 0
  msg11_len EQU $-msg11
  msg12      DB 'Digite o numero 1: ', 0
  msg12_len EQU $-msg12
  msg13      DB 'Digite o numero 2: ', 0
  msg13_len EQU $-msg13
  msg14      DB 'O resultado eh: ', 0
  msg14_len EQU $-msg14

section .bss
  name           RESB 10
  option         RESB 2
  enter_         RESB 1

  num1           RESB 10
  converted_num1 RESB 10

  num2           RESB 10
  converted_num2 RESB 10

  result         RESB 10

section .text

  global _start:
    _start:
      mov ECX, msg1
      mov EDX, msg1_len
      call @print

      mov ECX, name
      mov EDX, 10
      call @scan

      mov ECX, msg2
      mov EDX, msg2_len
      call @print

      mov ECX, name
      mov EDX, 10
      call @print

    _menu:
      mov ECX, msg3
      mov EDX, msg3_len
      call @print

      mov ECX, msg4
      mov EDX, msg4_len
      call @print

      mov ECX, msg5
      mov EDX, msg5_len
      call @print

      mov ECX, msg6
      mov EDX, msg6_len
      call @print

      mov ECX, msg7
      mov EDX, msg7_len
      call @print

      mov ECX, msg8
      mov EDX, msg8_len
      call @print

      mov ECX, msg9
      mov EDX, msg9_len
      call @print

      mov ECX, msg10
      mov EDX, msg10_len
      call @print

      mov ECX, msg11
      mov EDX, msg11_len
      call @print

      mov ECX, option
      mov EDX, 2
      call @scan

      cmp BYTE [option], '1'
      je _soma

      cmp BYTE [option], '2'
      je _subtracao

      cmp BYTE [option], '3'
      je _multiplicacao

      cmp BYTE [option], '4'
      je _divisao

      cmp BYTE [option], '5'
      je _modulo

      cmp BYTE [option], '6'
      je _start_exit
    
    @get_numbers:
      mov ECX, msg12
      mov EDX, msg12_len
      call @print

      mov ECX, num1
      mov EDX, 10
      call @scan

      mov ECX, msg13
      mov EDX, msg13_len
      call @print

      mov ECX, num2
      mov EDX, 10
      call @scan

      ret

    @show_result:
      lea ESI, [result]
      mov ECX, 10
      call @itoa

      mov ECX, msg14
      mov EDX, msg14_len
      call @print

      mov ECX, result
      mov EDX, 10
      call @print

      mov ECX, enter_
      mov EDX, 1
      call @scan

      ret

    _soma:
      call @get_numbers      

      mov ESI, num1
      call @atoi     ; eax = num1

      mov EBX, EAX   ; ebx = num1

      mov ESI, num2
      call @atoi     ; eax = num2

      add EAX, EBX   ; eax = num1 + num2

      call @show_result

      jmp _start_exit

    _subtracao:
      call @get_numbers

      mov ESI, num1
      call @atoi     ; eax = num1

      mov EBX, EAX   ; ebx = num1

      mov ESI, num2
      call @atoi     ; eax = num2

      sub EBX, EAX   ; ebx = num1 + num2
      mov EAX, EBX

      call @show_result

      jmp _start_exit

    _multiplicacao:
      call @get_numbers
      
      mov ESI, num1
      call @atoi     ; eax = num1

      mov EBX, EAX   ; ebx = num1

      mov ESI, num2
      call @atoi     ; eax = num2

      mul EBX        ; eax = eax * ebx -> num2 = num2 * num1

      call @show_result

      jmp _start_exit

    _divisao:
      call @get_numbers

      mov ESI, num1
      call @atoi    ; eax = num1

      mov EBX, EAX  ; ebx = num1
      
      mov ESI, num2
      call @atoi    ; eax = num2
      
      mov ECX, EAX  ; ecx = num2
      mov EAX, EBX  ; eax = num1

      xor EDX, EDX

      div ECX       ; eax = num1 / num2

      call @show_result

      jmp _start_exit

    _modulo:
      call @get_numbers

      mov ESI, num1
      call @atoi    ; eax = num1

      mov EBX, EAX  ; ebx = num1
      
      mov ESI, num2
      call @atoi    ; eax = num2
      
      mov ECX, EAX  ; ecx = num2
      mov EAX, EBX  ; eax = num1

      xor EDX, EDX

      div ECX       ; eax = num1 / num2
      movzx EAX, DL

      call @show_result

      jmp _start_exit

    _start_exit:
      mov EAX, 1
      mov EBX, 0
      int 80h

    @print:
      mov EAX, 4
      mov EBX, 1
      int 80h

      ret

    @scan:
      mov EAX, 3
      mov EBX, 0
      int 80h
      ret

    @atoi:
      ; input:
        ; string -> esi
      ; output:
        ; num -> eax
      
      xor EAX, EAX
      xor ECX, ECX

      .atoi_convert:
        movzx EDX, BYTE [ESI] ; get char
        inc ESI

        cmp EDX, '0'
        jl .atoi_exit
        cmp EDX, '9'
        jg .atoi_exit

        sub EDX, '0' ; char - '0'
        imul EAX, 10 ; eax = eax * 10
        add EAX, EDX ; eax = eax + char - '0'

        jmp .atoi_convert

      .atoi_exit:
        ret

    @itoa:
      ; input:
        ; num -> eax
        ; len -> ecx
      ; output:
        ; string -> esi

      add ESI, ECX
      dec ESI
      mov BYTE [ESI], 0
      mov EBX, 10

      .itoa_convert:
        xor EDX, EDX
        div EBX
        add DL, '0'
        dec ESI
        mov [ESI], DL
        test EAX, EAX
        jnz .itoa_convert

        ret
