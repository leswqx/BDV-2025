.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "..\Debug\StaticLibrary.lib"
ExitProcess PROTO :DWORD 
.stack 4096

 stringToUnsigned PROTO : DWORD, : DWORD

 unsignedToString PROTO : DWORD, : DWORD

 stringLength PROTO : DWORD, : DWORD

 stringConcat PROTO : DWORD, : DWORD, : DWORD

 outUint PROTO : DWORD

 outString PROTO : DWORD
.const
		newline byte 13, 10, 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		temp_str_buf byte 256 dup(0)
		v_x dword ?
		v_local dword ?
		L_2 dword 2
		v_y dword ?
		L_10 dword 10
		v_n dword ?
		L_1 dword 1
		L_LIT_1 byte "one", 0
		L_LIT_2 byte "two", 0
		L_LIT_3 byte "other", 0
		L_LIT_4 byte "Types and literals", 0
		v_decimal dword ?
		L_42 dword 42
		v_hex dword ?
		L_255 dword 255
		v_zero dword ?
		L_0 dword 0
		v_text dword ?
		v_text_buf byte 256 dup(0)
		L_LIT_5 byte "Hello", 0
		L_LIT_6 byte "decimal=42:", 0
		L_LIT_7 byte "hex=0xFF:", 0
		L_LIT_8 byte "text:", 0
		L_LIT_9 byte 0
		L_LIT_10 byte "Square & and power ^", 0
		v_a dword ?
		L_3 dword 3
		v_square dword ?
		v_power dword ?
		v_directSquare dword ?
		L_5 dword 5
		v_directPower dword ?
		L_LIT_11 byte "a=3", 0
		L_LIT_12 byte "a& =", 0
		L_LIT_13 byte "a^2 =", 0
		L_LIT_14 byte "5& =", 0
		L_LIT_15 byte "2^3 =", 0
		L_LIT_16 byte 0
		L_LIT_17 byte "All aritmetic operations", 0
		L_20 dword 20
		v_sum dword ?
		v_diff dword ?
		v_mul dword ?
		v_div dword ?
		v_mod dword ?
		L_LIT_18 byte "x=20, y=3", 0
		L_LIT_19 byte "x+y =", 0
		L_LIT_20 byte "x-y =", 0
		L_LIT_21 byte "x*y =", 0
		L_LIT_22 byte "x/y =", 0
		L_LIT_23 byte "x%y =", 0
		L_LIT_24 byte 0
		L_LIT_25 byte "Operator precedence", 0
		v_prio1 dword ?
		v_prio2 dword ?
		v_prio3 dword ?
		v_prio4 dword ?
		v_prio5 dword ?
		L_LIT_26 byte "2+3& =", 0
		L_LIT_27 byte "(2+3)& =", 0
		L_LIT_28 byte "2*3& =", 0
		L_LIT_29 byte "2^3*2 =", 0
		L_LIT_30 byte "10/2& =", 0
		L_LIT_31 byte 0
		L_LIT_32 byte "All comparison operators", 0
		v_cmp1 dword ?
		v_cmp2 dword ?
		v_cmp3 dword ?
		L_4 dword 4
		v_cmp4 dword ?
		v_cmp5 dword ?
		v_cmp6 dword ?
		L_LIT_33 byte "5>3 =", 0
		L_LIT_34 byte "2==2 =", 0
		L_LIT_35 byte "4!=5 =", 0
		L_LIT_36 byte "3<5 =", 0
		L_LIT_37 byte "5>=5 =", 0
		L_LIT_38 byte "2<=3 =", 0
		L_LIT_39 byte 0
		L_LIT_40 byte "User functions and scope", 0
		v_resA dword ?
		v_resB dword ?
		L_LIT_41 byte "funcA(4) =", 0
		L_LIT_42 byte "funcB(4) =", 0
		L_LIT_43 byte 0
		L_LIT_44 byte "While loop", 0
		v_i dword ?
		v_temp dword ?
		L_LIT_45 byte "i=", 0
		L_LIT_46 byte "i&=", 0
		L_LIT_47 byte 0
		L_LIT_48 byte "Switch statement", 0
		v_val dword ?
		L_LIT_49 byte "ONE", 0
		L_LIT_50 byte "TWO", 0
		L_LIT_51 byte "DONE", 0
		v_word dword ?
		v_word_buf byte 256 dup(0)
		v_wordDefault dword ?
		v_wordDefault_buf byte 256 dup(0)
		L_LIT_52 byte "getNum(2) =", 0
		L_LIT_53 byte "getNum(5) =", 0
		L_LIT_54 byte 0
		L_LIT_55 byte "All Static functions", 0
		v_concat dword ?
		v_concat_buf byte 256 dup(0)
		L_LIT_56 byte "Hello", 0
		L_LIT_57 byte "World", 0
		v_length dword ?
		L_LIT_58 byte "Hello", 0
		v_fromStr dword ?
		L_LIT_59 byte "123", 0
		v_toStr dword ?
		v_toStr_buf byte 256 dup(0)
		L_456 dword 456
		L_LIT_60 byte "stringConcat(Hello, World) =", 0
		L_LIT_61 byte "stringLength(Hello) =", 0
		L_LIT_62 byte "stringToUnsigned(123) =", 0
		L_LIT_63 byte "unsignedToString(456) =", 0
.code
;----------- fn_funcA ------------
fn_funcA PROC p_x : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_x
push L_2
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_local, eax
push v_local
pop eax
imul eax, eax
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
fn_funcA ENDP
;------------------------------

;----------- fn_funcB ------------
fn_funcB PROC p_y : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_y
push L_10
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_local, eax
push v_local
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_1_start
mov eax, 1
jmp pow_1_end
pow_1_start:
dec ecx
pow_1_loop:
cmp ecx, 0
je pow_1_end
imul eax, ebx
dec ecx
jmp pow_1_loop
pow_1_end:
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
fn_funcB ENDP
;------------------------------

;----------- fn_getNum ------------
fn_getNum PROC p_n : dword
; --- save registers ---
push ebx
push edx
; ----------------------
; --- switch ---
mov eax, p_n
cmp eax, 1
je case_1_0
cmp eax, 2
je case_1_1
jmp default_1
case_1_0:
push offset L_LIT_1
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_1:
push offset L_LIT_2
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
default_1:
push offset L_LIT_3
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
switch_1_end:
fn_getNum ENDP
;------------------------------

;----------- MAIN ------------
main PROC
push offset L_LIT_4
call outString
push offset newline
call outString
push L_42
pop eax
mov v_decimal, eax
push L_255
pop eax
mov v_hex, eax
push L_0
pop eax
mov v_zero, eax
mov v_text, offset L_LIT_5
push offset L_LIT_6
call outString
push offset newline
call outString
push v_decimal
call outUint
push offset newline
call outString
push offset L_LIT_7
call outString
push offset newline
call outString
push v_hex
call outUint
push offset newline
call outString
push offset L_LIT_8
call outString
push offset newline
call outString
push v_text
call outString
push offset newline
call outString
push offset L_LIT_9
call outString
push offset newline
call outString
push offset L_LIT_10
call outString
push offset newline
call outString
push L_3
pop eax
mov v_a, eax
push v_a
pop eax
imul eax, eax
push eax
pop eax
mov v_square, eax
push v_a
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_2_start
mov eax, 1
jmp pow_2_end
pow_2_start:
dec ecx
pow_2_loop:
cmp ecx, 0
je pow_2_end
imul eax, ebx
dec ecx
jmp pow_2_loop
pow_2_end:
push eax
pop eax
mov v_power, eax
push L_5
pop eax
imul eax, eax
push eax
pop eax
mov v_directSquare, eax
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_3_start
mov eax, 1
jmp pow_3_end
pow_3_start:
dec ecx
pow_3_loop:
cmp ecx, 0
je pow_3_end
imul eax, ebx
dec ecx
jmp pow_3_loop
pow_3_end:
push eax
pop eax
mov v_directPower, eax
push offset L_LIT_11
call outString
push offset newline
call outString
push offset L_LIT_12
call outString
push offset newline
call outString
push v_square
call outUint
push offset newline
call outString
push offset L_LIT_13
call outString
push offset newline
call outString
push v_power
call outUint
push offset newline
call outString
push offset L_LIT_14
call outString
push offset newline
call outString
push v_directSquare
call outUint
push offset newline
call outString
push offset L_LIT_15
call outString
push offset newline
call outString
push v_directPower
call outUint
push offset newline
call outString
push offset L_LIT_16
call outString
push offset newline
call outString
push offset L_LIT_17
call outString
push offset newline
call outString
push L_20
pop eax
mov v_x, eax
push L_3
pop eax
mov v_y, eax
push v_x
push v_y
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_sum, eax
push v_x
push v_y
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
mov v_diff, eax
push v_x
push v_y
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_mul, eax
push v_x
push v_y
pop ebx
pop eax
cmp ebx, 0
je divsafe_4_zero
xor edx, edx
div ebx
push eax
jmp divsafe_4_end
divsafe_4_zero:
push 0
divsafe_4_end:
pop eax
mov v_div, eax
push v_x
push v_y
pop ebx
pop eax
cmp ebx, 0
je modsafe_5_zero
xor edx, edx
div ebx
push edx
jmp modsafe_5_end
modsafe_5_zero:
push 0
modsafe_5_end:
pop eax
mov v_mod, eax
push offset L_LIT_18
call outString
push offset newline
call outString
push offset L_LIT_19
call outString
push offset newline
call outString
push v_sum
call outUint
push offset newline
call outString
push offset L_LIT_20
call outString
push offset newline
call outString
push v_diff
call outUint
push offset newline
call outString
push offset L_LIT_21
call outString
push offset newline
call outString
push v_mul
call outUint
push offset newline
call outString
push offset L_LIT_22
call outString
push offset newline
call outString
push v_div
call outUint
push offset newline
call outString
push offset L_LIT_23
call outString
push offset newline
call outString
push v_mod
call outUint
push offset newline
call outString
push offset L_LIT_24
call outString
push offset newline
call outString
push offset L_LIT_25
call outString
push offset newline
call outString
push L_2
push L_3
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_prio1, eax
push L_2
push L_3
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
pop eax
mov v_prio2, eax
push L_2
push L_3
pop eax
imul eax, eax
push eax
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_prio3, eax
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_6_start
mov eax, 1
jmp pow_6_end
pow_6_start:
dec ecx
pow_6_loop:
cmp ecx, 0
je pow_6_end
imul eax, ebx
dec ecx
jmp pow_6_loop
pow_6_end:
push eax
push L_2
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_prio4, eax
push L_10
push L_2
pop eax
imul eax, eax
push eax
pop ebx
pop eax
cmp ebx, 0
je divsafe_7_zero
xor edx, edx
div ebx
push eax
jmp divsafe_7_end
divsafe_7_zero:
push 0
divsafe_7_end:
pop eax
mov v_prio5, eax
push offset L_LIT_26
call outString
push offset newline
call outString
push v_prio1
call outUint
push offset newline
call outString
push offset L_LIT_27
call outString
push offset newline
call outString
push v_prio2
call outUint
push offset newline
call outString
push offset L_LIT_28
call outString
push offset newline
call outString
push v_prio3
call outUint
push offset newline
call outString
push offset L_LIT_29
call outString
push offset newline
call outString
push v_prio4
call outUint
push offset newline
call outString
push offset L_LIT_30
call outString
push offset newline
call outString
push v_prio5
call outUint
push offset newline
call outString
push offset L_LIT_31
call outString
push offset newline
call outString
push offset L_LIT_32
call outString
push offset newline
call outString
push L_5
push L_3
pop ebx
pop eax
cmp eax, ebx
jg cmp_true_kpppdmpd
mov eax, 0
jmp cmp_end_kpppdmpd
cmp_true_kpppdmpd:
mov eax, 1
cmp_end_kpppdmpd:
push eax
pop eax
mov v_cmp1, eax
push L_2
push L_2
pop ebx
pop eax
cmp eax, ebx
je cmp_true_kpppdmpdc
mov eax, 0
jmp cmp_end_kpppdmpdc
cmp_true_kpppdmpdc:
mov eax, 1
cmp_end_kpppdmpdc:
push eax
pop eax
mov v_cmp2, eax
push L_4
push L_5
pop ebx
pop eax
cmp eax, ebx
jne cmp_true_kpppdmpdcc
mov eax, 0
jmp cmp_end_kpppdmpdcc
cmp_true_kpppdmpdcc:
mov eax, 1
cmp_end_kpppdmpdcc:
push eax
pop eax
mov v_cmp3, eax
push L_3
push L_5
pop ebx
pop eax
cmp eax, ebx
jl cmp_true_kpppdmpdccc
mov eax, 0
jmp cmp_end_kpppdmpdccc
cmp_true_kpppdmpdccc:
mov eax, 1
cmp_end_kpppdmpdccc:
push eax
pop eax
mov v_cmp4, eax
push L_5
push L_5
pop ebx
pop eax
cmp eax, ebx
jge cmp_true_kpppdmpdcccc
mov eax, 0
jmp cmp_end_kpppdmpdcccc
cmp_true_kpppdmpdcccc:
mov eax, 1
cmp_end_kpppdmpdcccc:
push eax
pop eax
mov v_cmp5, eax
push L_2
push L_3
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kpppdmpdccccc
mov eax, 0
jmp cmp_end_kpppdmpdccccc
cmp_true_kpppdmpdccccc:
mov eax, 1
cmp_end_kpppdmpdccccc:
push eax
pop eax
mov v_cmp6, eax
push offset L_LIT_33
call outString
push offset newline
call outString
push v_cmp1
call outUint
push offset newline
call outString
push offset L_LIT_34
call outString
push offset newline
call outString
push v_cmp2
call outUint
push offset newline
call outString
push offset L_LIT_35
call outString
push offset newline
call outString
push v_cmp3
call outUint
push offset newline
call outString
push offset L_LIT_36
call outString
push offset newline
call outString
push v_cmp4
call outUint
push offset newline
call outString
push offset L_LIT_37
call outString
push offset newline
call outString
push v_cmp5
call outUint
push offset newline
call outString
push offset L_LIT_38
call outString
push offset newline
call outString
push v_cmp6
call outUint
push offset newline
call outString
push offset L_LIT_39
call outString
push offset newline
call outString
push offset L_LIT_40
call outString
push offset newline
call outString

push L_4
call fn_funcA
push eax
pop eax
mov v_resA, eax

push L_4
call fn_funcB
push eax
pop eax
mov v_resB, eax
push offset L_LIT_41
call outString
push offset newline
call outString
push v_resA
call outUint
push offset newline
call outString
push offset L_LIT_42
call outString
push offset newline
call outString
push v_resB
call outUint
push offset newline
call outString
push offset L_LIT_43
call outString
push offset newline
call outString
push offset L_LIT_44
call outString
push offset newline
call outString
push L_1
pop eax
mov v_i, eax
while_1_start:
push v_i
push L_2
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kpppdmpdcccccc
mov eax, 0
jmp cmp_end_kpppdmpdcccccc
cmp_true_kpppdmpdcccccc:
mov eax, 1
cmp_end_kpppdmpdcccccc:
push eax
pop eax
cmp eax, 0
je while_1_end
push v_i
pop eax
imul eax, eax
push eax
pop eax
mov v_temp, eax
push offset L_LIT_45
call outString
push offset newline
call outString
push v_i
call outUint
push offset newline
call outString
push offset L_LIT_46
call outString
push offset newline
call outString
push v_temp
call outUint
push offset newline
call outString
push v_i
push L_1
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_i, eax
jmp while_1_start
while_1_end:
push offset L_LIT_47
call outString
push offset newline
call outString
push offset L_LIT_48
call outString
push offset newline
call outString
push L_2
pop eax
mov v_val, eax
; --- switch ---
mov eax, v_val
cmp eax, 1
je case_2_0
cmp eax, 2
je case_2_1
jmp default_2
case_2_0:
push offset L_LIT_49
call outString
push offset newline
call outString
jmp switch_2_end
case_2_1:
push offset L_LIT_50
call outString
push offset newline
call outString
jmp switch_2_end
default_2:
push offset L_LIT_51
call outString
push offset newline
call outString
switch_2_end:

push L_2
call fn_getNum
mov v_word, eax

push L_5
call fn_getNum
mov v_wordDefault, eax
push offset L_LIT_52
call outString
push offset newline
call outString
push v_word
call outString
push offset newline
call outString
push offset L_LIT_53
call outString
push offset newline
call outString
push v_wordDefault
call outString
push offset newline
call outString
push offset L_LIT_54
call outString
push offset newline
call outString
push offset L_LIT_55
call outString
push offset newline
call outString

push offset L_LIT_57
push offset L_LIT_56
push offset buffer
call stringConcat
; Copy string from buffer to variable's buffer
push esi
push edi
mov esi, eax
mov edi, offset v_concat_buf
copy_15_loop:
mov al, [esi]
mov [edi], al
cmp al, 0
je copy_15_end
inc esi
inc edi
jmp copy_15_loop
copy_15_end:
pop edi
pop esi
mov v_concat, offset v_concat_buf

push offset L_LIT_58
push offset buffer
call stringLength
push eax
pop eax
mov v_length, eax

push offset L_LIT_59
push offset buffer
call stringToUnsigned
push eax
pop eax
mov v_fromStr, eax

push L_456
push offset buffer
call unsignedToString
; Copy string from buffer to variable's buffer
push esi
push edi
mov esi, eax
mov edi, offset v_toStr_buf
copy_16_loop:
mov al, [esi]
mov [edi], al
cmp al, 0
je copy_16_end
inc esi
inc edi
jmp copy_16_loop
copy_16_end:
pop edi
pop esi
mov v_toStr, offset v_toStr_buf
push offset L_LIT_60
call outString
push offset newline
call outString
push v_concat
call outString
push offset newline
call outString
push offset L_LIT_61
call outString
push offset newline
call outString
push v_length
call outUint
push offset newline
call outString
push offset L_LIT_62
call outString
push offset newline
call outString
push v_fromStr
call outUint
push offset newline
call outString
push offset L_LIT_63
call outString
push offset newline
call outString
push v_toStr
call outString
push offset newline
call outString

push 0
call ExitProcess
main ENDP
end main