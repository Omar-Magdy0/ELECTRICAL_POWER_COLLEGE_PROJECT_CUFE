#include "energy_monitor_cli.h"
#include <iostream>
#include "features/signals/signal.h"

/*
  SUBSIGNAL IMPLEMENTATION TEST FOR SIGNAL ANALYSIS FOR DIFFERENT STATES FOR A SIGNAL
  DIVIDE AND CONQUER LIKISH APPROACH WHERE EACH TIME A PERIOD WITH DIFFERENT PERIODIC TIME BREAKS A SET
  THE SET THAT WAS BROKEN BECOMES A UNIQUE SIGNAL THAT DESERVES AN ANALYSIS FOR IT ONLY 


*/

/*!
  @file energy_monitor_cli.cpp
  @brief the main file where the main() function exists and the program starts executing
*/


#include <iostream>
#include "core/core.h"
#include "features/features.h"


#define WS_TO_KWH(s)(s/3.6e6)
using dd_container = dataTable<double>;

void analyticBlock(signal *dummySignal, bool show_peaks_troughs = false);

int main(){
    std::string string_held;
    std::vector<std::string> module_nd_params;
    appliance *appliances = NULL;
    std::string appliance_name;
    _voltage *voltage = NULL;
    _current *current = NULL;
    bool quit = false;

    while(1){

      try{
      if(quit)break;
      unsigned char c = cin.get();
      switch (c)
      {
      case ' ':
        module_nd_params.push_back(string_held);
        string_held.clear();
        break;
      case '\n':
        module_nd_params.push_back(string_held);
        if(module_nd_params.at(0) == "load"){
          
          if(module_nd_params.size() >= 1){
            std::string command = module_nd_params.at(1);
            if(module_nd_params.size() >= 2){
              if(command == "loadData"){
                if(module_nd_params.size() >= 3){
                  std::string file_path = module_nd_params.at(2);
                  dd_container program_table;
                  file_IO file_inOut;
                  cout << "IMPORTING DATA" << endl;
                  file_inOut.data_import(file_path, program_table,csv);
                  cout << "NUMBER OF COLUMNS " << program_table.get_col_num() << endl;
                  cout << "NUMBER OF ROWS " << program_table.get_row_num() << endl;
                  program_table.refresh();
                  std::vector<double> time;
                  std::vector<double> voltage_val;
                  std::vector<double> current_val;
                  cout << "REARANGING DATA" << endl;
                  program_table.extractColumn(0 , time);
                  program_table.extractColumn(1 , voltage_val);
                  program_table.extractColumn(2 , current_val);
                  cout << "IMPORTING DATA" << endl;
                  current = new _current;
                  current->loadData(time, current_val);
                  voltage = new _voltage;
                  voltage->loadData(time, voltage_val);
                  cout << "OK" << endl;
                  time.clear();
                  voltage_val.clear();
                  current_val.clear();
                }
              }else if(command == "exportCurrent"){
                if(module_nd_params.size() >= 2){
                  std::string file_path = module_nd_params.at(2);
                        // Find the last occurrence of '.'
                  size_t pos = file_path.find_last_of(".");
                  if (pos != std::string::npos){
                    std::string extension = file_path.substr(pos + 1);
                    if(extension == "csv"){
                      current->exportSignal(file_path,  false, sig_exp::csv);
                      cout << "OK" << endl;
                    }else if(extension == "sig"){
                      current->exportSignal(file_path,  false, sig_exp::sig);
                      cout << "OK" << endl;
                    }else if(extension == "pdf"){
                      current->pdf_export(file_path);
                      cout << "OK" << endl;
                    }else{
                      cerr << "UNSUPPORTED EXTENSION" << endl;
                    }
                  }else{
                    cerr << "UNSUPPORTED EXTENSION" << endl;
                  }
               }
              }else if(command == "exportVoltage"){
                if(module_nd_params.size() >= 2){
                  std::string file_path = module_nd_params.at(2);
                      // Find the last occurrence of '.'
                  size_t pos = file_path.find_last_of(".");
                  if (pos != std::string::npos){
                    std::string extension = file_path.substr(pos + 1);
                    if(extension == "csv"){
                      voltage->exportSignal(file_path,  false, sig_exp::csv);
                      cout << "OK" << endl;
                    }else if(extension == "sig"){
                      voltage->exportSignal(file_path,  false, sig_exp::sig);
                      cout << "OK" << endl;
                    }else if(extension == "pdf"){
                      voltage->pdf_export(file_path);
                      cout << "OK" << endl;
                    }else{
                     cerr << "UNSUPPORTED EXTENSION" << endl;
                    }
                  }else{
                    cerr << "UNSUPPORTED EXTENSION" << endl;
                  }
                }
              }else if(command == "exportPower"){
                  if(module_nd_params.size() >= 2){
                  std::string file_path = module_nd_params.at(2);
                      // Find the last occurrence of '.'
                  size_t pos = file_path.find_last_of(".");
                  if (pos != std::string::npos){
                    std::string extension = file_path.substr(pos + 1);
                    if(extension == "csv"){
                      appliances->get_power()->exportSignal(file_path,  false, sig_exp::csv);
                      cout << "OK" << endl;
                    }else if(extension == "sig"){
                      appliances->get_power()->exportSignal(file_path,  false, sig_exp::sig);
                      cout << "OK" << endl;
                    }else if(extension == "pdf"){
                      appliances->get_power()->pdf_export(file_path);
                      cout << "OK" << endl;
                    }else{
                     cerr << "UNSUPPORTED EXTENSION" << endl;
                    }
                  }else{
                    cerr << "UNSUPPORTED EXTENSION" << endl;
                  }
                }
              }else if(command == "filter"){
                if(module_nd_params.size() >= 5){
                  double order = stod(module_nd_params.at(3));
                  double cutoff_freq = stod(module_nd_params.at(4));
                  if(module_nd_params.at(2) == "current"){
                    cout << "FILTERING ..." << endl;
                    signal_operation_global.firstO_lowPass_filter(*current,  *current, cutoff_freq, order);
                    cout << "OK" << endl;
                  }else if(module_nd_params.at(2) == "voltage"){
                    cout << "FILTERING ..." << endl;
                    signal_operation_global.firstO_lowPass_filter(*voltage,  *voltage, cutoff_freq, order);
                    cout << "OK" << endl;
                  }
                }
              }else if(command == "getAnalytics"){
                if(module_nd_params.size() >= 3){
                  if(module_nd_params.at(2) == "current"){
                    analyticBlock(current);
                    cout << "OK" << endl;
                  }else if(module_nd_params.at(2) == "voltage"){
                    analyticBlock(voltage);
                    cout << "OK" << endl;
                  }else if(module_nd_params.at(2) == "power"){
                    analyticBlock(appliances->get_power());
                    cout << "OK" << endl;
                  }
                }
                cout << "OK" << endl;
              }else if(command == "setHysteresis"){
                if(module_nd_params.size() >= 5){
                  double lower_threshold = stod(module_nd_params.at(3));
                  double upper_threshold = stod(module_nd_params.at(4));
                  if(module_nd_params.at(2) == "current"){
                    current->set_hysteresis(upper_threshold,  lower_threshold);
                    cout << "OK" << endl;
                  }else if(module_nd_params.at(2) == "voltage"){
                    voltage->set_hysteresis(upper_threshold, lower_threshold);
                    cout << "OK" << endl;
                  }else if(module_nd_params.at(2) == "power"){
                    appliances->get_power()->set_hysteresis(upper_threshold,  lower_threshold);
                    cout << "OK" << endl;
                  }
                }
              }else if(command == "simulate"){
                cout << "SIMULATING ..." << endl;
                for(int idx = 0; idx < appliances->get_voltage()->get_analytics()->samples_num; idx++){
                  appliances->readStep();
                }
                if(appliances->current_tripped())cout << "CURRENT TRIPPED AT : " << appliances->current_tripTime() << endl;
                if(appliances->current_tripped())cout << "VOLTAGE TRIPPED AT : " << appliances->voltage_tripTime() << endl;
                double time_start = appliances->get_power()->get_analytics()->timeStart;
                double time_end = appliances->get_power()->get_analytics()->timeEnd;
                cout << "ENERGY CONSUMPTION : " << WS_TO_KWH(appliances->get_power()->get_energy(time_start,  time_end) ) << endl;
                cout << "ACTIVE : " << appliances->get_power()->get_active() << endl;
                cout << "APPARENT : " << appliances->get_power()->get_apparent() << endl;
                cout << "REACTIVE : " << appliances->get_power()->get_reactive() << endl;
                cout << "COST : " << std::fixed << std::setprecision(10) << tarrif_calc(WS_TO_KWH(appliances->get_power()->get_energy(time_start,  time_end) )) << endl;
                cout << "OK" << endl;
              }else if(command == "clean"){
                if(current != NULL)delete current;
                if(voltage != NULL)delete voltage;
                if(appliances != NULL)delete appliances;
                current = NULL;
                voltage = NULL;
                appliances = NULL;
                cout << "CLEAN" << endl;
              }else if(command == "appliance_model"){
                if(module_nd_params.size() >= 3){
                  std::string name = module_nd_params.at(2); 
                  appliances = new appliance(*voltage, *current, name);
                  cout << "OK" << endl;
                }
              }else{
                cout << "ERROR" << endl;
              }
            }
          } 
        }else if(module_nd_params.at(0) == "QUIT" ){
          cout << "PROGRAM TERMINATING ..." << endl;
          quit = true;
        }
         module_nd_params.clear();
         string_held.clear();

         break;

      default:
        string_held.push_back(c);
        break;
      } 
    }catch(std::exception e){
      std::cerr << "WE CAME ACCROSS UNHANDLED EXCEPTION :( " << e.what() << endl;
    }
  }
}





void analyticBlock(signal *dummySignal, bool show_peaks_troughs){
  dummySignal->analyse();
  std::cout << "TIME_START :: " << dummySignal->get_analytics()->timeStart << endl;
  std::cout << "TIME_END :: " << dummySignal->get_analytics()->timeEnd << endl;
  std::cout << "MAX::"<<dummySignal->get_analytics()->max_val << " at "  <<  dummySignal->get_analytics()->max_val_time << endl;
  std::cout << "MIN::"<<dummySignal->get_analytics()->min_val << " at "  <<  dummySignal->get_analytics()->min_val_time << endl;
  std::cout << "AVG::"<<dummySignal->get_analytics()->avg << endl;
  std::cout << "RMS::"<<dummySignal->get_analytics()->rms << endl;
  std::cout << "SAMPLING_RATE::"<<dummySignal->get_analytics()->avg_sample_time << endl;
  std::cout << "NUM_OF_SAMPLES::"<<dummySignal->get_analytics()->samples_num << endl;
  std::cout << "DATA VIABLE ::" << dummySignal->dataViable() << endl;
  std::cout << "FREQUENCY ::" << dummySignal->get_analytics()->base_frequency << endl;
}


