#pragma once
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>
#include <fstream>

  enum{
    signal_defaulSignalImportPath,
    signal_defaulSignalExportPath,
    signal_defaultAccuracyFactor,
    sample_time_diff_factor,
    freq_diff_factor
  };

class _settings{

  YAML::Node config;

  std::string signal_exportPath;
  std::string signal_importPath;
  std::string signal_accuracyFactor;
  std::string signal_sampleTimeFactor;
  std::string signal_frequencyFactor;


  public:
  _settings(){
    config = YAML::LoadFile("config.yaml");
    signal_exportPath = config["signal"]["defaultExportPath"].as<std::string>();
    signal_importPath = config["signal"]["defaultImportPath"].as<std::string>();
    signal_accuracyFactor = config["signal"]["defaultAccuracyFactor"].as<std::string>();
    signal_sampleTimeFactor = config["signal"]["max_samplingTime_diff"].as<std::string>();
    signal_frequencyFactor = config["signal"]["operation_max_freqDiff"].as<std::string>();
  }
  bool save_settings(){
    std::ofstream file;
    file.open("config.yaml");
    if(file.is_open()){
      file << config;
      file.close();
      return true;
    }else{
      return false;
    }
  }


  template <typename t>
  void change_settings(int settingToChange,t data){
    switch (settingToChange)
    {
    case signal_defaulSignalImportPath:
        signal_importPath = data;
        config["signal"]["defualtImportPath"] = signal_importPath;
      break;
    
    case signal_defaulSignalExportPath:
        signal_exportPath = data;
        config["signal"]["defaultExportPath"] = signal_exportPath;
      break;

    case signal_defaultAccuracyFactor:
        signal_accuracyFactor = data;
        config["signal"]["defaulAccuracyFactor"] = signal_accuracyFactor;
      break;
    
    case signal_defaultAccuracyFactor:
        signal_sampleTimeFactor = data;
        config["signal"]["max_samplingTime_diff"] = signal_sampleTimeFactor;
      break;

    case signal_defaultAccuracyFactor:
        signal_frequencyFactor = data;
        config["signal"]["operation_max_freqDiff"] = signal_frequencyFactor;
      break;

    default:
      break;
    }
  }

std::string get_settings(int settingToGet){
    switch (settingToGet)
    {
      case signal_defaulSignalImportPath:
        return signal_importPath;
        break;
    
      case signal_defaulSignalExportPath:
        return signal_exportPath;
        break;

      case signal_defaultAccuracyFactor:
        return signal_accuracyFactor;
        break;

      case sample_time_diff_factor:
        return signal_sampleTimeFactor;
        break;
      
      case freq_diff_factor:
        return signal_frequencyFactor;
        break;

      default:
        return "0";
        break;
    }
  }



};