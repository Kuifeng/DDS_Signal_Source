/*
 * GPIO_Config.h
 *
 *  Created on: 2016-3-29
 *      Author: Kuikui
 */

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_

void GPIOBankPinInit();
void GPIOPinMuxSetup();
void GPIOBankPinInterruptInit();
void USER0KEYIsr();
void USER1KEYIsr();

#endif /* GPIO_CONFIG_H_ */
