#ifndef __PVD_H
#define	__PVD_H

#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"

void PVD_Configuration(void);
void PVD_EXTI_Config(void);


#endif /* __PVD_H */

