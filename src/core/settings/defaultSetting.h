#pragma once
#include <json.hpp>


/*!
  @file defaultSetting.h
  @brief default Implementation for setting for each application where the settings files will be generated if not found 
*/


using json = nlohmann::ordered_json;
//DEFAULT SETTING FUNCTION FOR ALL
void default_settings_init(json *def);
void signal_defaults(json *def);
void electrical_defaults(json *def);