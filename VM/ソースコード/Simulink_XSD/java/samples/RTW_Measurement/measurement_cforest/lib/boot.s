/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

	.file "boot.s"

	/* スタック領域パラメータ（適宜変更してください） */
	.EQU N_PE, 8
	.EQU STACK_SIZE, 1024*4

	/* PE0以外用スタック領域（PE1はGCCのcrt0で設定されるスタックを使用） */
	.comm	_stack_area, N_PE * STACK_SIZE, 4

	/* 例外ハンドラ */
	/* ここにRBASEが設定されることを想定（ここが電源投入時の開始アドレス） */
	/* リセット以外の要因はIntHandler関数@int_handler.cに行く */
	.section .reset.text
	.align 1
	.global _ResetVector
	.type	_ResetVector, @function
_ResetVector:
	jr _Bootup
	.REPT 6
	nop
	.ENDR
SYSERR: #010H 010H
	.REPT 8
	nop
	.ENDR
HVTRAP: #020H 020H
	.REPT 8
	nop
	.ENDR
FETRAP: #030H 030H
	.REPT 8
	nop
	.ENDR
TRAP0: #040H 040H
	.REPT 8
	nop
	.ENDR
TRAP1: #050H 050H
	.REPT 8
	nop
	.ENDR
RIE: #060H 060H
	.REPT 8
	nop
	.ENDR
FPPFPI: #070H 070H
	.REPT 8
	nop
	.ENDR
UCPOP: #080H 080H
	.REPT 8
	nop
	.ENDR
MIP: #090H 090H
	.REPT 8
	nop
	.ENDR
PIE: #0A0H 0A0H
	.REPT 8
	nop
	.ENDR
DEBUG: #0B0H 0B0H
	.REPT 8
	nop
	.ENDR
MAE: #0C0H 0C0H
	.REPT 8
	nop
	.ENDR
RFU: #0D0H 0D0H
	.REPT 8
	nop
	.ENDR
FENMI: #0E0H 0E0H
	.REPT 8
	nop
	.ENDR
FEINT: #0F0H 0F0H
	.REPT 8
	nop
	.ENDR
EIINTn0: #（優先度0） 100H
	.REPT 8
	nop
	.ENDR
EIINTn1: #（優先度1） 110H
	.REPT 8
	nop
	.ENDR
EIINTn2: #（優先度2） 120H
	.REPT 8
	nop
	.ENDR
EIINTn3: #（優先度3） 130H
	.REPT 8
	nop
	.ENDR
EIINTn4: #（優先度4） 140H
	.REPT 8
	nop
	.ENDR
EIINTn5: #（優先度5） 150H
	.REPT 8
	nop
	.ENDR
EIINTn6: #（優先度6） 160H
	.REPT 8
	nop
	.ENDR
EIINTn7: #（優先度7） 170H
	.REPT 8
	nop
	.ENDR
EIINTn8: #（優先度8） 180H
	.REPT 8
	nop
	.ENDR
EIINTn9: #（優先度9） 190H
	.REPT 8
	nop
	.ENDR
EIINTn10: #（優先度10） 1A0H
	.REPT 8
	nop
	.ENDR
EIINTn11: #（優先度11） 1B0H
	.REPT 8
	nop
	.ENDR
EIINTn12: #（優先度12） 1C0H
	.REPT 8
	nop
	.ENDR
EIINTn13: #（優先度13） 1D0H
	.REPT 8
	nop
	.ENDR
EIINTn14: #（優先度14） 1E0H
	.REPT 8
	nop
	.ENDR
EIINTn15: #（優先度15） 1F0H
    jr _Int_vetor
	.REPT 6
	nop
	.ENDR
	.size	_ResetVector, .-_ResetVector

	/* ブート処理 */
	/* PE1はGCCのcrt0を実行する */
	/* PE1以外はスタックポインタをr3に設定後、HALT。PE1からの割り込みを待つ*/
	.align 1
	.global _Bootup
	.extern _EBASE_ADDR
	.type	_Bootup, @function
_Bootup:
	movhi hi(_EBASE_ADDR),r0,r10
	movea lo(_EBASE_ADDR),r10,r10
	/* ldsr r10,sr3,1 */
	.long 0x08201FEA
	/* stsr sr0,r11,1 */
	.long 0x10405FE0
	shr 16, r11
	/* 仕様上は3bitだが、8PE対応のため4bitにする */
	/* andi 7, r11, r11 */
	andi 0x3f, r11, r11
	cmp 1,r11
	bne NotPE1
PE1:
	/* bss clear */
	movhi hi(__bss_start),r0,r10
        movea lo(__bss_start),r10,r10
        movhi hi(__bss_end),r0,r11
        movea lo(__bss_end),r11,r11
        cmp r11,r10
        bnl end_clear
clear_loop:
        st.w r0,0[r10]
        add 4,r10
        cmp r11,r10
        bl clear_loop
end_clear:
	movhi hi(_start),r0,r10
	movea lo(_start),r10,r10
	jarl .+4,r31 ; add 4,r31 ; jmp r10
	br GoHalt
NotPE1:
	movhi hi(_stack_area),r0,r3
	movea lo(_stack_area),r3,r3
	addi  -1, r11, r11
	mulhi STACK_SIZE, r11, r11
	add r11,r3
GoHalt:
	halt
	br GoHalt
	.size	_Bootup, .-_Bootup

	/* レジスタ退避、割り込みハンドラ呼び出し、レジスタ復帰、割り込みからの復帰 */
	.align 1
	.extern _IntHandler
	.global _Int_vetor
	.type	_Int_vetor, @function
_Int_vetor:
    /* レジスタ退避 r0~31以外のレジスタ（FPUレジスタなど）は未対応 */
    addi    -4, r3, r3
    st.w    r30, 0[r3]
    addi    -116, r3, r3
    mov     r3, r30
    sst.w   r1,   0[r30]
    sst.w   r2,   4[r30]
    sst.w   r4,   8[r30]
    sst.w   r5,  12[r30]
    sst.w   r6,  16[r30]
    sst.w   r7,  20[r30]
    sst.w   r8,  24[r30]
    sst.w   r9,  28[r30]
    sst.w   r10, 32[r30]
    sst.w   r11, 36[r30]
    sst.w   r12, 40[r30]
    sst.w   r13, 44[r30]
    sst.w   r14, 48[r30]
    sst.w   r15, 52[r30]
    sst.w   r16, 56[r30]
    sst.w   r17, 60[r30]
    sst.w   r18, 64[r30]
    sst.w   r19, 68[r30]
    sst.w   r20, 72[r30]
    sst.w   r21, 76[r30]
    sst.w   r22, 80[r30]
    sst.w   r23, 84[r30]
    sst.w   r24, 88[r30]
    sst.w   r25, 92[r30]
    sst.w   r26, 96[r30]
    sst.w   r27, 100[r30]
    sst.w   r28, 104[r30]
    sst.w   r29, 108[r30]
    sst.w   r31, 112[r30]

    /* 割り込みハンドラ呼び出し */
	stsr sr13,r6
	movhi hi(_IntHandler),r0,r10
	movea lo(_IntHandler),r10,r10
	jarl .+4,r31 ; add 4,r31 ; jmp r10

    /* レジスタ復帰 */
    mov      r3, r30
    sld.w    0[r30], r1
    sld.w    4[r30], r2
    sld.w    8[r30], r4
    sld.w   12[r30], r5
    sld.w   16[r30], r6
    sld.w   20[r30], r7
    sld.w   24[r30], r8
    sld.w   28[r30], r9
    sld.w   32[r30], r10
    sld.w   36[r30], r11
    sld.w   40[r30], r12
    sld.w   44[r30], r13
    sld.w   48[r30], r14
    sld.w   52[r30], r15
    sld.w   56[r30], r16
    sld.w   60[r30], r17
    sld.w   64[r30], r18
    sld.w   68[r30], r19
    sld.w   72[r30], r20
    sld.w   76[r30], r21
    sld.w   80[r30], r22
    sld.w   84[r30], r23
    sld.w   88[r30], r24
    sld.w   92[r30], r25
    sld.w   96[r30], r26
    sld.w   100[r30], r27
    sld.w   104[r30], r28
    sld.w   108[r30], r29
    sld.w   112[r30], r31
    addi    116, r3, r3
    ld.w    0[r3], r30
    addi    4, r3, r3
    /* 割り込みからの復帰 */
    /* eiret */
    .long 0x014807E0
	.size	_Int_vetor, .-_Int_vetor
