#include <linux/interrupt.h>

extern HWI_HANDLE_FORM_S m_astHwiForm[OS_HWI_MAX_NUM];
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
        const char *name, void *dev)
{
    UINT32 uwRet = 0;
    irq_args *astuwIrqPara;

    if(OS_INT_ACTIVE)
        return OS_ERRNO_HWI_INTERR;

    astuwIrqPara = (irq_args *)malloc(sizeof(irq_args));
    if (NULL == astuwIrqPara)
        return OS_ERRNO_HWI_NO_MEMORY;

    astuwIrqPara->iIrq = irq;
    astuwIrqPara->pDevId = dev;

    uwRet = LOS_HwiCreate(irq, 0, 0, (HWI_PROC_FUNC)handler, (HWI_ARG_T)astuwIrqPara);

    if (uwRet == LOS_OK)
        hal_interrupt_unmask(irq);
    else
        free(astuwIrqPara);/*lint !e424*/

    return uwRet;
}

void free_irq(unsigned int irq, void *dev)
{
    HWI_HANDLE_FORM_S *pstHwiForm;
    HWI_HANDLE_FORM_S *pstHwiFormtmp;
    UINTPTR uvIntSave;

    if(OS_INT_ACTIVE)
        return ;

    if (irq > OS_USER_HWI_MAX || irq < OS_USER_HWI_MIN)/*lint !e685 !e568*/
        return;

    uvIntSave = LOS_IntLock();
    pstHwiForm = &m_astHwiForm[irq];
    pstHwiFormtmp = pstHwiForm;

    while (pstHwiForm)
    {
        if((NULL == (irq_args*)pstHwiForm->uwParam) || (((irq_args*)(pstHwiForm->uwParam))->pDevId != dev))
        {
            pstHwiFormtmp = pstHwiForm;
            pstHwiForm = pstHwiForm->pstNext;
        }
        else
        {
            pstHwiFormtmp ->pstNext =  pstHwiForm->pstNext;
            free((void*)pstHwiForm->uwParam);
            pstHwiForm->uwParam = (HWI_ARG_T)NULL;
            free(pstHwiForm);/*lint !e424*/
            break;
        }
    }
    LOS_IntRestore(uvIntSave);
}

void irq_trigger(unsigned int irq)
{
    extern void dsb(void);
    extern void hal_interrupt_pending(unsigned int vector);
    hal_interrupt_pending(irq);
    dsb();
    dsb();    //TODO
}

void enable_irq(unsigned int irq)
{
    hal_interrupt_unmask(irq);
}

void disable_irq(unsigned int irq)
{
    hal_interrupt_mask(irq);
}

