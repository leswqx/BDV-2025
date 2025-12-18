.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "D:\COURSE_2\3_SEM\KPO\BDV-2025\Debug\StaticLibrary.lib"
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
		v_n dword ?
		v_result dword ?
		L_1 dword 1
		v_counter dword ?
		L_0 dword 0
		v_base dword ?
		v_exp dword ?
		v_x dword ?
		v_a dword ?
		v_b dword ?
		v_c dword ?
		L_2 dword 2
		v_name dword ?
		L_LIT_1 byte "Hello, ", 0
		L_LIT_2 byte "zero", 0
		L_LIT_3 byte "one", 0
		L_LIT_4 byte "two", 0
		L_3 dword 3
		L_LIT_5 byte "three", 0
		L_4 dword 4
		L_LIT_6 byte "four", 0
		L_5 dword 5
		L_LIT_7 byte "five", 0
		L_LIT_8 byte "many", 0
		L_LIT_9 byte 0
		L_LIT_10 byte "=== 1. TYPES AND LITERALS ===", 0
		v_decimal dword ?
		L_42 dword 42
		v_hexadecimal dword ?
		L_255 dword 255
		v_zero dword ?
		v_text dword ?
		L_LIT_11 byte "Hello World!", 0
		v_empty dword ?
		L_LIT_12 byte 0
		L_LIT_13 byte "Decimal:", 0
		L_LIT_14 byte "Hexadecimal:", 0
		L_LIT_15 byte "Zero:", 0
		L_LIT_16 byte "Text:", 0
		L_LIT_17 byte 0
		L_LIT_18 byte "=== 2. ARITHMETIC OPERATIONS ===", 0
		L_20 dword 20
		v_sum dword ?
		v_diff dword ?
		v_prod dword ?
		v_quot dword ?
		v_rem dword ?
		L_LIT_19 byte "a=20, b=3", 0
		L_LIT_20 byte "a+b:", 0
		L_LIT_21 byte "a-b:", 0
		L_LIT_22 byte "a*b:", 0
		L_LIT_23 byte "a/b:", 0
		L_LIT_24 byte "a%b:", 0
		L_LIT_25 byte 0
		L_LIT_26 byte "=== 3. SQUARE OPERATOR & ===", 0
		v_xSquare dword ?
		v_directSquare dword ?
		L_7 dword 7
		v_exprSquare dword ?
		v_nestedSquare dword ?
		v_combSquare dword ?
		L_LIT_27 byte "x=5", 0
		L_LIT_28 byte "x&:", 0
		L_LIT_29 byte "7&:", 0
		L_LIT_30 byte "(2+3)&:", 0
		L_LIT_31 byte "((5&)&):", 0
		L_LIT_32 byte "2&+3&+4&:", 0
		L_LIT_33 byte 0
		L_LIT_34 byte "=== 4. POWER OPERATOR ^ ===", 0
		v_pow1 dword ?
		v_pow2 dword ?
		v_pow3 dword ?
		L_10 dword 10
		v_pow4 dword ?
		v_powSquare dword ?
		L_LIT_35 byte "2^3:", 0
		L_LIT_36 byte "3^2:", 0
		L_LIT_37 byte "10^0:", 0
		L_LIT_38 byte "5^1:", 0
		L_LIT_39 byte "(2^3)&:", 0
		L_LIT_40 byte 0
		L_LIT_41 byte "=== 5. COMPARISON OPERATIONS ===", 0
		v_val1 dword ?
		v_val2 dword ?
		v_val3 dword ?
		v_eq dword ?
		v_neq dword ?
		v_gt dword ?
		v_lt dword ?
		v_gte dword ?
		v_lte dword ?
		L_LIT_42 byte "val1=10, val2=5, val3=10", 0
		L_LIT_43 byte "10==10:", 0
		L_LIT_44 byte "10!=5:", 0
		L_LIT_45 byte "10>5:", 0
		L_LIT_46 byte "5<10:", 0
		L_LIT_47 byte "10>=10:", 0
		L_LIT_48 byte "5<=10:", 0
		L_LIT_49 byte 0
		L_LIT_50 byte "=== 6. COMPLEX EXPRESSIONS ===", 0
		v_expr1 dword ?
		v_expr2 dword ?
		v_expr3 dword ?
		v_expr4 dword ?
		v_expr5 dword ?
		v_expr6 dword ?
		L_LIT_51 byte "2+3*4:", 0
		L_LIT_52 byte "(2+3)*4:", 0
		L_LIT_53 byte "10/2+3:", 0
		L_LIT_54 byte "2&*3+4&:", 0
		L_LIT_55 byte "(1+2)&*(3+4)&:", 0
		L_LIT_56 byte "2^3+5*2&:", 0
		L_LIT_57 byte 0
		L_LIT_58 byte "=== 7. STRING OPERATIONS ===", 0
		v_str1 dword ?
		L_LIT_59 byte "Hello", 0
		v_str2 dword ?
		L_LIT_60 byte "World", 0
		v_concat dword ?
		v_len1 dword ?
		v_len2 dword ?
		L_LIT_61 byte "str1:", 0
		L_LIT_62 byte "str2:", 0
		L_LIT_63 byte "concat:", 0
		L_LIT_64 byte "len(str1):", 0
		L_LIT_65 byte "len(concat):", 0
		v_numFromStr dword ?
		L_LIT_66 byte "123", 0
		v_strFromNum dword ?
		L_456 dword 456
		L_LIT_67 byte "stringToUnsigned(123):", 0
		L_LIT_68 byte "unsignedToString(456):", 0
		L_LIT_69 byte 0
		L_LIT_70 byte "=== 8. USER FUNCTIONS ===", 0
		v_fact5 dword ?
		v_pow23 dword ?
		v_sq7 dword ?
		v_pythagoras dword ?
		v_complex dword ?
		L_LIT_71 byte "factorial(5):", 0
		L_LIT_72 byte "power(2,10):", 0
		L_LIT_73 byte "square(7):", 0
		L_LIT_74 byte "sumOfSquares(3,4):", 0
		L_LIT_75 byte "complexCalc(5,3,2):", 0
		v_greeting dword ?
		L_LIT_76 byte "Alice", 0
		L_LIT_77 byte "greet(Alice):", 0
		L_LIT_78 byte 0
		L_LIT_79 byte "=== 9. WHILE LOOPS ===", 0
		L_LIT_80 byte "Countdown from 5:", 0
		L_LIT_81 byte "Sum 1 to 10:", 0
		v_i dword ?
		v_sumLoop dword ?
		L_LIT_82 byte "Squares 1 to 5:", 0
		v_j dword ?
		L_LIT_83 byte 0
		L_LIT_84 byte "=== 10. SWITCH STATEMENTS ===", 0
		L_LIT_85 byte "Testing switch with 0:", 0
		v_testVal dword ?
		L_LIT_86 byte "ZERO", 0
		L_LIT_87 byte "ONE", 0
		L_LIT_88 byte "TWO", 0
		L_LIT_89 byte "OTHER", 0
		L_LIT_90 byte "Testing switch with 5:", 0
		v_testVal2 dword ?
		L_LIT_91 byte "ZERO", 0
		L_LIT_92 byte "ONE", 0
		L_LIT_93 byte "TWO", 0
		L_LIT_94 byte "OTHER (default)", 0
		L_LIT_95 byte "Number to word:", 0
		v_num0 dword ?
		v_num3 dword ?
		v_num99 dword ?
		L_99 dword 99
		v_word0 dword ?
		v_word3 dword ?
		v_word99 dword ?
		L_LIT_96 byte 0
		L_LIT_97 byte "=== 11. NESTED STRUCTURES ===", 0
		L_LIT_98 byte "Nested expression:", 0
		v_nested dword ?
		L_LIT_99 byte "Nested loops - multiplication table 3:", 0
		v_mult dword ?
		L_LIT_100 byte 0
		L_LIT_101 byte "=== 12. EDGE CASES ===", 0
		v_zero1 dword ?
		v_one dword ?
		v_maxTest dword ?
		L_65535 dword 65535
		L_LIT_102 byte "0&:", 0
		L_LIT_103 byte "1&:", 0
		L_LIT_104 byte "0^5:", 0
		L_LIT_105 byte "5^0:", 0
		L_LIT_106 byte "1^100:", 0
		L_100 dword 100
		L_LIT_107 byte "Division by powers:", 0
		v_div1 dword ?
		v_div2 dword ?
		L_64 dword 64
		L_LIT_108 byte "100/2&:", 0
		L_LIT_109 byte "64/(2^3):", 0
		L_LIT_110 byte 0
		L_LIT_111 byte "=== 13. COMBINED OPERATIONS ===", 0
		L_LIT_112 byte "All operators:", 0
		v_allOps dword ?
		L_LIT_113 byte "Comparisons in expressions:", 0
		v_cmpExpr dword ?
		L_LIT_114 byte "Square of power:", 0
		v_sqPow dword ?
		L_LIT_115 byte "Power of square:", 0
		v_powSq dword ?
		L_LIT_116 byte 0
		L_LIT_117 byte "=== 14. FUNCTIONS SHOWCASE ===", 0
		L_LIT_118 byte "n=4", 0
		L_LIT_119 byte "factorial(n):", 0
		L_LIT_120 byte "square(n):", 0
		L_LIT_121 byte "power(n,2):", 0
		L_LIT_122 byte "sumOfSquares(n,3):", 0
.code
;----------- fn_factorial ------------
fn_factorial PROC p_n : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push L_1
pop eax
mov v_result, eax
push p_n
pop eax
mov v_counter, eax
while_1_start:
push v_counter
push L_0
pop ebx
pop eax
cmp eax, ebx
jg cmp_true_k
mov eax, 0
jmp cmp_end_k
cmp_true_k:
mov eax, 1
cmp_end_k:
push eax
pop eax
cmp eax, 0
je while_1_end
push v_result
push v_counter
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_result, eax
push v_counter
push L_1
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
mov v_counter, eax
jmp while_1_start
while_1_end:
push v_result
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
fn_factorial ENDP
;------------------------------

;----------- fn_power ------------
fn_power PROC p_base : dword, p_exp : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_base
push p_exp
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
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 8
fn_power ENDP
;------------------------------

;----------- fn_square ------------
fn_square PROC p_x : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_x
pop eax
imul eax, eax
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
fn_square ENDP
;------------------------------

;----------- fn_sumOfSquares ------------
fn_sumOfSquares PROC p_a : dword, p_b : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_a
pop eax
imul eax, eax
push eax
push p_b
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 8
fn_sumOfSquares ENDP
;------------------------------

;----------- fn_complexCalc ------------
fn_complexCalc PROC p_a : dword, p_b : dword, p_c : dword
; --- save registers ---
push ebx
push edx
; ----------------------
push p_a
push p_b
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
push p_c
pop ebx
pop eax
imul eax, ebx
push eax
push p_a
push L_2
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
pop ebx
pop eax
sub eax, ebx
push eax
push p_b
push p_c
pop ebx
pop eax
cmp ebx, 0
je modsafe_4_zero
xor edx, edx
div ebx
push edx
jmp modsafe_4_end
modsafe_4_zero:
push 0
modsafe_4_end:
pop ebx
pop eax
add eax, ebx
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 12
fn_complexCalc ENDP
;------------------------------

;----------- fn_greet ------------
fn_greet PROC p_name : dword
; --- save registers ---
push ebx
push edx
; ----------------------

push p_name
push offset L_LIT_1
push offset buffer
call stringConcat
push eax
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
fn_greet ENDP
;------------------------------

;----------- fn_numberToWord ------------
fn_numberToWord PROC p_n : dword
; --- save registers ---
push ebx
push edx
; ----------------------
; --- switch ---
mov eax, p_n
cmp eax, 0
je case_1_0
cmp eax, 1
je case_1_1
cmp eax, 2
je case_1_2
cmp eax, 3
je case_1_3
cmp eax, 4
je case_1_4
cmp eax, 5
je case_1_5
jmp default_1
case_1_0:
push offset L_LIT_2
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_1:
push offset L_LIT_3
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_2:
push offset L_LIT_4
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_3:
push offset L_LIT_5
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_4:
push offset L_LIT_6
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
case_1_5:
push offset L_LIT_7
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
jmp switch_1_end
default_1:
push offset L_LIT_8
pop eax
; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret 4
switch_1_end:
fn_numberToWord ENDP
;------------------------------

;----------- MAIN ------------
main PROC
push offset L_LIT_9
call outString
push offset newline
call outString
push offset L_LIT_10
call outString
push offset newline
call outString
push L_42
pop eax
mov v_decimal, eax
push L_255
pop eax
mov v_hexadecimal, eax
push L_0
pop eax
mov v_zero, eax
mov v_text, offset L_LIT_11
mov v_empty, offset L_LIT_12
push offset L_LIT_13
call outString
push offset newline
call outString
push v_decimal
call outUint
push offset newline
call outString
push offset L_LIT_14
call outString
push offset newline
call outString
push v_hexadecimal
call outUint
push offset newline
call outString
push offset L_LIT_15
call outString
push offset newline
call outString
push v_zero
call outUint
push offset newline
call outString
push offset L_LIT_16
call outString
push offset newline
call outString
push v_text
call outString
push offset newline
call outString
push offset L_LIT_17
call outString
push offset newline
call outString
push offset L_LIT_18
call outString
push offset newline
call outString
push L_20
pop eax
mov v_a, eax
push L_3
pop eax
mov v_b, eax
push v_a
push v_b
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_sum, eax
push v_a
push v_b
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
mov v_diff, eax
push v_a
push v_b
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_prod, eax
push v_a
push v_b
pop ebx
pop eax
cmp ebx, 0
je divsafe_5_zero
xor edx, edx
div ebx
push eax
jmp divsafe_5_end
divsafe_5_zero:
push 0
divsafe_5_end:
pop eax
mov v_quot, eax
push v_a
push v_b
pop ebx
pop eax
cmp ebx, 0
je modsafe_6_zero
xor edx, edx
div ebx
push edx
jmp modsafe_6_end
modsafe_6_zero:
push 0
modsafe_6_end:
pop eax
mov v_rem, eax
push offset L_LIT_19
call outString
push offset newline
call outString
push offset L_LIT_20
call outString
push offset newline
call outString
push v_sum
call outUint
push offset newline
call outString
push offset L_LIT_21
call outString
push offset newline
call outString
push v_diff
call outUint
push offset newline
call outString
push offset L_LIT_22
call outString
push offset newline
call outString
push v_prod
call outUint
push offset newline
call outString
push offset L_LIT_23
call outString
push offset newline
call outString
push v_quot
call outUint
push offset newline
call outString
push offset L_LIT_24
call outString
push offset newline
call outString
push v_rem
call outUint
push offset newline
call outString
push offset L_LIT_25
call outString
push offset newline
call outString
push offset L_LIT_26
call outString
push offset newline
call outString
push L_5
pop eax
mov v_x, eax
push v_x
pop eax
imul eax, eax
push eax
pop eax
mov v_xSquare, eax
push L_7
pop eax
imul eax, eax
push eax
pop eax
mov v_directSquare, eax
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
mov v_exprSquare, eax
push v_x
pop eax
imul eax, eax
push eax
pop eax
imul eax, eax
push eax
pop eax
mov v_nestedSquare, eax
push L_2
pop eax
imul eax, eax
push eax
push L_3
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
push L_4
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_combSquare, eax
push offset L_LIT_27
call outString
push offset newline
call outString
push offset L_LIT_28
call outString
push offset newline
call outString
push v_xSquare
call outUint
push offset newline
call outString
push offset L_LIT_29
call outString
push offset newline
call outString
push v_directSquare
call outUint
push offset newline
call outString
push offset L_LIT_30
call outString
push offset newline
call outString
push v_exprSquare
call outUint
push offset newline
call outString
push offset L_LIT_31
call outString
push offset newline
call outString
push v_nestedSquare
call outUint
push offset newline
call outString
push offset L_LIT_32
call outString
push offset newline
call outString
push v_combSquare
call outUint
push offset newline
call outString
push offset L_LIT_33
call outString
push offset newline
call outString
push offset L_LIT_34
call outString
push offset newline
call outString
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_7_start
mov eax, 1
jmp pow_7_end
pow_7_start:
dec ecx
pow_7_loop:
cmp ecx, 0
je pow_7_end
imul eax, ebx
dec ecx
jmp pow_7_loop
pow_7_end:
push eax
pop eax
mov v_pow1, eax
push L_3
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_8_start
mov eax, 1
jmp pow_8_end
pow_8_start:
dec ecx
pow_8_loop:
cmp ecx, 0
je pow_8_end
imul eax, ebx
dec ecx
jmp pow_8_loop
pow_8_end:
push eax
pop eax
mov v_pow2, eax
push L_10
push L_0
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_9_start
mov eax, 1
jmp pow_9_end
pow_9_start:
dec ecx
pow_9_loop:
cmp ecx, 0
je pow_9_end
imul eax, ebx
dec ecx
jmp pow_9_loop
pow_9_end:
push eax
pop eax
mov v_pow3, eax
push L_5
push L_1
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_10_start
mov eax, 1
jmp pow_10_end
pow_10_start:
dec ecx
pow_10_loop:
cmp ecx, 0
je pow_10_end
imul eax, ebx
dec ecx
jmp pow_10_loop
pow_10_end:
push eax
pop eax
mov v_pow4, eax
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_11_start
mov eax, 1
jmp pow_11_end
pow_11_start:
dec ecx
pow_11_loop:
cmp ecx, 0
je pow_11_end
imul eax, ebx
dec ecx
jmp pow_11_loop
pow_11_end:
push eax
pop eax
imul eax, eax
push eax
pop eax
mov v_powSquare, eax
push offset L_LIT_35
call outString
push offset newline
call outString
push v_pow1
call outUint
push offset newline
call outString
push offset L_LIT_36
call outString
push offset newline
call outString
push v_pow2
call outUint
push offset newline
call outString
push offset L_LIT_37
call outString
push offset newline
call outString
push v_pow3
call outUint
push offset newline
call outString
push offset L_LIT_38
call outString
push offset newline
call outString
push v_pow4
call outUint
push offset newline
call outString
push offset L_LIT_39
call outString
push offset newline
call outString
push v_powSquare
call outUint
push offset newline
call outString
push offset L_LIT_40
call outString
push offset newline
call outString
push offset L_LIT_41
call outString
push offset newline
call outString
push L_10
pop eax
mov v_val1, eax
push L_5
pop eax
mov v_val2, eax
push L_10
pop eax
mov v_val3, eax
push v_val1
push v_val3
pop ebx
pop eax
cmp eax, ebx
je cmp_true_kcppmdmppppp
mov eax, 0
jmp cmp_end_kcppmdmppppp
cmp_true_kcppmdmppppp:
mov eax, 1
cmp_end_kcppmdmppppp:
push eax
pop eax
mov v_eq, eax
push v_val1
push v_val2
pop ebx
pop eax
cmp eax, ebx
jne cmp_true_kcppmdmpppppc
mov eax, 0
jmp cmp_end_kcppmdmpppppc
cmp_true_kcppmdmpppppc:
mov eax, 1
cmp_end_kcppmdmpppppc:
push eax
pop eax
mov v_neq, eax
push v_val1
push v_val2
pop ebx
pop eax
cmp eax, ebx
jg cmp_true_kcppmdmpppppcc
mov eax, 0
jmp cmp_end_kcppmdmpppppcc
cmp_true_kcppmdmpppppcc:
mov eax, 1
cmp_end_kcppmdmpppppcc:
push eax
pop eax
mov v_gt, eax
push v_val2
push v_val1
pop ebx
pop eax
cmp eax, ebx
jl cmp_true_kcppmdmpppppccc
mov eax, 0
jmp cmp_end_kcppmdmpppppccc
cmp_true_kcppmdmpppppccc:
mov eax, 1
cmp_end_kcppmdmpppppccc:
push eax
pop eax
mov v_lt, eax
push v_val1
push v_val3
pop ebx
pop eax
cmp eax, ebx
jge cmp_true_kcppmdmpppppcccc
mov eax, 0
jmp cmp_end_kcppmdmpppppcccc
cmp_true_kcppmdmpppppcccc:
mov eax, 1
cmp_end_kcppmdmpppppcccc:
push eax
pop eax
mov v_gte, eax
push v_val2
push v_val1
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kcppmdmpppppccccc
mov eax, 0
jmp cmp_end_kcppmdmpppppccccc
cmp_true_kcppmdmpppppccccc:
mov eax, 1
cmp_end_kcppmdmpppppccccc:
push eax
pop eax
mov v_lte, eax
push offset L_LIT_42
call outString
push offset newline
call outString
push offset L_LIT_43
call outString
push offset newline
call outString
push v_eq
call outUint
push offset newline
call outString
push offset L_LIT_44
call outString
push offset newline
call outString
push v_neq
call outUint
push offset newline
call outString
push offset L_LIT_45
call outString
push offset newline
call outString
push v_gt
call outUint
push offset newline
call outString
push offset L_LIT_46
call outString
push offset newline
call outString
push v_lt
call outUint
push offset newline
call outString
push offset L_LIT_47
call outString
push offset newline
call outString
push v_gte
call outUint
push offset newline
call outString
push offset L_LIT_48
call outString
push offset newline
call outString
push v_lte
call outUint
push offset newline
call outString
push offset L_LIT_49
call outString
push offset newline
call outString
push offset L_LIT_50
call outString
push offset newline
call outString
push L_2
push L_3
push L_4
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_expr1, eax
push L_2
push L_3
pop ebx
pop eax
add eax, ebx
push eax
push L_4
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_expr2, eax
push L_10
push L_2
pop ebx
pop eax
cmp ebx, 0
je divsafe_18_zero
xor edx, edx
div ebx
push eax
jmp divsafe_18_end
divsafe_18_zero:
push 0
divsafe_18_end:
push L_3
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_expr3, eax
push L_2
pop eax
imul eax, eax
push eax
push L_3
pop ebx
pop eax
imul eax, ebx
push eax
push L_4
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_expr4, eax
push L_1
push L_2
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
push L_3
push L_4
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_expr5, eax
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_19_start
mov eax, 1
jmp pow_19_end
pow_19_start:
dec ecx
pow_19_loop:
cmp ecx, 0
je pow_19_end
imul eax, ebx
dec ecx
jmp pow_19_loop
pow_19_end:
push eax
push L_5
push L_2
pop eax
imul eax, eax
push eax
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_expr6, eax
push offset L_LIT_51
call outString
push offset newline
call outString
push v_expr1
call outUint
push offset newline
call outString
push offset L_LIT_52
call outString
push offset newline
call outString
push v_expr2
call outUint
push offset newline
call outString
push offset L_LIT_53
call outString
push offset newline
call outString
push v_expr3
call outUint
push offset newline
call outString
push offset L_LIT_54
call outString
push offset newline
call outString
push v_expr4
call outUint
push offset newline
call outString
push offset L_LIT_55
call outString
push offset newline
call outString
push v_expr5
call outUint
push offset newline
call outString
push offset L_LIT_56
call outString
push offset newline
call outString
push v_expr6
call outUint
push offset newline
call outString
push offset L_LIT_57
call outString
push offset newline
call outString
push offset L_LIT_58
call outString
push offset newline
call outString
mov v_str1, offset L_LIT_59
mov v_str2, offset L_LIT_60

push v_str2
push v_str1
push offset buffer
call stringConcat
mov v_concat, eax

push v_str1
push offset buffer
call stringLength
push eax
pop eax
mov v_len1, eax

push v_concat
push offset buffer
call stringLength
push eax
pop eax
mov v_len2, eax
push offset L_LIT_61
call outString
push offset newline
call outString
push v_str1
call outString
push offset newline
call outString
push offset L_LIT_62
call outString
push offset newline
call outString
push v_str2
call outString
push offset newline
call outString
push offset L_LIT_63
call outString
push offset newline
call outString
push v_concat
call outString
push offset newline
call outString
push offset L_LIT_64
call outString
push offset newline
call outString
push v_len1
call outUint
push offset newline
call outString
push offset L_LIT_65
call outString
push offset newline
call outString
push v_len2
call outUint
push offset newline
call outString

push offset L_LIT_66
push offset buffer
call stringToUnsigned
push eax
pop eax
mov v_numFromStr, eax

push L_456
push offset buffer
call unsignedToString
mov v_strFromNum, eax
push offset L_LIT_67
call outString
push offset newline
call outString
push v_numFromStr
call outUint
push offset newline
call outString
push offset L_LIT_68
call outString
push offset newline
call outString
push v_strFromNum
call outString
push offset newline
call outString
push offset L_LIT_69
call outString
push offset newline
call outString
push offset L_LIT_70
call outString
push offset newline
call outString

push L_5
call fn_factorial
push eax
pop eax
mov v_fact5, eax

push L_10
push L_2
call fn_power
push eax
pop eax
mov v_pow23, eax

push L_7
call fn_square
push eax
pop eax
mov v_sq7, eax

push L_4
push L_3
call fn_sumOfSquares
push eax
pop eax
mov v_pythagoras, eax

push L_2
push L_3
push L_5
call fn_complexCalc
push eax
pop eax
mov v_complex, eax
push offset L_LIT_71
call outString
push offset newline
call outString
push v_fact5
call outUint
push offset newline
call outString
push offset L_LIT_72
call outString
push offset newline
call outString
push v_pow23
call outUint
push offset newline
call outString
push offset L_LIT_73
call outString
push offset newline
call outString
push v_sq7
call outUint
push offset newline
call outString
push offset L_LIT_74
call outString
push offset newline
call outString
push v_pythagoras
call outUint
push offset newline
call outString
push offset L_LIT_75
call outString
push offset newline
call outString
push v_complex
call outUint
push offset newline
call outString

push offset L_LIT_76
call fn_greet
mov v_greeting, eax
push offset L_LIT_77
call outString
push offset newline
call outString
push v_greeting
call outString
push offset newline
call outString
push offset L_LIT_78
call outString
push offset newline
call outString
push offset L_LIT_79
call outString
push offset newline
call outString
push offset L_LIT_80
call outString
push offset newline
call outString
push L_5
pop eax
mov v_counter, eax
while_2_start:
push v_counter
push L_0
pop ebx
pop eax
cmp eax, ebx
jg cmp_true_kcppmdmpppppccccccdp
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdp
cmp_true_kcppmdmpppppccccccdp:
mov eax, 1
cmp_end_kcppmdmpppppccccccdp:
push eax
pop eax
cmp eax, 0
je while_2_end
push v_counter
call outUint
push offset newline
call outString
push v_counter
push L_1
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
mov v_counter, eax
jmp while_2_start
while_2_end:
push offset L_LIT_81
call outString
push offset newline
call outString
push L_1
pop eax
mov v_i, eax
push L_0
pop eax
mov v_sumLoop, eax
while_3_start:
push v_i
push L_10
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kcppmdmpppppccccccdpc
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpc
cmp_true_kcppmdmpppppccccccdpc:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpc:
push eax
pop eax
cmp eax, 0
je while_3_end
push v_sumLoop
push v_i
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_sumLoop, eax
push v_i
push L_1
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_i, eax
jmp while_3_start
while_3_end:
push v_sumLoop
call outUint
push offset newline
call outString
push offset L_LIT_82
call outString
push offset newline
call outString
push L_1
pop eax
mov v_j, eax
while_4_start:
push v_j
push L_5
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kcppmdmpppppccccccdpcc
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpcc
cmp_true_kcppmdmpppppccccccdpcc:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpcc:
push eax
pop eax
cmp eax, 0
je while_4_end
push v_j
pop eax
imul eax, eax
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push v_j
push L_1
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_j, eax
jmp while_4_start
while_4_end:
push offset L_LIT_83
call outString
push offset newline
call outString
push offset L_LIT_84
call outString
push offset newline
call outString
push offset L_LIT_85
call outString
push offset newline
call outString
push L_0
pop eax
mov v_testVal, eax
; --- switch ---
mov eax, v_testVal
cmp eax, 0
je case_2_0
cmp eax, 1
je case_2_1
cmp eax, 2
je case_2_2
jmp default_2
case_2_0:
push offset L_LIT_86
call outString
push offset newline
call outString
jmp switch_2_end
case_2_1:
push offset L_LIT_87
call outString
push offset newline
call outString
jmp switch_2_end
case_2_2:
push offset L_LIT_88
call outString
push offset newline
call outString
jmp switch_2_end
default_2:
push offset L_LIT_89
call outString
push offset newline
call outString
switch_2_end:
push offset L_LIT_90
call outString
push offset newline
call outString
push L_5
pop eax
mov v_testVal2, eax
; --- switch ---
mov eax, v_testVal2
cmp eax, 0
je case_3_0
cmp eax, 1
je case_3_1
cmp eax, 2
je case_3_2
jmp default_3
case_3_0:
push offset L_LIT_91
call outString
push offset newline
call outString
jmp switch_3_end
case_3_1:
push offset L_LIT_92
call outString
push offset newline
call outString
jmp switch_3_end
case_3_2:
push offset L_LIT_93
call outString
push offset newline
call outString
jmp switch_3_end
default_3:
push offset L_LIT_94
call outString
push offset newline
call outString
switch_3_end:
push offset L_LIT_95
call outString
push offset newline
call outString
push L_0
pop eax
mov v_num0, eax
push L_3
pop eax
mov v_num3, eax
push L_99
pop eax
mov v_num99, eax

push v_num0
call fn_numberToWord
mov v_word0, eax

push v_num3
call fn_numberToWord
mov v_word3, eax

push v_num99
call fn_numberToWord
mov v_word99, eax
push v_word0
call outString
push offset newline
call outString
push v_word3
call outString
push offset newline
call outString
push v_word99
call outString
push offset newline
call outString
push offset L_LIT_96
call outString
push offset newline
call outString
push offset L_LIT_97
call outString
push offset newline
call outString
push offset L_LIT_98
call outString
push offset newline
call outString
push L_2
push L_3
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
push L_4
push L_1
pop ebx
pop eax
sub eax, ebx
push eax
pop eax
imul eax, eax
push eax
pop ebx
pop eax
imul eax, ebx
push eax
push L_5
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_23_start
mov eax, 1
jmp pow_23_end
pow_23_start:
dec ecx
pow_23_loop:
cmp ecx, 0
je pow_23_end
imul eax, ebx
dec ecx
jmp pow_23_loop
pow_23_end:
push eax
pop eax
imul eax, eax
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_nested, eax
push v_nested
call outUint
push offset newline
call outString
push offset L_LIT_99
call outString
push offset newline
call outString
push L_1
pop eax
mov v_mult, eax
while_5_start:
push v_mult
push L_5
pop ebx
pop eax
cmp eax, ebx
jle cmp_true_kcppmdmpppppccccccdpcccp
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpcccp
cmp_true_kcppmdmpppppccccccdpcccp:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpcccp:
push eax
pop eax
cmp eax, 0
je while_5_end
push L_3
push v_mult
pop ebx
pop eax
imul eax, ebx
push eax
pop eax
mov v_result, eax
push v_result
call outUint
push offset newline
call outString
push v_mult
push L_1
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_mult, eax
jmp while_5_start
while_5_end:
push offset L_LIT_100
call outString
push offset newline
call outString
push offset L_LIT_101
call outString
push offset newline
call outString
push L_0
pop eax
mov v_zero1, eax
push L_1
pop eax
mov v_one, eax
push L_65535
pop eax
mov v_maxTest, eax
push offset L_LIT_102
call outString
push offset newline
call outString
push v_zero1
pop eax
imul eax, eax
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_103
call outString
push offset newline
call outString
push v_one
pop eax
imul eax, eax
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_104
call outString
push offset newline
call outString
push v_zero1
push L_5
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_25_start
mov eax, 1
jmp pow_25_end
pow_25_start:
dec ecx
pow_25_loop:
cmp ecx, 0
je pow_25_end
imul eax, ebx
dec ecx
jmp pow_25_loop
pow_25_end:
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_105
call outString
push offset newline
call outString
push L_5
push v_zero1
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_26_start
mov eax, 1
jmp pow_26_end
pow_26_start:
dec ecx
pow_26_loop:
cmp ecx, 0
je pow_26_end
imul eax, ebx
dec ecx
jmp pow_26_loop
pow_26_end:
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_106
call outString
push offset newline
call outString
push v_one
push L_100
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_27_start
mov eax, 1
jmp pow_27_end
pow_27_start:
dec ecx
pow_27_loop:
cmp ecx, 0
je pow_27_end
imul eax, ebx
dec ecx
jmp pow_27_loop
pow_27_end:
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_107
call outString
push offset newline
call outString
push L_100
push L_2
pop eax
imul eax, eax
push eax
pop ebx
pop eax
cmp ebx, 0
je divsafe_28_zero
xor edx, edx
div ebx
push eax
jmp divsafe_28_end
divsafe_28_zero:
push 0
divsafe_28_end:
pop eax
mov v_div1, eax
push L_64
push L_2
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_29_start
mov eax, 1
jmp pow_29_end
pow_29_start:
dec ecx
pow_29_loop:
cmp ecx, 0
je pow_29_end
imul eax, ebx
dec ecx
jmp pow_29_loop
pow_29_end:
push eax
pop ebx
pop eax
cmp ebx, 0
je divsafe_30_zero
xor edx, edx
div ebx
push eax
jmp divsafe_30_end
divsafe_30_zero:
push 0
divsafe_30_end:
pop eax
mov v_div2, eax
push offset L_LIT_108
call outString
push offset newline
call outString
push v_div1
call outUint
push offset newline
call outString
push offset L_LIT_109
call outString
push offset newline
call outString
push v_div2
call outUint
push offset newline
call outString
push offset L_LIT_110
call outString
push offset newline
call outString
push offset L_LIT_111
call outString
push offset newline
call outString
push offset L_LIT_112
call outString
push offset newline
call outString
push L_10
push L_5
pop ebx
pop eax
add eax, ebx
push eax
pop eax
imul eax, eax
push eax
push L_2
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_31_start
mov eax, 1
jmp pow_31_end
pow_31_start:
dec ecx
pow_31_loop:
cmp ecx, 0
je pow_31_end
imul eax, ebx
dec ecx
jmp pow_31_loop
pow_31_end:
push eax
pop ebx
pop eax
imul eax, ebx
push eax
push L_3
pop eax
imul eax, eax
push eax
pop ebx
pop eax
sub eax, ebx
push eax
push L_100
push L_4
pop ebx
pop eax
cmp ebx, 0
je divsafe_32_zero
xor edx, edx
div ebx
push eax
jmp divsafe_32_end
divsafe_32_zero:
push 0
divsafe_32_end:
push L_7
pop ebx
pop eax
cmp ebx, 0
je modsafe_33_zero
xor edx, edx
div ebx
push edx
jmp modsafe_33_end
modsafe_33_zero:
push 0
modsafe_33_end:
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_allOps, eax
push v_allOps
call outUint
push offset newline
call outString
push offset L_LIT_113
call outString
push offset newline
call outString
push L_5
push L_3
pop ebx
pop eax
cmp eax, ebx
jg cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdm
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdm
cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdm:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdm:
push eax
push L_10
push L_10
pop ebx
pop eax
cmp eax, ebx
je cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdmc
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdmc
cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdmc:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdmc:
push eax
pop ebx
pop eax
add eax, ebx
push eax
push L_7
push L_7
pop ebx
pop eax
cmp eax, ebx
jne cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdmcc
mov eax, 0
jmp cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdmcc
cmp_true_kcppmdmpppppccccccdpcccpcpppdpdpdmcc:
mov eax, 1
cmp_end_kcppmdmpppppccccccdpcccpcpppdpdpdmcc:
push eax
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov v_cmpExpr, eax
push v_cmpExpr
call outUint
push offset newline
call outString
push offset L_LIT_114
call outString
push offset newline
call outString
push L_3
push L_3
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_37_start
mov eax, 1
jmp pow_37_end
pow_37_start:
dec ecx
pow_37_loop:
cmp ecx, 0
je pow_37_end
imul eax, ebx
dec ecx
jmp pow_37_loop
pow_37_end:
push eax
pop eax
imul eax, eax
push eax
pop eax
mov v_sqPow, eax
push v_sqPow
call outUint
push offset newline
call outString
push offset L_LIT_115
call outString
push offset newline
call outString
push L_3
pop eax
imul eax, eax
push eax
push L_2
pop ecx
pop eax
mov ebx, eax
cmp ecx, 0
jne pow_38_start
mov eax, 1
jmp pow_38_end
pow_38_start:
dec ecx
pow_38_loop:
cmp ecx, 0
je pow_38_end
imul eax, ebx
dec ecx
jmp pow_38_loop
pow_38_end:
push eax
pop eax
mov v_powSq, eax
push v_powSq
call outUint
push offset newline
call outString
push offset L_LIT_116
call outString
push offset newline
call outString
push offset L_LIT_117
call outString
push offset newline
call outString
push L_4
pop eax
mov v_n, eax
push offset L_LIT_118
call outString
push offset newline
call outString
push offset L_LIT_119
call outString
push offset newline
call outString

push v_n
call fn_factorial
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_120
call outString
push offset newline
call outString

push v_n
call fn_square
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_121
call outString
push offset newline
call outString

push L_2
push v_n
call fn_power
push eax
pop eax
push eax
call outUint
push offset newline
call outString
push offset L_LIT_122
call outString
push offset newline
call outString

push L_3
push v_n
call fn_sumOfSquares
push eax
pop eax
push eax
call outUint
push offset newline
call outString

push 0
call ExitProcess
main ENDP
end main