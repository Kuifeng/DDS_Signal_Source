/*
 * GPIO_Config.c
 *
 *  Created on: 2016-3-23
 *      Author: Kuikui
 */
#include "include.h"

void GPIOBankPinInit()
{
	GPIODirModeSet(SOC_GPIO_0_REGS, 109, GPIO_DIR_OUTPUT);
	GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_INPUT);     // USER0 KEY GPIO0[6]
	GPIODirModeSet(SOC_GPIO_0_REGS, 98, GPIO_DIR_INPUT);    // USER1 KEY GPIO6[1]
}

void GPIOPinMuxSetup()
{
   unsigned int PinMux_13_Val=0;
   PinMux_13_Val=HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13));
   PinMux_13_Val=(PinMux_13_Val&0xffff0fff)|0x00008000;
   HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13))=PinMux_13_Val;
   UARTPinMuxSetup(2, FALSE);
   GPIOBank0Pin6PinMuxSetup();
   GPIOBank6Pin1PinMuxSetup();
}

void GPIOBankPinInterruptInit(void)
{
    // 底板按键中断
    // 配置 USER0 KEY GPIO0[6] 为下降沿触发
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 7, GPIO_INT_TYPE_FALLEDGE);
    // 配置 USER1 KEY GPIO6[1] 为下降沿触发
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 98, GPIO_INT_TYPE_FALLEDGE);

    // 使能 GPIO BANK 中断
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);                  // USER0 KEY GPIO0
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);                  // USER1 KEY GPIO6

	// 注册中断服务函数
	IntRegister(C674X_MASK_INT4, USER0KEYIsr);
	IntRegister(C674X_MASK_INT5, USER1KEYIsr);

	// 映射中断到 DSP 可屏蔽中断
	IntEventMap(C674X_MASK_INT4, SYS_INT_GPIO_B0INT);
	IntEventMap(C674X_MASK_INT5, SYS_INT_GPIO_B6INT);

	// 使能 DSP 可屏蔽中断
	IntEnable(C674X_MASK_INT4);
	IntEnable(C674X_MASK_INT5);
}

void USER0KEYIsr()
{
	GPIOBankIntDisable(SOC_GPIO_0_REGS, 0);//禁用GPIO BANK0中断
	IntEventClear(SYS_INT_GPIO_B0INT);//清除中断标志
	if(GPIOPinIntStatus(SOC_GPIO_0_REGS, 7) == GPIO_INT_PEND)
	    {
			// 清除 GPIO0[6] 中断状态
			GPIOPinIntClear(SOC_GPIO_0_REGS, 7);

	    }
	    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);
}

void USER1KEYIsr(void)
{
    GPIOBankIntDisable(SOC_GPIO_0_REGS, 6);
    IntEventClear(SYS_INT_GPIO_B6INT);

    if(GPIOPinIntStatus(SOC_GPIO_0_REGS, 98) == GPIO_INT_PEND)
    {
		// 清除 GPIO6[1] 中断状态
		GPIOPinIntClear(SOC_GPIO_0_REGS, 98);
    }
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);
}
