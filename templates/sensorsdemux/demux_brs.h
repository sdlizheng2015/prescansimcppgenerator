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


#include "prescan/api/Brs.hpp"
#include "prescan/sim/BrsSensorUnit.hpp"
#include <vector>


namespace prescan{
namespace sensorDemux{
  void demux_brs(prescan::sim::BrsSensorUnit* brsUnit,
                         std::shared_ptr<std::vector<unsigned int>> ObjectID = nullptr,
                         std::shared_ptr<std::vector<double>> Left = nullptr,
                         std::shared_ptr<std::vector<double>> Right = nullptr,
                         std::shared_ptr<std::vector<double>> Bottom = nullptr,
                         std::shared_ptr<std::vector<double>> Top = nullptr){

        if (Left != nullptr && Right != nullptr && Bottom != nullptr && Top != nullptr){
          ObjectID->clear();
          Left->clear();
          Right->clear();
          Bottom->clear();
          Top->clear();

          for (auto & rec : brsUnit->brsSensorOutput()){
              ObjectID->push_back(rec->ObjectID);
              Left->push_back(rec->Left);
              Right->push_back(rec->Left);
              Top->push_back(rec->Top);
              Right->push_back(rec->Right);
          }
        }
  }
}
}
