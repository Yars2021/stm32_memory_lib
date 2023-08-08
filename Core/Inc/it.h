//
// Created by user on 04.03.2021.
//

#ifndef _IT_H_
#define _IT_H_

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void Error_Handler (void);


#ifdef __cplusplus
}
#endif

#endif /* _IT_H_ */
