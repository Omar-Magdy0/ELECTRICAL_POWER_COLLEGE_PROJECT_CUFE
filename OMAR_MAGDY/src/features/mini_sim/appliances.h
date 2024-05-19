#pragma once
#include <json.hpp>
#include "../signals/signal.h"
#include "../signals/signal_operation.h"
#include "../signals/electric.h"
#include <fstream>


/*!
  @file appliance.h
  @brief models the appliance using a .json file and contains all the implementations needed for model simulation
         based on current and voltage readings or voltage and impedance tables
*/





class appliance {
  using json = nlohmann::ordered_json;
  private:
    double maximum_tripVoltage = 0;
    double tripVoltage_startTime = 0;
    double maximum_tripVoltage_duration = 0;
    double maximum_tripCurrent = 0;
    double tripCurrent_startTime = 0;
    double maximum_tripCurrent_duration = 0;
    bool disconnect_on_trip = true;
    bool connected = true;
    double R = 0;
    double L = 0;
    double C = 0;
    double real = 0;
    double complex = 0;
    std::string name;
    unsigned int inputSignal_idx = 0;



    bool voltageTripped = false;
    bool currentTripped = false;

    bool unkownCurrent = false;
    signal* impedanceTable = NULL;
    _voltage* volt_input = NULL;
    _current* result_current = NULL; 
    _power* result_power = NULL;


  public:
    appliance(_voltage &_volt_input,  _current &_result_current, std::string _name = "appliance"){
      name = _name;
      //READ APPLIANCE PROPERTIES FROM JSON FILE FIRST IF EXISTS IF IT DOESN'T CREATE ONE 
      ifstream file;
      json in;
      file.open("appliances/"+_name+".json");

      if(file.is_open()){
        in = json::parse(file);
        maximum_tripVoltage = in[_name]["_maximum_tripVoltage"];
        maximum_tripVoltage_duration = in[_name]["_maximum_tripVoltage_duration"];
        maximum_tripCurrent = in[_name]["_maximum_tripCurrent"];
        maximum_tripCurrent_duration = in[_name]["_maximum_tripCurrent_duration"];
        R = in[_name]["_R"];
        L = in[_name]["_L"];
        C = in[_name]["_C"];
        real = in[_name]["_real"];
        complex = in[_name]["_complex"];
        disconnect_on_trip = in[_name]["disconnect_on_trip"];
        
        file.close();
        
      }else{
        init();
      }
    //LINK THE VOLT TO THE APPLIANCE AND THE RESULT CURRENT TOO
    //IF RESULT CURRENT WAS PROVIDED PROVIDE POWER STATISTICS INSTANTLY
      volt_input = &_volt_input;
      result_current = &_result_current;

      if(!volt_input->isTimeAnalysed())volt_input->analyse();
      if(!result_current->isTimeAnalysed())result_current->analyse();

        unkownCurrent = false;
        result_power = new _power(*volt_input,*result_current);
        result_power->analyse();
    }  

    ~appliance(){
      if( (result_current != NULL))free(result_current);
      if(result_power != NULL)free(result_power);
      if(volt_input != NULL)free(result_power);
    }

    

    void readStep(){
      if( ( (inputSignal_idx < volt_input->get_analytics()->samples_num ) || (inputSignal_idx < result_current->get_analytics()->samples_num ) ) && connected){
        double thisInstantVoltage = 0;
        double thisInstantTime = 0;
        double thisInstantCurrent = 0;
  
        if(inputSignal_idx < volt_input->get_analytics()->samples_num){
          thisInstantVoltage = volt_input->get_signal_data()->getData(inputSignal_idx,_val);
          thisInstantTime = volt_input->get_signal_data()->getData(inputSignal_idx,_time);
        }

        if(inputSignal_idx < result_current->get_analytics()->samples_num){
          thisInstantCurrent = result_current->get_signal_data()->getData(inputSignal_idx,_val);
        }

        if(abs(thisInstantVoltage) > abs(maximum_tripVoltage) && !voltageTripped){
          tripVoltage_startTime = thisInstantTime;
          voltageTripped = true;
          if(disconnect_on_trip)connected = false;
        }
        
        if(abs(thisInstantCurrent) > abs(maximum_tripCurrent) && !currentTripped){
          tripCurrent_startTime = thisInstantTime;
          currentTripped = true;
          if(disconnect_on_trip)connected = false;
        }



        // WE ARE GOING TO ADD HERE COMPLEX LOGIC FOR OVERVOLTAGE PROTEXTION INTERVALS 
        // OVER CURRENT PROTECTION INTERVALS 
        // AND OTHER OVER_UNDER FREQUENCY LOGIC

      



  
        inputSignal_idx++;
      }
    }


    void init(double _maximum_tripVoltage = 0,
                    double _maximum_tripVoltage_duration = 0,
                    double _maximum_tripCurrent = 0,
                    double _maximum_tripCurrent_duration = 0,
                    double _R = 0,
                    double _L = 0,
                    double _C = 0,
                    double _real = 0,
                    double _complex = 0,
                    bool _disconnect_on_trip = 0
      ){
      json out;
      ofstream fileout;
      out[name]["_maximum_tripVoltage"] = (_maximum_tripVoltage);
      out[name]["_maximum_tripVoltage_duration"] = (_maximum_tripVoltage_duration);
      out[name]["_maximum_tripCurrent"] = (_maximum_tripCurrent);
      out[name]["_maximum_tripCurrent_duration"] = (_maximum_tripCurrent_duration);
      out[name]["_R"] = (_R);
      out[name]["_L"] = (_L);
      out[name]["_C"] = (_C);
      out[name]["_real"] = (_real);
      out[name]["_complex"] = (_complex);
      out[name]["disconnect_on_trip"] = (_disconnect_on_trip);

      ifstream fileCheck;
      fileCheck.open("appliances/"+name+".json");
      if(!fileCheck.is_open()){
        fileout.open("appliances/"+name+".json");
        fileout << std::setw(4) << out;
        fileout.close();
      }
      fileCheck.close();
    }



    void refresh(){
      ifstream file;
      json in;
      file.open("appliances/"+name+".json");
      if(file.is_open()){
        in = json::parse(file);
        maximum_tripVoltage = in[name]["_maximum_tripVoltage"];
        maximum_tripVoltage_duration = in[name]["_maximum_tripVoltage_duration"];
        maximum_tripCurrent = in[name]["_maximum_tripCurrent"];
        maximum_tripCurrent_duration = in[name]["_maximum_tripCurrent_duration"];
        R = in[name]["_R"];
        L = in[name]["_L"];
        C = in[name]["_C"];
        real = in[name]["_real"];
        complex = in[name]["_complex"];
        disconnect_on_trip = in[name]["disconnect_on_trip"];
      }
      file.close();
    }


    bool voltage_tripped(){
      return voltageTripped;
    }
    bool current_tripped(){
      return currentTripped;
    }
    double current_tripTime(){
      return tripCurrent_startTime;
    }
    double voltage_tripTime(){
      return tripVoltage_startTime;
    }
    _power* get_power(){
      return result_power;
    }

    _current* get_current(){
      return result_current;
    }

    _voltage* get_voltage(){
      return volt_input;
    }
    bool is_connected(){
      return connected;
    }



    bool pdf_export(string name, string file_address = settings.get_setting("signal","import_path"));
};








