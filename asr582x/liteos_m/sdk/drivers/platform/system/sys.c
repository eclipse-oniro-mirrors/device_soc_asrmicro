/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////     

//////////////////////////////////////////////////////////////////////////////////  


  
__asm void WFI_SET(void)
{
    WFI;          
}

__asm void INTX_DISABLE(void)
{
    CPSID   I
    BX      LR      
}

__asm void INTX_ENABLE(void)
{
    CPSIE   I
    BX      LR  
}

__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0             //set Main Stack value
    BX r14
}


// below code is for no need to select microlib
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
 struct __FILE 
{ 
    int handle; 
}; 

 FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
    x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{     
    while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
    return ch;
}
#endif













