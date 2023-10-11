//
// Copyright 2021 Siemens
//
#pragma once

#include <iostream>
#include <string>
#include <utility>

#include "prescan/api/Experiment.hpp"
#include "prescan_api_userdata.h"
#include "prescan/api/Trajectory.hpp"

namespace prescan{
namespace api{
namespace UserData{

typedef struct _EgoUserDate
{
	const std::string EgoTriggerDistance = "EgoTriggerDistance";
	
	double egoTriggerDistance;
	uint32_t triggerIndex;
	bool triggered = false;

	bool found_trigger;

	void show_results(){
		auto ego_trigger = found_trigger? " ": " not ";

		auto ego_trigger_v = found_trigger? " valid value: ": " invalid value: ";

		std::cout << EgoTriggerDistance << ego_trigger << "found; ";
		
		std::cout << EgoTriggerDistance << ego_trigger_v << egoTriggerDistance << "m" <<"; ";
	}
}EgoUserDate, *p_EgoUserDate;


void get_ego_trigger_distance(const prescan::api::experiment::Experiment& experiment,
	                          EgoUserDate & egoUserDate, const std::string & ego_name) {

	prescan_object_t* userData = nullptr;
	prescan_object_t* elementOut = nullptr;

	prescan_result_t r_user_data = prescan_experiment_get_userdata(experiment.handle(), &userData);
	if (r_user_data == PRESCAN_SUCCESS){
		std::cout << "User Data accessable: ";
	}else{
		std::cout << "User Data not accessable: ";
	}

	prescan_result_t r_ego_trg = prescan_userdata_get_element_by_name(userData, egoUserDate.EgoTriggerDistance.c_str(), &elementOut);
	if(r_ego_trg == PRESCAN_SUCCESS){
		prescan_element_get_value_as_double(elementOut, &egoUserDate.egoTriggerDistance);
		egoUserDate.found_trigger = true;
	}else{
		egoUserDate.found_trigger = false;
	}
	
	
	prescan_object_destroy(&userData);
	prescan_object_destroy(&elementOut);

	egoUserDate.show_results();
};

}
}
}

