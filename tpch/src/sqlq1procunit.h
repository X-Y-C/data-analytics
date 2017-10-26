/*******************************************************************************
Vendor: Xilinx
Revision History:
Oct 11, 2016: initial release
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
#ifndef TPCH_SQLQ1PROCUNIT_H
#define TPCH_SQLQ1PROCUNIT_H
#include "sqldefs.h"

struct SqlQ1GrpData {
  struct Q1Data {
    SqlDec17P2D sum_qty;
    SqlDec17P2D sum_base_price;
    SqlDec17P2D sum_disc_price;
    SqlDec17P2D sum_charge;
    SqlDec17P2D sum_disc;
    // Sql integer with 10-precision for coutn_order, 32 bit int
    SqlInt10P   count_order;
    void SQLEVAL(add,SQLKERNEL_ID)(Q1Data& qd);  
    void SQLEVAL(init,SQLKERNEL_ID)();
    int SQLEVAL(binStr,SQLKERNEL_ID)(char *bin, int binLoc);

    int binStrToBin(char *bin, int binLoc);
    void print();    
  };
  
  // ReturnFlag f and LineStatus s
  Q1Data fNsO;
  Q1Data fRsF;
  Q1Data fAsF;
  Q1Data fNsF;
  void SQLEVAL(init,SQLKERNEL_ID)();
  void SQLEVAL(add,SQLKERNEL_ID)(SqlQ1GrpData& qgd);
  int SQLEVAL(binStr,SQLKERNEL_ID)(char *bin, int binLoc);

  int binStrToBin(char *bin, int binLoc);
  void print();
};

void SQLEVAL(sqlq1procunit,SQLKERNEL_ID)(SqlQ1GrpData& res,
		   hls::stream<SqlDefs::RdDdrWord> &bufStream,
		   unsigned char block_num,
		   unsigned char noOfProcUnitBlocks);

#endif
