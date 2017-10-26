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

#include "blkpu.h"
#include "stdlib.h"
#include <cstring>

void readBuf(DdrWord *inbuf,
             hls::stream<DdrWord> bufStream[ProcUnitCountForBlock]) {
  
  DdrWord bufWord;
  unsigned int startAddr;
  
  LOOP_BURST_BLOCK: for (unsigned int ml=0; ml<ProcUnitBlockSize/DdrBurstSize; ml++) {
  LOOP_PU_BUF: for (unsigned int pu=0; pu<ProcUnitCountForBlock; pu++) {
      startAddr = pu*ProcUnitBlockSize/DdrWordSize + ml*DdrBurstLen;
    LOOP_BURST_I : for (unsigned int i=0; i < DdrBurstLen; i++) {
#pragma HLS PIPELINE
	bufWord = inbuf[startAddr + i];
	bufStream[pu].write(bufWord);
      }
    }
  }
}

void writeBuf(DdrWord *outbuf,
	      hls::stream<DdrWord> bufStream[ProcUnitCountForBlock],
	      Ops& opcodes,	      
	      const char totalOps) {
  
  DdrWord bufWord;
  unsigned int startAddr;
  //  int opsDone = 0;
  LOOP_BURST_BLOCK: for (unsigned int ml=0; ml<ProcUnitBlockSize/DdrBurstSize; ml++) {
  LOOP_PU_BUF: for (unsigned int pu=0; pu<ProcUnitCountForBlock; pu++) {
      startAddr = pu*ProcUnitBlockSize/DdrWordSize + ml*DdrBurstLen;
    LOOP_BURST_I : for (unsigned int i=0; i < DdrBurstLen; i++) {
#pragma HLS PIPELINE
	bufStream[pu].read(bufWord);
	DdrWord outBufWord = bufWord ;
	//todo	if (opsDone < totalOps) {
	  // for now only support addition
	  char a = bufWord & 0x000000FF;
	  char b = bufWord>>8 & 0x000000FF;	
	  char c = 0;
	  if (opcodes == 0x01)
	    c=a+b;
	  else if (opcodes == 0x02)
	    c= b-a;
	  else if (opcodes == 0x03)
	    c = b*a;
	  outBufWord = outBufWord | (c << 24);
	  //	  opsDone++;
	  //	}
	outbuf[startAddr + i] = outBufWord;	
      }
    }
  }  
}


void processData(DdrWord *inbuf, DdrWord *outbuf, Reg& reg, Ops& opcodes, const char totalOps) {
#pragma HLS DATAFLOW

  hls::stream<DdrWord> bufStream[ProcUnitCountForBlock];
#pragma HLS STREAM variable=bufStream depth=1024 dim=1
#pragma HLS ARRAY_PARTITION variable=bufStream complete dim=1

  readBuf(inbuf, bufStream);
  writeBuf(outbuf,bufStream,opcodes,totalOps);
}

void readCmdBuf(DdrWord *inbuf,
		hls::stream<DdrWord>& bufStream) {
  DdrWord bufWord;
 LOOP_CMD_BURST_I : for (unsigned int i=0; i < DdrBurstLen; i++) {
#pragma HLS PIPELINE
    bufWord = inbuf[i];
    bufStream.write(bufWord);
  }
}

void loadCommand(DdrWord *inbuf, DdrWord *outbuf, Reg& reg, Ops& opcodes) {
  //#pragma HLS DATAFLOW

  hls::stream<DdrWord> bufStream;
#pragma HLS STREAM variable=bufStream depth=1024
  readCmdBuf(inbuf, bufStream);
  DdrWord bufWord;
  bufStream.read(bufWord);
  opcodes = bufWord & 0x000000FF;
  reg.A = bufWord & 0x0000FF00;
  reg.B = bufWord & 0x00FF0000;
  reg.C = bufWord & 0xFF000000;

  // Flush the rest of burst data, readcmd buf only read one burst
 LOOP_BURST_BLOCK: for (unsigned int ml=1; ml<DdrBurstLen; ml++) {
    bufStream.read(bufWord);
  }
}

// This kernel takes inbuf and loops back data to outbuf for KernelBlockDepth Bytes
extern "C" void blkpu(DdrWord *inbuf, DdrWord *outbuf) {
#pragma HLS INTERFACE m_axi port=inbuf offset=slave depth=2097152 bundle=gmem
#pragma HLS INTERFACE m_axi port=outbuf offset=slave depth=2097152 bundle=gmem
#pragma HLS INTERFACE s_axilite port=inbuf bundle=control
#pragma HLS INTERFACE s_axilite port=outbuf bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  DdrWord bufWord = inbuf[0];
  char blkType = bufWord & 0x000000FF;
  if (blkType == 0) {
    const char totalOps = (bufWord>>16) & 0x000000FF;
    processData(inbuf,outbuf,reg,opcodes,totalOps);
  } else {
    loadCommand(inbuf,outbuf,reg,opcodes);
  }
}
