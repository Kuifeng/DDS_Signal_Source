/*
 * Timer_Config.c
 *
 *  Created on: 2016-3-29
 *      Author: Kuikui
 */
#include "include.h"

int WaveIndex=0;
int FrequencyN=1000;
extern double sintable[4096];
extern double squaretable[4096];
extern unsigned short outwave[2];
void Timer64Init()
{
	TimerConfigure(SOC_TMR_2_REGS,TMR_CFG_64BIT_CLK_INT);
    TimerPeriodSet(SOC_TMR_2_REGS,TMR_TIMER12,TMR_2_Period_LSB);
    TimerPeriodSet(SOC_TMR_2_REGS,TMR_TIMER34,TMR_2_Period_MSB);
    TimerEnable(SOC_TMR_2_REGS,TMR_TIMER12,TMR_ENABLE_CONT);
}
/*
 * ��ʱ�жϳ�ʼ��
 */
void TimerInterruptInit()
{
	// ע���жϷ�����
	IntRegister(C674X_MASK_INT4, TimerISR);

	// ӳ���жϵ� DSP �������ж�
	IntEventMap(C674X_MASK_INT4, SYS_INT_T64P2_TINTALL);

	// ʹ�� DSP �������ж�
	IntEnable(C674X_MASK_INT4);

	// ʹ�� ��ʱ�� / ������ �ж�
	TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}
/*
 * �жϷ�����
 */
void TimerISR()
{
	 int DAC_out=0;
	 // ���ö�ʱ�� / �������ж�S
	 TimerIntDisable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

	 // ����жϱ�־
	 IntEventClear(SYS_INT_T64P2_TINTALL);
	 TimerIntStatusClear(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
     WaveIndex+=FrequencyN;
     if(WaveIndex>4096)
     {
    	 WaveIndex=0;
     }
     DAC_out=squaretable[WaveIndex]*3.0*65535.0/3.3/2.0;
     outwave[0]=DAC_out;
     outwave[1]=DAC_out;
  	 // ʹ�� ��ʱ�� / ������ �ж�
	 TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}
