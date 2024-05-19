#pragma once
#include <json.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "defaultSetting.h"



/*!
  @file setting.h
  @brief implements a simple interface for permanent settings across different parts across the program
          using the .json format 

*/

  
class _settings{
  using json = nlohmann::ordered_json;

  json configs;

  public:
  _settings(){ 

    std::ifstream configFileRead("configFile.json");
    if(!configFileRead.is_open()){

      std::ofstream configFileWrite;
      configFileWrite.open("configFile.json");
      default_settings_init(&configs);
      configFileWrite << std::setw(4) << configs;
      configFileWrite.close();
    }else{
      configs = json::parse(configFileRead);
    }
  }



  bool save_settings(){
    std::ofstream configFileWrite("configFile.json");
    configFileWrite << setw(4) << configs << endl;
    return true;
  }



  template <typename t>
  void change_setting(string class_name, string setting,t val){
    configs[class_name][setting] = to_string(val);
  }



  string get_setting(string class_name,string setting){
    return configs[class_name][setting];
  }

};