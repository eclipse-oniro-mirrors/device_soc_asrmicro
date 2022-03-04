/**************************************************************************//**
 * @file     gcc.ld
 * @brief    Linker script for ASR582x Device
 * @version  V1.00
 * @date     3. March 2022
 *
 * @note
 *
 ******************************************************************************/
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

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20FFF000;

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size  = 0x10000;  /* required amount of heap  */
_Min_Stack_Size = 0x2000;   /* required amount of stack */

_VECTOR_REMAP_ADDR = 0x00087F00;

/* Specify the memory areas */
MEMORY
{
	FLASH (xr)       	: 	ORIGIN = 0x10012000, LENGTH = 1432K
	ITCM (xrw)       	:	ORIGIN = 0x00080000, LENGTH = 32K
	RAM (xrw)       	:  	ORIGIN = 0x20FD0400, LENGTH = 187K
	RAM2 (xrw)      	:  	ORIGIN = 0x21000000, LENGTH = 96K    /*RAM2 for Application*/
	SHARED_MEMORY(rw) 	: 	ORIGIN = 0x60000000, LENGTH = 32K
}

/* Define output sections */
SECTIONS
{
  _stext = .;
/* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = 0x100;
  } >FLASH

  .app_version_sec :
  {
    KEEP(*(app_version_sec))
    . = ALIGN(0x10);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);

    *(EXCLUDE_FILE (*memcpy.o *rand.o *_clzsi2.o *los_dispatch.o *los_exc.o *lega_rtos.o).text)
    *(EXCLUDE_FILE (*memcpy.o *rand.o *_clzsi2.o *los_dispatch.o *los_exc.o *lega_rtos.o).text*)
  
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
  
      . = ALIGN(4);
      __zinitcall_bsp_start = .;
      KEEP (*(.zinitcall.bsp0.init))
      KEEP (*(.zinitcall.bsp1.init))
      KEEP (*(.zinitcall.bsp2.init))
      KEEP (*(.zinitcall.bsp3.init))
      KEEP (*(.zinitcall.bsp4.init))
      __zinitcall_bsp_end = .;
      __zinitcall_device_start = .;
      KEEP (*(.zinitcall.device0.init))
      KEEP (*(.zinitcall.device1.init))
      KEEP (*(.zinitcall.device2.init))
      KEEP (*(.zinitcall.device3.init))
      KEEP (*(.zinitcall.device4.init))
      __zinitcall_device_end = .;
      __zinitcall_core_start = .;
      KEEP (*(.zinitcall.core0.init))
      KEEP (*(.zinitcall.core1.init))
      KEEP (*(.zinitcall.core2.init))
      KEEP (*(.zinitcall.core3.init))
      KEEP (*(.zinitcall.core4.init))
      __zinitcall_core_end = .;
      __zinitcall_sys_service_start = .;
      KEEP (*(.zinitcall.sys.service0.init))
      KEEP (*(.zinitcall.sys.service1.init))
      KEEP (*(.zinitcall.sys.service2.init))
      KEEP (*(.zinitcall.sys.service3.init))
      KEEP (*(.zinitcall.sys.service4.init))
      __zinitcall_sys_service_end = .;
      __zinitcall_sys_feature_start = .;
      KEEP (*(.zinitcall.sys.feature0.init))
      KEEP (*(.zinitcall.sys.feature1.init))
      KEEP (*(.zinitcall.sys.feature2.init))
      KEEP (*(.zinitcall.sys.feature3.init))
      KEEP (*(.zinitcall.sys.feature4.init))
      __zinitcall_sys_feature_end = .;
      __zinitcall_run_start = .;
      KEEP (*(.zinitcall.run0.init))
      KEEP (*(.zinitcall.run1.init))
      KEEP (*(.zinitcall.run2.init))
      KEEP (*(.zinitcall.run3.init))
      KEEP (*(.zinitcall.run4.init))
      __zinitcall_run_end = .;
      __zinitcall_app_service_start = .;
      KEEP (*(.zinitcall.app.service0.init))
      KEEP (*(.zinitcall.app.service1.init))
      KEEP (*(.zinitcall.app.service2.init))
      KEEP (*(.zinitcall.app.service3.init))
      KEEP (*(.zinitcall.app.service4.init))
      __zinitcall_app_service_end = .;
      __zinitcall_app_feature_start = .;
      KEEP (*(.zinitcall.app.feature0.init))
      KEEP (*(.zinitcall.app.feature1.init))
      KEEP (*(.zinitcall.app.feature2.init))
      KEEP (*(.zinitcall.app.feature3.init))
      KEEP (*(.zinitcall.app.feature4.init))
      __zinitcall_app_feature_end = .;
      __zinitcall_test_start = .;
      KEEP (*(.zinitcall.test0.init))
      KEEP (*(.zinitcall.test1.init))
      KEEP (*(.zinitcall.test2.init))
      KEEP (*(.zinitcall.test3.init))
      KEEP (*(.zinitcall.test4.init))
      __zinitcall_test_end = .;
      __zinitcall_exit_start = .;
      KEEP (*(.zinitcall.exit0.init))
      KEEP (*(.zinitcall.exit1.init))
      KEEP (*(.zinitcall.exit2.init))
      KEEP (*(.zinitcall.exit3.init))
      KEEP (*(.zinitcall.exit4.init))
      __zinitcall_exit_end = .;

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);

    *(EXCLUDE_FILE (*_clz.o).rodata)
    *(EXCLUDE_FILE (*_clz.o).rodata*)
  
    . = ALIGN(4);
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  .hdf_drivers_entry :
  {
    _hdf_drivers_start = .;
    KEEP(*(.hdf.driver))
    _hdf_drivers_end = .;
  } >FLASH

  /* shared RAM */
  sharemem(NOLOAD):
  {
    . = ALIGN(4);
    _shmem_s = .;
    *(SHAREDRAM)
    . = ALIGN(4);
    _shmem_e = .;
  } >SHARED_MEMORY

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    *_clz.o (.rodata .rodata*)
 
    . = ALIGN(4);
    _edata = .;
  } >RAM  AT>FLASH

  /* some bss filed putted in DTCM*/
  . = ALIGN(4);
  .bss_dtcm(NOLOAD) :
  {
    _bdsdata = .;
    . = ALIGN(4);
    KEEP(*(BSS_DTCM))
    . = ALIGN(4);
    _bdedata = .;        /* define a global symbol at data end */
  } >RAM

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM2

  /* used by the startup to initialize data */
  _sifunc = LOADADDR(.func);

  /* Initialized func sections goes into ROM, load LMA copy after code */
  .func :
  {
    . = ALIGN(4);
    _sfunc = .;        /* create a global symbol at func start */
    *(seg_critical_function)          /* .func sections */
    *(seg_flash_driver)
	  *(seg_wf_flash_driver)
    *rand.o(.text*)
    *memcpy.o(.text*)
    *_clzsi2.o (.text .text*)
    *los_dispatch.o(.text .text*)
    *los_exc.o(.text .text*)
    *lega_rtos.o (.text .text*)
  
    . = ALIGN(4);
    _efunc = .;        /* define a global symbol at func end */
  } >ITCM  AT>FLASH

  /* some bss filed putted in ITCM*/
  . = ALIGN(4);
  .bss_itcm(NOLOAD) :
  {
    _bisdata = .;
    . = ALIGN(4);
    KEEP(*(BSS_ITCM))
    . = ALIGN(4);
    _biedata = .;        /* define a global symbol at data end */
  } >ITCM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
  _sstack = .;
  
  /* User_heap_stack section, used to check that there is enough RAM left */
  PROVIDE ( end  = _bdedata );
  PROVIDE ( _end = _bdedata );

  /* system stack */
  PROVIDE (_stack_base  = _estack - _Min_Stack_Size); /* _estack is top of stack*/
  ASSERT  ((_stack_base > end), "Error: No room left for the stack")
  /* _estack is top of stack*/

  /* left ram for heap */
  PROVIDE (__los_heap_addr_start__ = _end);
  PROVIDE (__los_heap_addr_end__   = _stack_base - 1);
  PROVIDE (heap_len   = __los_heap_addr_end__ - __los_heap_addr_start__);
  ASSERT  ((heap_len > _Min_Heap_Size), "Error: No room left for the heap")

  ASSERT  ((_biedata < _VECTOR_REMAP_ADDR), "Error: No room left for ITCM")

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
