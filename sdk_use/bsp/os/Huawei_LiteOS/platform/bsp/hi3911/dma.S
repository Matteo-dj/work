.global arm926_dma_inv_range
.global arm926_dma_clean_range
.global los_setjmp
.global los_longjmp


.equ CACHE_DLINESIZE, 32
 /*
 *  dma_inv_range(start, end)
 *
 *  Invalidate (discard) the specified virtual address range.
 *  May not write back any entries.  If 'start' or 'end'
 *  are not cache line aligned, those lines must be written
 *  back.
 *
 *  - start - virtual start address
 *  - end   - virtual end address
 *
 */
arm926_dma_inv_range:
    tst r0, #CACHE_DLINESIZE - 1
    mcrne p15, 0, r0, c7, c10, 1        @ clean D entry
    tst r1, #CACHE_DLINESIZE -1
    mcrne p15, 0, r1, c7, c10, 1        @ clean D entry

    bic r0, r0, #CACHE_DLINESIZE - 1
1:
    mcr p15, 0, r0, c7, c6, 1       @ invalidate D entry
    add r0, r0, #CACHE_DLINESIZE
    cmp r0, r1
    blo 1b
    mcr p15, 0, r0, c7, c10, 4      @ drain WB

    mov pc, lr      @ return

.type arm926_dma_inv_range, %function;
@.size arm926_dma_inv_range, .-arm926_dma-inv_range

/*
 *  dma_clean_range(start, end)
 *
 *  Clean the specified virtual address range.
 *
 *  - start - virtual start address(r0)
 *  - end   - virtual end address(r1)
 */

arm926_dma_clean_range:
    bic r0, r0, #CACHE_DLINESIZE - 1
1:
    mcr p15, 0, r0, c7, c10, 1      @ clean D entry
    add r0, r0, #CACHE_DLINESIZE
    cmp r0, r1
    blo 1b

    mcr p15, 0, r0, c7, c10, 4      @ drain WB
    mov pc, lr      @ return
.type arm926_dma_clean_range,%function;
@.size arm926_dma_clean_range, .-arm926_clean_range


los_longjmp:

        ldmfd   r0,{r4-r14}
        cmp     r1,#0
        moveq   r1,#1
        mov     r0,r1         @# return [arg1]
        mov     pc,lr

.type los_longjmp,%function;
.size los_longjmp, .-los_longjmp

los_setjmp:
        stmea   r0,{r4-r14}
        mov     r0,#0

        mov     pc,lr

.type los_setjmp,%function;
.size los_setjmp, .-los_setjmp

