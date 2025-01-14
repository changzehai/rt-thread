/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     XuXinming    first version
 * 2013-05-24     Grissiom     port to RM48x50
 */
#include <rtthread.h>

#include "armv7.h"
/**
 * @addtogroup RM48x50
 */
/*@{*/

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
                             rt_uint8_t *stack_addr, void *texit)
{
    rt_uint32_t *stk;

    stack_addr += sizeof(rt_uint32_t);
    stack_addr  = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stack_addr, 8);
    stk      = (rt_uint32_t *)stack_addr;
    *(--stk) = (rt_uint32_t)tentry;         /* entry point */
    *(--stk) = (rt_uint32_t)texit;          /* lr */
    *(--stk) = 0xdeadbeef;                  /* r12 */
    *(--stk) = 0xdeadbeef;                  /* r11 */
    *(--stk) = 0xdeadbeef;                  /* r10 */
    *(--stk) = 0xdeadbeef;                  /* r9 */
    *(--stk) = 0xdeadbeef;                  /* r8 */
    *(--stk) = 0xdeadbeef;                  /* r7 */
    *(--stk) = 0xdeadbeef;                  /* r6 */
    *(--stk) = 0xdeadbeef;                  /* r5 */
    *(--stk) = 0xdeadbeef;                  /* r4 */
    *(--stk) = 0xdeadbeef;                  /* r3 */
    *(--stk) = 0xdeadbeef;                  /* r2 */
    *(--stk) = 0xdeadbeef;                  /* r1 */
    *(--stk) = (rt_uint32_t)parameter;      /* r0 : argument */

    /* cpsr */
    if ((rt_uint32_t)tentry & 0x01)
        *(--stk) = SVCMODE | 0x20;          /* thumb mode */
    else
        *(--stk) = SVCMODE;                 /* arm mode   */

#if defined(__TI_VFP_SUPPORT__) || (defined (__VFP_FP__) && !defined(__SOFTFP__))
#ifdef RT_VFP_LAZY_STACKING
    {
        int i;

        for (i = 0; i < VFP_DATA_NR; i++)
        {
            *(--stk) = 0;
        }
        /* FPSCR TODO: do we need to set the values other than 0? */
        *(--stk) = 0;
        /* FPEXC. Enable the FVP if no lazy stacking. */
        *(--stk) = 0x40000000;
    }
#else
    /* FPEXC. Disable the FVP by default. */
    *(--stk) = 0x00000000;
#endif
#endif

    /* return task's current stack address */
    return (rt_uint8_t *)stk;
}

/*@}*/
