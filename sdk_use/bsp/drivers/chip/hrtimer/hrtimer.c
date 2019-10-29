
#include "hrtimer_def.h"
#include <hi_driver_internal.h>
//#include <intLib.h>
//#include <jobLibP.h>
#include <hi_sal.h>

#define DRV_DFX_SYS_VAL_DECLARE
#include <drv_dfx_sys_val.h>
#include <hi_low_power.h>
//#include <private/jobLibP.h>
//#include <intlib.h>
#ifdef TERMINAL_DISPLAY
#define hrtimer_error(s) HI_UART_Printf(s)
#else
#define hrtimer_error(s)
#endif

struct hrtimer_struct
{
	HI_U32 base_virt;	/* timer register base address */
	VOIDFUNCPTR hisr;		/* hisr handle */
	HI_VOID *hisr_stack;	/* stack of hisr */
};

struct hrtimer_list_head
{
	struct HI_hrtimer_list_node_TYPE_S head;
};

#define HRTIMER_DEBUG

#ifndef STATIC
# ifdef HRTIMER_DEBUG
#define STATIC
# else
#define STATIC static
# endif
#endif

#ifndef NULL
#define NULL 0UL
#endif

#if (PRECISION_DELT < 0)
#error PRECISION_DELT cannot set to negative
#elif (PRECISION_DELT > 100)
#error PRECISION_DELT set too large ?
#endif


typedef struct
{
    HI_U32 last_over_turn_cnt;
}HI_HR_TIMER_CTRL;

HI_HR_TIMER_CTRL g_hr_timer_ctrl;


STATIC struct hrtimer_struct hrtimer_data;
STATIC struct hrtimer_list_head active_head;
STATIC struct hrtimer_list_head expired_head;

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 
STATIC struct hrtimer_list_head expired_lisr_head;
#endif

#define HRTIMER_NUMBER 100
#define hrtimer_writel(reg, val)\
	HI_REG_WRITE32(hrtimer_data.base_virt + (reg), (val))

#define hrtimer_readl(reg)\
	HI_REG_READ_VAL32(hrtimer_data.base_virt + (reg))

#define hrtimer_phys_to_virt(addr)	(addr)
#define hrtimer_virt_to_phys(addr)	(addr)

STATIC HI_VOID init_node(struct HI_hrtimer_list_node_TYPE_S *node, HI_VOID *parent)
{
	node->prv = node->next = node;
	node->parent = parent;
}
STATIC HI_VOID del_node(struct HI_hrtimer_list_node_TYPE_S *node)
{
	node->prv->next = node->next;
	node->next->prv = node->prv;
	node->next = node->prv = node;
}

STATIC HI_BOOL hrtimer_is_enable(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
    if (val & ENABLE_BIT)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
    
}
STATIC HI_VOID hrtimer_disable(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val &= ~ENABLE_BIT;
	hrtimer_writel(CONTROL_REG, val);
}
STATIC HI_VOID hrtimer_enable(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val |= ENABLE_BIT;
	hrtimer_writel(CONTROL_REG, val);
}
STATIC HI_VOID hrtimer_set_oneshot(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val |= ONESHOT_BIT;
	hrtimer_writel(CONTROL_REG, val);
}
STATIC HI_VOID hrtimer_enable_int(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val &= ~INTEN_BIT;
	hrtimer_writel(CONTROL_REG, val);
}
STATIC HI_VOID hrtimer_disable_int(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val |= INTEN_BIT;
	hrtimer_writel(CONTROL_REG, val);
}
STATIC HI_U32 hrtimer_get_curr_val(HI_VOID)
{
	HI_U32 val;

	val = hrtimer_readl(CONTROL_REG);
	val |= LOCK_BIT;
	hrtimer_writel(CONTROL_REG, val);
	while(hrtimer_readl(CONTROL_REG)&LOCK_BIT){};

	val = hrtimer_readl(CURRCOUNT_REG);
	return val;
}

STATIC HI_VOID hrtimer_set_load(HI_U32 cnt)
{
	hrtimer_writel(LOADCOUNT_REG, cnt);
}

STATIC HI_U32 hrtimer_get_load(HI_VOID)
{
    HI_U32 cnt;

    cnt = hrtimer_readl(LOADCOUNT_REG);
    return cnt;
}
#ifndef HAVE_PCLINT_CHECK
HI_U32 hr_timer_hisr_cnt = 0;
#endif
STATIC HI_VOID hrtimer_hisr(HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5)
{
	HI_HRTIMER_TYPE_S *parent = HI_NULL;
	struct HI_hrtimer_list_node_TYPE_S *pos;
	HI_U32 ulTimerCnt = 0;
	HIS_INT_CONTROL_VARS
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		g_Isr_stat.stHrtimer.ulHisrTickIn = HI_MDM_GetTick();
		(g_Isr_stat.stHrtimer.ulHisrCnt) ++;
#endif
	HIS_INT_ALL_DISABLE();
#ifndef HAVE_PCLINT_CHECK
	hr_timer_hisr_cnt ++;
#endif
	/* iterate the expired list from the head
	 * and call timeout callback,
	 * then move this node out of the expired list.
	 */
	pos = expired_head.head.next;
/* BEGIN: Modified by c00132816, 2013/4/28	 问题单号: DTS2013042300370 */
	for (; pos != &expired_head.head;pos = expired_head.head.next)
	{
		ulTimerCnt++;
		if(ulTimerCnt > HRTIMER_NUMBER)
		{
			HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER0);
			return;
		}
		parent = (HI_HRTIMER_TYPE_S *)pos->parent;

		expired_head.head.next	 = pos->next;
		/* cut off node link to move this timer out of
		 * expired list.
		 */
		del_node(pos);

		HIS_INT_ALL_RESTORE();
		g_drv_hrt_info.ulTimerTimeoutCnt++;
		if(HI_NULL != parent->func)
		{
			parent->func(parent->data);
		}
		else
		{
			hrtimer_error("hrtimer: no func  \r\n");		
		}

		HIS_INT_ALL_DISABLE();
	}
 /* END:   Modified by c00132816, 2013/4/28 */
	HIS_INT_ALL_RESTORE();
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		 g_Isr_stat.stHrtimer.ulHisrTickOut = HI_MDM_GetTick();
#endif
}
/* add node to the tail of head */
STATIC HI_VOID hrtimer_spilt_node(struct HI_hrtimer_list_node_TYPE_S *node,
		struct hrtimer_list_head *head)
{
	node->prv = head->head.prv;
	node->next = &head->head;
	head->head.prv->next = node;
	head->head.prv = node;
}
STATIC HI_VOID hrtimer_dec_expired(HI_HRTIMER_TYPE_S *timer, HI_U32 expired)
{
	if (timer->remaining_time >= expired)
		timer->remaining_time -= expired;
	else
		timer->remaining_time = 0;
}
#ifndef HAVE_PCLINT_CHECK
HI_U32 hr_timer_lisr_cnt = 0;
#endif

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 

STATIC HI_VOID hrtimer_lisr_run(HI_VOID)
{
    HI_HRTIMER_TYPE_S *parent = HI_NULL;
    struct HI_hrtimer_list_node_TYPE_S *pos;
    HI_U32 ulTimerCnt = 0;
    //HIS_INT_CONTROL_VARS 
    /* iterate the expired list from the head
     * and call timeout callback,
     * then move this node out of the expired list.
     */
    pos = expired_lisr_head.head.next;
/* BEGIN: Modified by c00132816, 2013/4/28   问题单号: DTS2013042300370 */
    for (; pos != &expired_lisr_head.head;pos = expired_lisr_head.head.next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER0);
            return;
        }
        parent = (HI_HRTIMER_TYPE_S *)pos->parent;

        expired_lisr_head.head.next   = pos->next;
        /* cut off node link to move this timer out of
         * expired list.
         */
        del_node(pos);
        hrtimer_spilt_node(&parent->node, &expired_head);

        if(parent->attribute==HI_FAST_TIMER_MAGIC&&parent->funcLisr!=HI_NULL)
        {
            parent->funcLisr(parent->data);
        }
    }
 /* END:   Modified by c00132816, 2013/4/28 */
}

#endif		

STATIC HI_VOID hrtimer_lisr(HI_S32 vector)
{
	struct HI_hrtimer_list_node_TYPE_S *pos;
	HI_HRTIMER_TYPE_S *parent = HI_NULL;
	HI_U32 expired;
	HI_U32 ulTimerCnt = 0;
	HI_U32 load_cnt = 0;
	HI_U32 cur_cnt =0;
	HI_U32 over_turn_cnt = 0;
#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		g_Isr_stat.stHrtimer.ulLisrTickIn = HI_MDM_GetTick();
		g_Isr_stat.stHrtimer.ulLisrCnt ++;
#endif
#ifndef HAVE_PCLINT_CHECK
	hr_timer_lisr_cnt ++;
#endif

	/* clear interrupt first. */
	(void)hrtimer_readl(EOI_REG);

	/* Avoid problems while getting staff 'parent'
	 * at some situation that the active timer list is empty.
	 * This will not occur in theory.
	 */
	if (active_head.head.next == &active_head.head)
	{
		hrtimer_error("hrtimer: active timer list empty "\
				"while interrupt occurs.\r\n");
		hrtimer_disable();
		goto out;
	}

	parent = (HI_HRTIMER_TYPE_S *)active_head.head.next->parent;
	expired = parent->remaining_time;

	/* move node on the front of active timer list
	 * to expired timer list.
	 */
	del_node(&parent->node);
	/*风险：这一行时被高优先级打断,删除或重启定时器场景，下面直接调用逻辑上有些问题。HISR调用存在同样问题。
	  解决办法1:对用户入参进行拷贝,而非仅拷贝指针。用户每次调用接口时传递不同data。用户可根据data值是否正确判断是否是当前定时器
	  解决方法2：此处关中断保护；HISR中关中断后从用户定时器中取出用户定时器,将用户定时器的data拷贝到局部变量后，开中断后传递局部变量给用户回调函数。 
	*/			   
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM     
    /*即使不需要在lisr中响应的timer也需要先加入到expired_lisr_head队列中,否则在hisr中定时器响应的先后顺序可能与预期不符*/      
    hrtimer_spilt_node(&parent->node, &expired_lisr_head);
#else           
    hrtimer_spilt_node(&parent->node, &expired_head);
#endif	  
	
	/* 正常情况下，+的结果应该为0，当时间到期而此时中断为关时，会reload时间，把中断到期后走过的时间补偿上 add by yuming*/
	load_cnt = hrtimer_get_load();
	cur_cnt = hrtimer_get_curr_val();
	over_turn_cnt = load_cnt-cur_cnt;
	if(over_turn_cnt > g_hr_timer_ctrl.last_over_turn_cnt)//通常应该是这样
	{
		expired += (over_turn_cnt-g_hr_timer_ctrl.last_over_turn_cnt);
		g_hr_timer_ctrl.last_over_turn_cnt = over_turn_cnt;
	}
	else//翻转了+又翻转
	{
		expired += over_turn_cnt;
		g_hr_timer_ctrl.last_over_turn_cnt = over_turn_cnt;
	}
	/* end */

	/* iterate the active list and
	 * re-caculate the remaining time for each timer
	 */
	for (pos = active_head.head.next; pos != &active_head.head;)
	{
		ulTimerCnt++;
		if(ulTimerCnt > HRTIMER_NUMBER)
		{
			HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER1);
			return;
		}
		parent = (HI_HRTIMER_TYPE_S *)pos->parent;
		hrtimer_dec_expired(parent, expired);
		/* if remaining times less than PRECISION_DELT,
		 * we deal with it as expired... and move it to
		 * expired timer list.
		 */

		if (parent->remaining_time <= PRECISION_DELT*HI_XTAL_CLOCK_BASE)		 
		{
			del_node(&parent->node);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM           
			hrtimer_spilt_node(&parent->node, &expired_lisr_head);
#else           
			hrtimer_spilt_node(&parent->node, &expired_head);
#endif
			/* node is link to expired timer list.
			 * so we should use active_head.head.next to loop
			 */
			pos = active_head.head.next;
		}
		else
		{
			pos = pos->next;
		}
	}

	hrtimer_disable();
	(void)hrtimer_readl(EOI_REG);

	/* if active list is not empty, restart hardware timer. */
	if (active_head.head.next != &active_head.head)
	{
		parent = (HI_HRTIMER_TYPE_S *)active_head.head.next->parent;
		hrtimer_set_load(parent->remaining_time);
		g_hr_timer_ctrl.last_over_turn_cnt = 0;
		hrtimer_enable();
	}
out:
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 
    hrtimer_lisr_run();
#endif
	(HI_VOID)HI_MDM_IRQ_AddJob(0,(HISR_PROC_FUNC)hrtimer_hisr, 1, 2, 3, 4, 5);

#if defined(PRODUCT_CFG_TASK_INT_SYS_ERR_INFO)&&defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
		g_Isr_stat.stHrtimer.ulLisrTickOut = HI_MDM_GetTick();
#endif
}

STATIC HI_U32 hrtimer_set_lisr(HI_VOID)
{
	return (HI_U32)HI_MDM_IRQ_Request(HRTIMER_IRQNR,0, hrtimer_lisr, 0);
}
STATIC HI_VOID hrtimer_reset_lisr(HI_VOID)
{
	(HI_VOID)HI_MDM_IRQ_Free(HRTIMER_IRQNR);
}

#define PRODUCT_CFG_LOWPOWER_HRTIMER_MIN_CNT (25*500)//500us

HI_PRV HI_BOOL hrtimer_check_lowpower_enable(HI_VOID)
{
    HI_U32 cur_val=0;

    if(HI_FALSE==hrtimer_is_enable())
    {
        return HI_TRUE;
    }
    
    cur_val = hrtimer_get_curr_val();
    if(cur_val>=PRODUCT_CFG_LOWPOWER_HRTIMER_MIN_CNT)//剩余时间大于门限
    {
        return HI_TRUE;
    }
    
    return HI_FALSE;
}

STATIC HI_S32 hrtimer_refcnt = 0;
HI_VOID HI_HRTIMER_Init(HI_VOID)
{
	HI_VOID *pointer = HI_NULL;

	if (hrtimer_refcnt ++)
	{
		return;
	}	

	hrtimer_data.hisr_stack = pointer;
	if (hrtimer_set_lisr() != OK)
	{
		goto clisr;
	}	

	HI_MDM_IRQ_Enable(HRTIMER_IRQNR);	
	hrtimer_data.base_virt = hrtimer_phys_to_virt(HRTIMER_REG_BASE);

	hrtimer_disable_int();
	hrtimer_disable();

	hrtimer_set_oneshot();

	init_node(&active_head.head, &active_head);
	init_node(&expired_head.head, &expired_head);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 
    init_node(&expired_lisr_head.head, &expired_lisr_head);
#endif        
	hrtimer_enable_int();

    
    HI_LOW_POWER_RegisterCheckFunc(HI_LOW_POWER_CHECK_ID_HRTIEMR,hrtimer_check_lowpower_enable);
	HI_DIAG_ReportSysInitEvent(HI_DEID_DRV_HRTIMER_INIT_OK, 1);
	return;

clisr:
	hrtimer_error("hrtimer: initiation fail!\r\n");
	return;
}

HI_VOID HI_HRTIMER_Destroy(HI_VOID)
{
	if (-- hrtimer_refcnt)
		return;

	if (active_head.head.next != &active_head.head
			|| expired_head.head.next != &expired_head.head
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 
            ||expired_lisr_head.head.next != &expired_lisr_head.head
#endif			
			)
	{
		hrtimer_error("hrtimer: list not empty!\r\n");
	}

	hrtimer_disable();

	hrtimer_reset_lisr();
	HI_DIAG_ReportSysInitEvent(HI_DEID_DRV_HRTIMER_DESTROY_OK, 1);
	return;
}
STATIC HI_U32 hrtimer_get_expired(HI_VOID)
{
	HI_HRTIMER_TYPE_S *parent = HI_NULL;

	HI_U32 curr_cnt = 0, expired = 0;
	HI_U32 i = 0;
	HI_U32 int_stat_1 = 0;
	HI_U32 int_stat_2 = 0;	 
	HI_U32 load_cnt = 0; 
	HI_U32 over_turn_cnt = 0;
	if (active_head.head.next != &active_head.head)
	{
		//保证读取到的中断状态和当前值是匹配的(全是到期前或全是到期后)
		for(i=0;i<3;i++)
		{
			
			int_stat_1 = hrtimer_readl(INTSTATUS_REG);
			/* get current time expired */
			curr_cnt = hrtimer_get_curr_val();
			int_stat_2 = hrtimer_readl(INTSTATUS_REG);
			if(int_stat_1==int_stat_2)
			{
				break;
			}
		}

		
		/* NOTICE:
		 * - register CURRENTVALUE is use to
		 * get 'the current value of register LOADCOUNT'.
		 * - register LOADCOUNT is decrease to 0 under oneshot mode.
		 * - so the value read frome CURRENTVALUE indicates
		 * the remaining time of the first timer
		 * on the active timer list.
		 */

		parent = (HI_HRTIMER_TYPE_S *)active_head.head.next->parent;

		/* calculate expired time since
		 * last update remaining time
		 */

		if(curr_cnt > parent->remaining_time||(int_stat_1&0x1))//TODO:前面条件可删除
		{
			load_cnt = hrtimer_get_load();
			over_turn_cnt = load_cnt-curr_cnt;
			if(over_turn_cnt > g_hr_timer_ctrl.last_over_turn_cnt)//通常应该是这样
			{
				expired = (over_turn_cnt-g_hr_timer_ctrl.last_over_turn_cnt) + parent->remaining_time;
				g_hr_timer_ctrl.last_over_turn_cnt = over_turn_cnt;
			}
			else//翻转了+又翻转
			{
				expired = over_turn_cnt+parent->remaining_time;
				g_hr_timer_ctrl.last_over_turn_cnt = over_turn_cnt;
			}
 
			/* 关中断时，利用寄存器中的时间来计算，替代以前的expire，来减小误差 add by yuming */
 
			/* end */
		}
		else
		{
			expired = parent->remaining_time - curr_cnt;
		}
	}
	return expired;
}

HI_VOID HI_HRTIMER_Start(HI_HRTIMER_TYPE_S *timer)
{
    struct HI_hrtimer_list_node_TYPE_S *pos;
    struct HI_hrtimer_list_node_TYPE_S *prv;
    HI_HRTIMER_TYPE_S *parent = HI_NULL;
    HI_U32 expired;
    HI_U32 ulTimerCnt = 0;
    HI_BOOL found = HI_FALSE;
    HIS_INT_CONTROL_VARS

    if (!timer || !timer->func ||(timer->expires == 0))//TODO：入参上限判断
    	return;
    HIS_INT_ALL_DISABLE();


    ulTimerCnt = 0;
    /* BEGIN: Added by c00132816, 2012/3/17	 问题单号:DTS2012022302221 */
    for (pos = active_head.head.next; pos != &active_head.head;
        pos = pos->next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER2);
            return;
        }
        parent = (HI_HRTIMER_TYPE_S *)pos->parent;
        if (parent == timer)
        {
            HI_HRTIMER_Stop(parent);
            found = HI_TRUE;
            break;
        }
    }
    /* BEGIN: Added by c00132816, 2013/4/28   问题单号: DTS2013042300370 */
    /*如果在过期链表中，则删除该结点，重新添加*/
    ulTimerCnt = 0;
    if (found == HI_FALSE)
    {
        for (pos = expired_head.head.next; pos != &expired_head.head;
            pos = pos->next)
        {
            ulTimerCnt++;
            if(ulTimerCnt > HRTIMER_NUMBER)
            {
                HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER5);
                return;
            }
            parent = (HI_HRTIMER_TYPE_S *)pos->parent;
            if (parent == timer)
            {
                del_node(&timer->node);
                init_node(&timer->node, timer);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM           
                found = HI_TRUE;
#endif
                break;
            }
        }
    }

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM 
    ulTimerCnt = 0;
    if (found == HI_FALSE)
    {
        for (pos = expired_lisr_head.head.next; pos != &expired_lisr_head.head;
                pos = pos->next)
        {
            ulTimerCnt++;
            if(ulTimerCnt > HRTIMER_NUMBER)
            {
                HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER5);
                return;
            }
            parent = (HI_HRTIMER_TYPE_S *)pos->parent;
            if (parent == timer)
            {
                del_node(&timer->node);
                init_node(&timer->node, timer);
                break;
            }
        }
    }
#endif

    /* END:   Added by c00132816, 2013/4/28 */
    /* END:   Added by c00132816, 2012/3/17 */
    /* do some initiations */
    timer->remaining_time = timer->expires*HI_XTAL_CLOCK_BASE;
    init_node(&timer->node, timer);

    /* searching active timer list, update remaining time
     * and find a place to add this timer
     */
     /* get current time expired */
    expired = hrtimer_get_expired();

    prv = &active_head.head;
    ulTimerCnt = 0;
    for (pos = active_head.head.next; pos != &active_head.head;
        pos = pos->next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER3);
            return;
        }
        parent = (HI_HRTIMER_TYPE_S *)pos->parent;
        hrtimer_dec_expired(parent, expired);

        if (parent->remaining_time <= timer->remaining_time)
            prv = pos;
    }

    /* add this timer on the active list */
    timer->node.prv = prv;
    timer->node.next = prv->next;
    /* BEGIN: Added by c00132816, 2012/3/17   问题单号:DTS2012022302221 */
    prv->next->prv= &timer->node;
    /* END:   Added by c00132816, 2012/3/17 */
    prv->next = &timer->node;
    /* if this timer's remaining time is the minus,
     * restart hardware timer use the first timer's remaining time
     */
    if (timer->node.prv == &active_head.head)
    {
        HI_HRTIMER_TYPE_S *head;

        head = (HI_HRTIMER_TYPE_S *)active_head.head.next->parent;

        hrtimer_disable();

        hrtimer_set_load(head->remaining_time);
        g_hr_timer_ctrl.last_over_turn_cnt = 0;
        hrtimer_enable();
    }
    g_drv_hrt_info.ulTimerAddCnt++;
    HIS_INT_ALL_RESTORE();

    return ;
}

HI_VOID HI_HRTIMER_Stop(HI_HRTIMER_TYPE_S *timer)
{
    struct HI_hrtimer_list_node_TYPE_S *pos;
    HI_HRTIMER_TYPE_S *parent = NULL;
    HI_HRTIMER_TYPE_S *found = NULL;
    HI_U32 expired;
    HI_U32 ulTimerCnt = 0;
    HIS_INT_CONTROL_VARS

    if (!timer)
        return;

    /* if this timer is not on any list. exit. */
    if (timer->node.next == &timer->node
            && timer->node.prv == &timer->node)
        return;

    HIS_INT_ALL_DISABLE();

    /* try searching expired list first. */
    ulTimerCnt = 0;
    for (pos = expired_head.head.next; pos != &expired_head.head;
        pos = pos->next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER4);
        }
        if (pos == &timer->node)
        {
            del_node(&timer->node);

            init_node(&timer->node, timer);
            goto out;
        }
    }
            
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM           
    ulTimerCnt = 0;
    for (pos = expired_lisr_head.head.next; pos != &expired_lisr_head.head;
            pos = pos->next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER4);
        }
        if (pos == &timer->node)
        {
            del_node(&timer->node);

            init_node(&timer->node, timer);
            goto out;
        }
    }
#endif

    /* searching active timer list try to find this timer.
     * while searching, we should update the remaining time
     * of each timer.
     */
    /* get current time expired */
    expired = hrtimer_get_expired();
    ulTimerCnt = 0;
    for (pos = active_head.head.next; pos != &active_head.head;
        pos = pos->next)
    {
        ulTimerCnt++;
        if(ulTimerCnt > HRTIMER_NUMBER)
        {
            HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_HRTIMER6);
            return;
        }
        parent = (HI_HRTIMER_TYPE_S *)pos->parent;


        hrtimer_dec_expired(parent, expired);
        if (parent == timer)
            found = parent;
    }

    /* if not found, cut off node link and exit. */
    if (!found)
    {
        /* if timer is not in active list and expired list,
        * but timer->node.next != timer->node,
        * maybe some error occured,
        * call init node to try to recover.
        */
        init_node(&timer->node, timer);
        goto out;
    }
    /* if this timer is on the head of active list,
     * we should enable hardware timer by next timer
     * on the active timer list if it exist.
     */
    if (found->node.prv == &active_head.head)
    {
        HI_HRTIMER_TYPE_S *next = HI_NULL;

        hrtimer_disable();
        (void)hrtimer_readl(EOI_REG);

        /* enable next timer if exist */
        if (found->node.next != &active_head.head)
        {
            next = (HI_HRTIMER_TYPE_S *)found->node.next->parent;

            hrtimer_set_load(next->remaining_time);
            g_hr_timer_ctrl.last_over_turn_cnt = 0;
            hrtimer_enable();
        }
    }

    /* If this time is not on the head of active timer list
     * we only delete it from the active timer list.
     *
     * On all case, we both delete it from active timer list.
     */
    del_node(&found->node);

    init_node(&found->node, found);
    g_drv_hrt_info.ulTimerDelCnt++;
out:
    HIS_INT_ALL_RESTORE();
    return;
}

HI_VOID HI_HRTIMER_Modify(HI_HRTIMER_TYPE_S *timer,
		HI_U32 expires,
		HI_VOID (*timeout)(HI_U32 ARG),
		HI_U32 data)
{
	//HIS_INT_CONTROL_VARS

	if (!timer || !timeout||(expires == 0))
		return;

	HI_HRTIMER_Stop(timer);

	timer->expires = expires;
	timer->func = timeout;
	timer->data = data;

	init_node(&timer->node, timer);

	HI_HRTIMER_Start(timer);
	return;
}
/* BEGIN: Added by wuxianfeng 190206, 2012/7/10   PN:DTS2012070702350*/
#define hrdelay_writel(base,reg, val)\
	HI_REG_WRITE32((base) + (reg), (val))
#define hrdelay_readl(base,reg)\
	HI_REG_READ_VAL32((base) + (reg))

HI_PRV HI_VOID hrtimer_start(HI_U32 ulBaseAddr)
{
    unsigned int val;

    /*************** disable INT***************************/
	val = hrdelay_readl(ulBaseAddr,CONTROL_REG);
	val |= INTEN_BIT;
	hrdelay_writel(ulBaseAddr,CONTROL_REG, val);

/*************** disable timer************************/
	val = hrdelay_readl(ulBaseAddr,CONTROL_REG);
	val &= ~ENABLE_BIT;
	hrdelay_writel(ulBaseAddr,CONTROL_REG, val);

/*************** setting srart count*********************/
	hrdelay_writel(ulBaseAddr,LOADCOUNT_REG, 0xFFFFFFFF);

/*************** setting running mode*******************/
	val = hrdelay_readl(ulBaseAddr,CONTROL_REG);
	val &=~ ONESHOT_BIT;
	hrdelay_writel(ulBaseAddr,CONTROL_REG, val);

/*************** enable timer4*************************/
	val = hrdelay_readl(ulBaseAddr,CONTROL_REG);
	val |= ENABLE_BIT;
	hrdelay_writel(ulBaseAddr,CONTROL_REG, val);

/*************** wait int and clear it *****************/
	val = hrdelay_readl(ulBaseAddr,CONTROL_REG);
	val |= LOCK_BIT;
	hrdelay_writel(ulBaseAddr,CONTROL_REG, val);

    while(hrdelay_readl(ulBaseAddr,CONTROL_REG)&LOCK_BIT)
        ;
}
#define HI_HRTIMER_VAL_TO_US(val) ((val)/(HIS_TMR_OS_CLOCK_BASE))
#define HI_HRTIMER_US_TO_VAL(us) ((us)*(HIS_TMR_OS_CLOCK_BASE))
#define HI_HRTIMER_GET_INTERVAL(curVal,lastVal) (((curVal) <= (lastVal))?((lastVal) - (curVal)):(0xFFFFFFFF - (curVal) + (lastVal)+1))

HI_U32 hrtimer_get_cur_val(HI_U32 ulBaseAddr)
{
    unsigned int val;
    
    val  = hrdelay_readl(ulBaseAddr,CONTROL_REG);
    val |= LOCK_BIT;
    hrdelay_writel(ulBaseAddr,CONTROL_REG, val);
    while (hrdelay_readl(ulBaseAddr,CONTROL_REG) & LOCK_BIT)
    ;

    val = hrdelay_readl(ulBaseAddr,CURRCOUNT_REG);
    return val;
}

HI_U32 HI_HRTIMER_GetCurrentVal(HI_VOID)
{
   static HI_BOOL isInit = HI_FALSE;
    if(isInit==HI_FALSE)
    {
        isInit = HI_TRUE;
        hrtimer_start(HRDELAY_REG_BASE);
    }
    return hrtimer_get_cur_val(HRDELAY_REG_BASE);
}

#define HI_HRTIMER_UDELAY_MAX_TIME (10*1000*1000)//10秒 (单位:us)
HI_U32 HI_HRTIMER_Udelay(HI_U32 count)
{
    HI_U32 ulDelayVal;
    HI_U32 ulStRegVal,ulEndRegVal;
    if(count>HI_HRTIMER_UDELAY_MAX_TIME)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    ulDelayVal = HI_HRTIMER_US_TO_VAL(count);
    ulStRegVal = HI_HRTIMER_GetCurrentVal();
    for(;;)
    {
        ulEndRegVal = HI_HRTIMER_GetCurrentVal();
        if(HI_HRTIMER_GET_INTERVAL(ulEndRegVal, ulStRegVal)>=ulDelayVal)
        {
            return HI_ERR_SUCCESS;
        }
    }

}


typedef struct {
	HI_U32 expires;//用户输入参数 ，单位为us
	HI_VOID (*func)(HI_U32 arg);//回调函数
	HI_U32 data;//回调函数传参使用
	/********* DO NOT UPDATE THESE STAFF *****/
	HI_U32 remaining_time;//内部使用，用户不要输入
}HI_HRTIMER_DEBUG_S;

typedef struct {
    HI_U32 ulTimerAddCnt; //添加TIMER的个数
    HI_U32 ulTimerDelCnt; //删除TIMER的个数
    HI_U32 ulTimerTimeoutCnt;    //到期TIEMR的个数
    HI_U16 ulTimerActiveListCnt; //链表中未到期的节点个数
    HI_U16 ulTimerExpiredListCnt;
    HI_HRTIMER_DEBUG_S g_stDfxAtvTimerinfo[HRTIMER_NUMBER];
    HI_HRTIMER_DEBUG_S g_stDfxExpTimerinfo[HRTIMER_NUMBER];
} NU_HRTIMERINFO_DEBUG_S;

HI_U32 DRV_GetHrTimerListSize(HI_VOID)
{
    return sizeof(NU_HRTIMERINFO_DEBUG_S);
}

/* END:   Added by wuxianfeng 190206, 2012/7/10 */

