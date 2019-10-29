@
@********************************************************************************************************
@********************************************************************************************************
    .extern   g_stLosTask
    .extern   g_vuwIntCount
    .extern   g_uwCurNestCount
    .extern   osExcHandleEntry
    .extern  __svc_stack_top
    .extern  __stack_chk_guard
    .extern  random_stack_guard

    .global    LOS_Panic
    .global   _osExceptFiqHdl
    .global    _osExceptAddrAbortHdl
    .global    _osExceptDataAbortHdl
    .global    _osExceptPrefetchAbortHdl
    .global    _osExceptSwiHdl
    .global    _osExceptUndefInstrHdl
    .global    __stack_chk_guard_setup

@********************************************************************************************************
@                                                EQUATES
@********************************************************************************************************

.equ OS_PSR_THUMB                     ,     0x20                @ Thumb instruction set
.equ OS_PSR_INT_DIS                    ,     0xC0                @ Disable both FIQ and IRQ
.equ OS_PSR_FIQ_DIS                    ,     0x40                @ Disable FIQ
.equ OS_PSR_IRQ_DIS                   ,     0x80                @ Disable IRQ
.equ OS_PSR_MODE_MASK                  ,     0x1F
.equ OS_PSR_MODE_USR                  ,     0x10
.equ OS_PSR_MODE_FIQ                   ,     0x11
.equ OS_PSR_MODE_IRQ                   ,     0x12
.equ OS_PSR_MODE_SVC                   ,    0x13
.equ OS_PSR_MODE_ABT                   ,     0x17
.equ OS_PSR_MODE_UND                   ,     0x1B
.equ OS_PSR_MODE_SYS                   ,    0x1F

@ Define exception type ID
.equ OS_EXCEPT_RESET        ,     0x00
.equ OS_EXCEPT_UNDEF_INSTR   ,     0x01
.equ OS_EXCEPT_SWI           ,     0x02
.equ OS_EXCEPT_PREFETCH_ABORT   ,    0x03
.equ OS_EXCEPT_DATA_ABORT   ,     0x04
.equ OS_EXCEPT_FIQ     ,     0x05
.equ OS_EXCEPT_ADDR_ABORT ,    0x06
.equ OS_EXCEPT_IRQ   ,     0x07

@.fpu vfpv4
@.fpu neon
@.arch armv7a
@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************
LOS_Panic:
    svc	0x00000000
    nop
@********************************************************************************************************
@ Description: Stack-Protector Init
@ Parameter  : None
@********************************************************************************************************
__stack_chk_guard_setup:
    PUSH {FP, LR}
    BL random_stack_guard
    LDR R1, =__stack_chk_guard
    MOV R3, R0
    ORR R2, R3, #0X80000000
    STR R2,[R1]
    POP {FP, PC}

@********************************************************************************************************
@ Description: Undefined instruction exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptUndefInstrHdl:
                                                              @ LR offset to return from this exception:  0.
    STMFD   SP, {R0-R5}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_UNDEF_INSTR                        @ Set exception ID to OS_EXCEPT_UNDEF_INSTR.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Software interrupt exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptSwiHdl:
    MOV     R4,   SP
    LDR     SP, =__svc_stack_top
    STMFD   SP!, {LR}                                      @ Store PC
    STMFD   SP!, {LR}
    STMFD   SP!, {R4}
    STMFD   SP!, {R0-R12}                                  @ Store SP,LR,R0-R12

    MRS     R1, SPSR                                          @ Save exception`s CPSR.
    STMFD   SP!, {R1}                                         @ Push task`s CPSR (i.e. exception SPSR).

    MOV     R0, #OS_EXCEPT_SWI                                @ Set exception ID to OS_EXCEPT_SWI.

    B       _osExceptionSwi                                   @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Prefectch abort exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptPrefetchAbortHdl:
    SUB     LR, LR, #4                                        @ LR offset to return from this exception: -4.
    STMFD   SP, {R0-R5}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_PREFETCH_ABORT                     @ Set exception ID to OS_EXCEPT_PREFETCH_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Data abort exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptDataAbortHdl:
    SUB     LR, LR, #8                                        @ LR offset to return from this exception: -8.
    STMFD   SP, {R0-R5}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_DATA_ABORT                         @ Set exception ID to OS_EXCEPT_DATA_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Address abort exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptAddrAbortHdl:
    SUB     LR, LR, #8                                        @ LR offset to return from this exception: -8.
    STMFD   SP, {R0-R5}                                       @ Push working registers, but don`t change SP.

    MOV     R0, #OS_EXCEPT_ADDR_ABORT                         @ Set exception ID to OS_EXCEPT_ADDR_ABORT.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Fast interrupt request exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptFiqHdl:
    SUB     LR, LR, #4                                        @ LR offset to return from this exception: -4.
    STMFD   SP, {R0-R5}                                      @ Push working registers.

    MOV     R0, #OS_EXCEPT_FIQ                                @ Set exception ID to OS_EXCEPT_FIQ.

    B       _osExceptDispatch                                 @ Branch to global exception handler.

@********************************************************************************************************
@ Description: Exception handler
@ Parameter  : R0     Exception Type
@ Regs Hold  : R3     Exception`s CPSR
@********************************************************************************************************
_osExceptDispatch:
    MRS     R1, SPSR                                          @ Save CPSR before exception.
    MOV     R2, LR                                            @ Save PC before exception.
    SUB     R4, SP, #(6 * 4)                                  @ Save the start address of working registers.

    MSR     CPSR_c, #(OS_PSR_INT_DIS | OS_PSR_MODE_SVC)    @ Switch to SVC mode, and disable all interrupts
    MOV     R3, SP
    LDR     SP, =__svc_stack_top
    STMFD   SP!, {R2}                                                      @ Push Exception PC
    STMFD   SP!, {LR}
    STMFD   SP!, {R3}                                         @ Push original LR,
    STMFD   SP!, {R6-R12}                                     @ Push original R12-R6,
    LDMFD   R4!, {R6-R11}                                     @ Move original R5-R0 from exception stack to original stack.
    STMFD   SP!, {R6-R11}
    STMFD   SP!, {R1}                                         @ Push task`s CPSR (i.e. exception SPSR).


_osExceptionSwi:
    MOV     R1, SP

    LDR     R2, =g_uwCurNestCount                             @ if(g_uwCurNestCount > 0) dump to _osExceptionGetSP
    LDR     R4, [R2]

    CMP     R4, #0
    BNE     _osExceptionGetSP

    LDR     R5, =g_vuwIntCount                                 @ Judge the exception is occur in task stack or system stack
    LDR     R6, [R5]

    CMP     R6, #0                                             @ if (g_vuwStackSwitchFlag > 0)
    BNE     _osExceptionGetSP                                  @ can not switch svc stack

    LDR     R2, =g_stLosTask
    LDR     R4, [R2]
    CMP     R4, #0                                             @ the first task have not created
    BEQ     _osExceptionGetSP

    LDR     R4, [R2]
    STR     SP, [R4]                                          @ g_stLosTask.pstRunTask->pStackPointer = SP.

@ADD     R6, R6, #1 deleted by b00208046
@STR     R6, [R5]   deleted by b00208046

_osExceptionGetSP:

    LDR     R2, =osExcHandleEntry                             @ osExcHandleEntry(UINT32 uwExcType, EXC_CONTEXT_S * puwExcBufAddr)@

    MOV     LR, PC
    BX      R2

    .end

