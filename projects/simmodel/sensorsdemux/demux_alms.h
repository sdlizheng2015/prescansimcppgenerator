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


#include "prescan/api/Alms.hpp"
#include "prescan/sim/AlmsSensorUnit.hpp"
#include "vector"
#include <cmath>
#include "corecrt_math_defines.h"

using namespace std;

namespace prescan {
  namespace sensorDemux {
    void demux_alms(const prescan::sim::AlmsSensorUnit* sensorUnit,
         std::shared_ptr<std::vector<const PRESCAN_ALMS_DATA*>> PolyLines = nullptr) {
        if (PolyLines == nullptr){
            return;
        }else{
          PolyLines->clear();
          auto _lines = sensorUnit->linesOutput();
          //std::cout << "Lines: " << _lines.size() << std::endl;
          for (const auto & _line : _lines){
            auto _polylines = _line.data();
            //std::cout << "Polylines: " << _polylines.size() << std::endl;
            for (const auto & _polyline: _polylines){
              PolyLines->push_back(_polyline);
            }
          }
        }
    }
  }
}
