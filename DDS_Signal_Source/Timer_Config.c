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
 * 定时中断初始化
 */
void TimerInterruptInit()
{
	// 注册中断服务函数
	IntRegister(C674X_MASK_INT4, TimerISR);

	// 映射中断到 DSP 可屏蔽中断
	IntEventMap(C674X_MASK_INT4, SYS_INT_T64P2_TINTALL);

	// 使能 DSP 可屏蔽中断
	IntEnable(C674X_MASK_INT4);

	// 使能 定时器 / 计数器 中断
	TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}
/*
 * 中断服务函数
 */
void TimerISR()
{
	 int DAC_out=0;
	 // 禁用定时器 / 计数器中断S
	 TimerIntDisable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

	 // 清除中断标志
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
  	 // 使能 定时器 / 计数器 中断
	 TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}
