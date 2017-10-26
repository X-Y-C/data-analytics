/*******************************************************************************
Vendor: Xilinx
Revision History:
Oct 25, 2016: initial release
*******************************************************************************
Copyright (C) 2016 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/

#ifndef XDBCONN_BLKLOOP_H
#define XDBCONN_BLKLOOP_H

#include <ap_int.h>
#include <hls_stream.h>

#ifdef HW_EMU_DEBUG
// Settings for 1024 Bytes (64*32) block of data
// Minimum blocksize must be ProcUnitBlockSize*ProcUnitCountForBlock
const unsigned int ProcUnitBlockSize =64;
const unsigned int ProcUnitCountForBlock = 32;
// DdrBurstSize is fixed to WordSize*BurstLen, must be less than ProcUnitBlockSize
const unsigned int DdrWordSize = 64;
const unsigned int DdrBurstLen = 4; 
const unsigned int DdrBurstSize = DdrWordSize*DdrBurstLen;

#else
// Settings for ~2 MB (64*32768) block of data
// Minimum blocksize must be ProcUnitBlockSize*ProcUnitCountForBlock
const unsigned int ProcUnitBlockSize = 32*32768;
const unsigned int ProcUnitCountForBlock = 2;
// DdrBurstSize is fixed to WordSize*BurstLen, must be less than ProcUnitBlockSize
const unsigned int DdrWordSize = 64;
const unsigned int DdrBurstLen = 64; 
const unsigned int DdrBurstSize = DdrWordSize*DdrBurstLen;
#endif

typedef ap_int<8*DdrWordSize> DdrWord;

extern "C" void blkloop(DdrWord *inbuf, DdrWord *outbuf);

#endif
