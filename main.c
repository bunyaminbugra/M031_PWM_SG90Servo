/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * @brief    A project template for M031 MCU.
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2017 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

void PWM_Init(void); 

#define __HXT         (32000000UL)  /*!< High Speed External Crystal Clock Frequency */

void SYS_Init(void)
{
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init System Clock                                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	//CLK->PWRCTL   = (CLK->PWRCTL   & ~(0x0000000DUL)) | 0x0231001FUL;
	//CLK->CLKDIV0  = (CLK->CLKDIV0  & ~(0x00FFFFFFUL)) | 0x00000000UL;
	//CLK->CLKDIV4  = (CLK->CLKDIV4  & ~(0x00FFFFFFUL)) | 0x00000000UL;
	//CLK->PCLKDIV  = (CLK->PCLKDIV  & ~(0x00000077UL)) | 0x00000000UL;
	//CLK->CLKSEL0  = (CLK->CLKSEL0  & ~(0x0000013FUL)) | 0x00000038UL;
	//CLK->CLKSEL1  = (CLK->CLKSEL1  & ~(0x7777777FUL)) | 0x4477773BUL;
	//CLK->CLKSEL2  = (CLK->CLKSEL2  & ~(0x0030033FUL)) | 0x0020032BUL;
	//CLK->CLKSEL3  = (CLK->CLKSEL3  & ~(0x77777700UL)) | 0x44444400UL;
	//CLK->AHBCLK   = (CLK->AHBCLK   & ~(0x0000009EUL)) | 0x00000004UL;
	//CLK->APBCLK0  = (CLK->APBCLK0  & ~(0x18FF33FFUL)) | 0x00000001UL;
	//CLK->APBCLK1  = (CLK->APBCLK1  & ~(0x000F0300UL)) | 0x00010000UL;
	//CLK->CLKOCTL  = (CLK->CLKOCTL  & ~(0x0000007FUL)) | 0x00000000UL;
	//SysTick->CTRL = (SysTick->CTRL & ~(0x00000005UL)) | 0x00000000UL;
	//RTC->LXTCTL   = (RTC->LXTCTL   & ~(0x00000080UL)) | 0x00000000UL;

	/* Unlock protected registers */
	SYS_UnlockReg();

	/* If the macros do not exist in your project, please refer to the related clk.h in Header folder of the tool package */
	/* Enable clock source */
	CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk|CLK_PWRCTL_HIRCEN_Msk|CLK_PWRCTL_HXTEN_Msk);

	/* Waiting for clock source ready */
	CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk|CLK_STATUS_HIRCSTB_Msk|CLK_STATUS_HXTSTB_Msk);

	/* Set HCLK clock */
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));

	/* Set PCLK-related clock */
	CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1);

	/* Enable IP clock */
	CLK_EnableModuleClock(ISP_MODULE);
	CLK_EnableModuleClock(PWM0_MODULE);
	
	/* Set IP clock */
	CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, MODULE_NoMsk);

	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
	SystemCoreClockUpdate();

   //SYS->GPA_MFPH = 0x00000000UL;
    //SYS->GPA_MFPL = 0x0000D000UL;
    //SYS->GPB_MFPH = 0x00000000UL;
    //SYS->GPB_MFPL = 0x00000000UL;
    //SYS->GPC_MFPL = 0x00000000UL;
    //SYS->GPF_MFPH = 0x00000000UL;
    //SYS->GPF_MFPL = 0x000000EEUL;

    /* If the macros do not exist in your project, please refer to the corresponding header file in Header folder of the tool package */
    SYS->GPA_MFPH = 0x00000000;
    SYS->GPA_MFPL = SYS_GPA_MFPL_PA3MFP_PWM0_CH2;
    SYS->GPB_MFPH = 0x00000000;
    SYS->GPB_MFPL = 0x00000000;
    SYS->GPC_MFPL = 0x00000000;
    SYS->GPF_MFPH = 0x00000000;
    SYS->GPF_MFPL = SYS_GPF_MFPL_PF1MFP_ICE_CLK | SYS_GPF_MFPL_PF0MFP_ICE_DAT;



	/* Lock protected registers */
	SYS_LockReg();

	return;
}

void PWM_Init(void)
{
	// PWM0 kanal 2 için saat ayarlari
	PWM_SetClockSource(PWM0, 2, PWM_CLKSRC_PWM_CLK);

	// Prescaler ve Counter ayarlari
	PWM_SET_PRESCALER(PWM0, 2, 0x3E7); // Prescaler degerini 999 olarak ayarlar
	PWM_SET_CNR(PWM0, 2, 0x27F); // Counter degerini 639 olarak ayarlar

	// Counter tipi ayari
	PWM0->CTL1 &= ~(0x3ul << 4 ); // Counter tipini belirler (Up-Counter)

	// PWM0 Pulse Generator ayarlari
	PWM_SET_OUTPUT_LEVEL(PWM0, PWM_CH_2_MASK, PWM_OUTPUT_HIGH, PWM_OUTPUT_LOW, PWM_OUTPUT_NOTHING, 
						 PWM_OUTPUT_NOTHING); // PWM çikis seviyesini ayarlar

	// PWM0 kanal 2 çikisini etkinlestir
	PWM_EnableOutput(PWM0, PWM_CH_2_MASK);

	// PWM0 kanal 2'yi baslat
	PWM_Start(PWM0, PWM_CH_2_MASK);
}

int main()
{
		// Baslangiç ve bitis noktalari
		int start = 16, end = 80;
		// Artis miktari
		int increment = 16;
	
		int pos = 0;
	
    SYS_Init();
	PWM_Init();
	
	while(1)
	{
		pos = start;
		
		// Start'tan end'e kadar hareket edin
		while(pos <= end)
		{
			// PWM0, kanal 2'nin CMR degerini ayarla (motorun pozisyonunu degistir)
			PWM_SET_CMR(PWM0, 2, pos);
			
			pos += increment;

			// Her dönüste 500 ms gecikme sagla
			CLK_SysTickDelay(500 * 1000);
		}

		pos = end;
		
		// End'den start'a geri dön
		while(pos >= start)
		{
			// PWM0, kanal 2'nin CMR degerini ayarla (motorun pozisyonunu degistir)
			PWM_SET_CMR(PWM0, 2, pos);
			
			pos -= increment;

			// Her dönüste 500 ms gecikme sagla
			CLK_SysTickDelay(500 * 1000);
		}

	}
}

/*** (C) COPYRIGHT 2017 Nuvoton Technology Corp. ***/
