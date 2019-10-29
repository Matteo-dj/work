    .extern   g_stLosTask
    .extern   g_vuwIntCount
    .extern   g_usUniTaskLock
    .extern   osExceptSchedule

    .global   _osLoadNewTask
    .global   osTaskSchedule
    .global   _TaskSwitch
    .global   _osExceptIrqHdl
    .global   osStartToRun
    .global  osIrqDisable
    .global  osIrqEnable
    .extern  OSRunning
    .extern  OSPrioCur
    .extern  OSPrioHighRdy
    .extern  OSTCBCur
    .extern  OSTCBHighRdy
    .extern  OSIntNesting
    .extern  OSIntExit
    .extern  OSTaskSwHook
    .extern  _TaskSwitch
    .extern  m_astHwiForm
    .extern __svc_stack_top
    .extern __irq_stack_top
    .extern __irq2_stack_top
    .extern osTaskSwitchCheck
    .extern hal_IRQ_handler
    .extern g_curirqnum

@********************************************************************************************************
@                                                EQUATES
@********************************************************************************************************

.equ OS_PSR_THUMB, 0x20                @ Thumb instruction set
.equ OS_PSR_INT_DIS, 0xC0                @ Disable both FIQ and IRQ
.equ OS_PSR_FIQ_DIS, 0x40                @ Disable FIQ
.equ OS_PSR_IRQ_DIS, 0x80                @ Disable IRQ
.equ OS_PSR_MODE_MASK, 0x1F
.equ OS_PSR_MODE_USR, 0x10
.equ OS_PSR_MODE_FIQ, 0x11
.equ OS_PSR_MODE_IRQ, 0x12
.equ OS_PSR_MODE_SVC, 0x13
.equ OS_PSR_MODE_ABT, 0x17
.equ OS_PSR_MODE_UND, 0x1B
.equ OS_PSR_MODE_SYS, 0x1F
.equ OS_REG_A7_PERI_GIC_DIST ,0x1000
.equ OS_REG_A7_PERI_GIC_CPU ,0x2000
.equ OS_REG_BASE_A7_PERI ,0x20300000
.equ OS_CFG_GIC_CPU_BASE ,0x20302000 @(REG_BASE_A7_PERI + REG_A7_PERI_GIC_CPU)
.equ OS_CFG_GIC_DIST_BASE ,0x20301000 @(REG_BASE_A7_PERI + REG_A7_PERI_GIC_DIST)

@ Define Vector Addr
@.equ OS_VECTOR_ADDR, 0xFFFFFF00

.equ TASK_STATUS_RUNNING, 0x0010
.equ OS_FLG_HWI_ACTIVE, 0x0001
.equ OS_ARM_PSR_THUMB, 0x20

@.fpu vfpv4
@.fpu neon
@.arch armv7a

osIrqDisable:
    MRS     R0, CPSR
    ORR     R1, R0, #0xC0                           @Set IRQ bit in CPSR to disable IRQs.
    MSR     CPSR_c, R1
    BX      LR

osIrqEnable:
    MRS     R0, CPSR
    BIC     R1, R0, #0xC0                           @Clear IRQ bit in CPSR to enable IRQs.
    MSR     CPSR_c, R1
    BX      LR

osStartToRun:

    MSR    CPSR_c, #(OS_PSR_INT_DIS | OS_PSR_FIQ_DIS | OS_PSR_MODE_SVC)

    LDR     R0, =g_bTaskScheduled
    MOV     R1, #1
    STR     R1, [R0]

    @ g_stLosTask.pstRunTask = g_stLosTask.pstNewTask, if g_stLosTask.pstRunTask is  NULL, will fall to error
    LDR     R0, =g_stLosTask
    LDR     R0, [R0, #4]
    LDR     R1, =g_stLosTask
    STR     R0, [R1]

    @VPUSH   {S0}                                     @fpu
    @VPOP    {S0}
    @VPUSH   {D0}
    @VPOP    {D0}

    B 	   osTaskContextSwitch

osTaskSchedule:
    STMFD  SP!, {LR}  @ PC
    STMFD  SP!, {LR}  @ LR
    STMFD  SP!, {R0-R12}
    MRS    R0, CPSR

    MRS    R1, CPSR
    ORR    R1, #OS_PSR_IRQ_DIS
    MSR    CPSR,R1

    TST    LR, #1
    ORRNE  R0, R0, #OS_ARM_PSR_THUMB
    STMFD  SP!, {R0}
    @VSTMDB  SP!, {D16-D31}                                     @fpu
    @VSTMDB  SP!, {D0-D15}                                     @fpu

    LDR    R0, =g_stLosTask
    LDR    R1, [R0]
    STR    SP, [R1]

    B      osTaskContextSwitch

@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************

    .text

@********************************************************************************************************
@ Description: Interrupt request exception handler
@ Parameter  : None
@********************************************************************************************************
_osExceptIrqHdl:
    LDR     SP, =__irq_stack_top
    SUB     LR, LR, #4
    STMFD   SP!, {R0-R2}
    MOV     R0, SP
    MRS     R1, SPSR
    MOV     R2, LR

    MSR    CPSR_c, #(OS_PSR_INT_DIS | OS_PSR_FIQ_DIS | OS_PSR_MODE_SVC)
    STMFD   SP!, {R2}         @ Push task  PC,
    STMFD   SP!, {LR}         @ Push task  LR,
    STMFD   SP!, {R3-R12}     @ Push task R12-R3,
    LDMFD   R0!, {R5-R7}      @ Move task R2-R0 from exception stack to tasks stack.
    STMFD   SP!, {R5-R7}
    STMFD   SP!, {R1}         @ Push task s CPSR (i.e. exception SPSR).
    @VSTMDB  SP!, {D16-D31}
    @VSTMDB  SP!, {D0-D15}


    @b00208046 before code:push PC,LR,R3-R12,R0-R2,CPSR
    LDR    R0, =g_vuwIntCount
    LDR    R1, [R0]
    ADD    R1, #1
    STR    R1, [R0]
    CMP    R1, #1
    @b00208046 two method 1:mov r0,sp hal_IRQ_handler(sp,g_vuwIntCount) continue use __svc_stack_top second:mov r0,sp ldr SP,=__WDG_STACK hal_IRQ_handler(sp,g_vuwIntCount) use __WDG_STACK
    MOV    R0,SP 			@modify by b00208046  
    BNE    osHwiDispatchBegin 		@b00208046 not first lisr go jump

    
    @ g_pRunningTask->pStackPointer = SP
    LDR    R2, =g_stLosTask
    LDR    R3, [R2]
    STR    SP, [R3]

    LDR    SP, =__irq2_stack_top


osHwiDispatchBegin:

    BLX hal_IRQ_handler


osInterruptExit:
    @ g_vuwIntCount--
    LDR   R0, =g_vuwIntCount
    LDR   R1, [R0]
    SUBS  R1, #1
    STR   R1, [R0]

    @ nest irq: g_vuwIntCount > 0
    BNE   osThrdContextLoad

osTaskContextSwitch:
    LDR     R0, =g_usLosTaskLock
    LDR     R0, [R0]
    LDR     R1, =g_stLosTask
    LDR     R1, [R1]
    CMP     R0, #0
    BNE    osTaskContextLoad

    LDR     R0, =g_stLosTask
    LDR     R0, [R0, #4]
    CMP    R1, R0
    BEQ    osTaskContextLoad

    @ g_pRunningTask->usTaskStatus &= ~OS_TSK_RUNNING
    LDRH   R7, [R1, #4]
    BIC    R7, #TASK_STATUS_RUNNING
    STRH   R7, [R1, #4]


    @ g_pRunningTask  = g_pHighestTask
    LDR     R0, =g_stLosTask
    LDR     R0, [R0, #4]
    LDR     R1, =g_stLosTask
    STR     R0, [R1]
    LDR     R1, [R1]

    @ g_pRunningTask->usTaskStatus |= OS_TSK_RUNNING
    LDRH   R7, [R0, #4] @LDRH
    ORR    R7, #TASK_STATUS_RUNNING
    STRH   R7, [R0, #4]


osTaskContextLoad:
    STMFD  SP!, {R0-R12}
    SUB    SP, SP, #4 @sp requires eight-byte alignment of the stack
    BLX    osTaskSwitchCheck
    ADD    SP, SP, #4
    LDMFD  SP!, {R0-R12}

    @ SP = g_pRunningTask->pStackPointer
    LDR    SP, [R1]

osThrdContextLoad:
    @VLDMIA  SP!, {D0-D15}                                     @fpu
    @VLDMIA  SP!, {D16-D31}                                     @fpu
    LDMFD  SP!, {R0}                @g_stLosTask.pstNewTask
    @MRS    R0,  CPSR
    MSR    SPSR_cxsf, R0

    LDMFD  SP!, {R0-R12, LR, PC}^

    .end
