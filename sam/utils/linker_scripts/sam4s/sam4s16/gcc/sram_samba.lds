/**
 * \file
 *
 * \brief SRAM Linker script for SAM.
 *
 * Copyright (c) 2011 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(entry)

/* Memory Spaces Definitions */
MEMORY
{
    romcodesram (W!RX) : ORIGIN = 0x20000000, LENGTH = 0x800
    sram (W!RX) : ORIGIN = 0x20000800, LENGTH = 0x0001F800 /* sram, 128K - sizeof(romcodesram) */
}

SECTIONS
{
    /* startup code in the .isr_vector */
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        KEEP(*(.isr_vector .isr_vector.*))
        *(.mailbox)
        *(.text .text.*)
        *(.rodata .rodata.*)
        *(.ramfunc .ramfunc.*)
        *(.glue_7)
        *(.glue_7t)
        *(.gcc_except_table)
        *(.rodata .rodata*)
        *(.gnu.linkonce.r.*)
        . = ALIGN(4);
        _etext = .;
    } > sram

    /* data */
    .data :
    {
        . = ALIGN(4);
        _sidata = .;
        _sdata = .;

        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    } > sram

    .bss (NOLOAD) : {
        _szero = .;
        *(.bss)
        . = ALIGN(4);
        _ezero = .;
    } >sram

    /* Stack in SRAM */
    _sstack = 0x2001FFF0;
}
. = ALIGN(4);
end = .;
