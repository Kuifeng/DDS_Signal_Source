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
    // �װ尴���ж�
    // ���� USER0 KEY GPIO0[6] Ϊ�½��ش���
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 7, GPIO_INT_TYPE_FALLEDGE);
    // ���� USER1 KEY GPIO6[1] Ϊ�½��ش���
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 98, GPIO_INT_TYPE_FALLEDGE);

    // ʹ�� GPIO BANK �ж�
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);                  // USER0 KEY GPIO0
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);                  // USER1 KEY GPIO6

	// ע���жϷ�����
	IntRegister(C674X_MASK_INT4, USER0KEYIsr);
	IntRegister(C674X_MASK_INT5, USER1KEYIsr);

	// ӳ���жϵ� DSP �������ж�
	IntEventMap(C674X_MASK_INT4, SYS_INT_GPIO_B0INT);
	IntEventMap(C674X_MASK_INT5, SYS_INT_GPIO_B6INT);

	// ʹ�� DSP �������ж�
	IntEnable(C674X_MASK_INT4);
	IntEnable(C674X_MASK_INT5);
}

void USER0KEYIsr()
{
	GPIOBankIntDisable(SOC_GPIO_0_REGS, 0);//����GPIO BANK0�ж�
	IntEventClear(SYS_INT_GPIO_B0INT);//����жϱ�־
	if(GPIOPinIntStatus(SOC_GPIO_0_REGS, 7) == GPIO_INT_PEND)
	    {
			// ��� GPIO0[6] �ж�״̬
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
		// ��� GPIO6[1] �ж�״̬
		GPIOPinIntClear(SOC_GPIO_0_REGS, 98);
    }
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 6);
}
