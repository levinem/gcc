/* c-isr library stuff of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2025 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

.macro RESTORE_ALL
	setgie.d
	dsb
	lmw.bim $r1, [$sp], $r2, #0x0   /* Restore IPC, IPSW.  */
	mtsr	$r1, $IPC
	mtsr	$r2, $IPSW
	RESTORE_FPU_REGS
	RESTORE_MAC_REGS
  RESTORE_USR_REGS
#if __NDS32_REDUCED_REGS__ || __NDS32_REDUCE_REGS
	lmw.bim $r0, [$sp], $r10, #0x0	/* Restore all regs.  */
	lmw.bim $r15, [$sp], $r15, #0xf
#else
	lmw.bim $r0, [$sp], $r27, #0xf	/* Restore all regs.  */
#endif
.endm
