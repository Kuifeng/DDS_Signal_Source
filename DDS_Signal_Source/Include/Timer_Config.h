/*
 * Timer_Config.h
 *
 *  Created on: 2016-3-29
 *      Author: Kuikui
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_


#define TMR_2_Period_LSB  0x0000D970//0x0D970100
#define TMR_2_Period_MSB  0
#define pi                3.141592653
#define SYS_CLK_1         456000000
#define SYS_CLK_2         228000000
#define UART_CLK          228000000


void Timer64Init();
void TimerInterruptInit();
void TimerISR();

#endif /* TIMER_CONFIG_H_ */
