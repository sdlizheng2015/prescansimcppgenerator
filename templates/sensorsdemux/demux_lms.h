//
// (c)Copyright 2023 by Siemens Industry Software and Services B.V.
// All rights reserved.
//
// Simcenter Prescan(r) has been developed at Siemens Industry Software and
// Services B.V.
//
// This document contains proprietary and confidential information of Siemens.
// The contents of this document may not be disclosed to third parties, copied
// or duplicated in any form, in whole or in part, without prior written
// permission of Siemens.
//
// The terms and conditions governing the license of Simcenter Prescan(r)
// software consist solely of those set forth in the written contracts between
// Siemens or Siemens authorized third parties and its customers. The software
// may only be used or copied in accordance with the terms of these contracts.
//


#pragma once


#include "prescan/api/Lms.hpp"
#include "prescan/sim/LmsSensorUnit.hpp"
#include "vector"
#include <cmath>
#include "corecrt_math_defines.h"

using namespace std;

namespace prescan {
  namespace sensorDemux {
    void demux_lms(const prescan::sim::LmsSensorUnit* sensorUnit,
         std::shared_ptr<std::vector<std::vector<PRESCAN_LMS_DATA>>> Lines = nullptr) {
        if (Lines == nullptr){
            return;
        }else{
          std::vector<std::vector<PRESCAN_LMS_DATA>> _lines = sensorUnit->linesOutput();
          Lines->assign(_lines.data(), _lines.data() + _lines.size());
        }
    }
  }
}
