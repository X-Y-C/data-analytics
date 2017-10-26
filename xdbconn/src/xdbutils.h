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
#ifndef XDBCONN_XDBUTILS_H
#define XDBCONN_XDBUTILS_H

#include <time.h>
#include <sys/time.h>
#include <chrono>

class XdbTimer {
public:
  void start() {
    mStart = std::chrono::high_resolution_clock::now(); 
  }
  void end() {
    mEnd = std::chrono::high_resolution_clock::now();
  }
  
  double duration()   {
    std::chrono::duration<double> elapsedSeconds = mEnd - mStart;
    return (elapsedSeconds.count());
  }

  XdbTimer() { }
  ~XdbTimer() { }  

private:
  // steady and system same across most implementaion, use high_resolution_clock (system_cloc)
  std::chrono::time_point<std::chrono::high_resolution_clock> mStart, mEnd;
  //std::chrono::time_point<std::chrono::system_clock> mSystemStart, mSystemEnd;
  //std::chrono::time_point<std::chrono::steady_clock> mSteadyStart, mSteadyEnd;  
};

#endif
