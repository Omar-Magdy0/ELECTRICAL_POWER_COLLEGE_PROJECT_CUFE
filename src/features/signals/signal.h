#pragma once 

#include <stdlib.h>
#include <vector>
#include "../../core/core.h"
#include <string>


struct _signal_data{
  std::string dataLabels[4] = {"time","val","dv/dt","vdt"};
  dataTable data;
};




   //ENUM FOR EASE OF ACCESSING SIGNAL DATA and ARRANGMENT OF COLUMNS
    enum dataTable_order{
      _time,
      _val,
      _first_deriv,
      _second_deriv,
      _area,
    };



class signal{
  protected:

  friend class _signal_operation;


  //Basic data
  struct _analytics{

    double avg_ptp; //avg peak to peak        //CHECKED
    double max_ptp;                           //CHECKED
    double min_ptp;                           //CHECKED



    //VALUES MAX/MIN
    double avg_max_val; //avg amplitude       //CHECKED
    double avg_min_val;                       //CHECKED
    double max_val;                           //CHECKED
    double max_val_time; //time in seconds;   //CHECKED
    double min_val;                           //CHECKED
    double min_val_time;                      //CHECKED
 
    //SLOPE MAX/MIN
    double max_dvBdt;                         //IN PROGRESS
    double max_dvBdt_time; //time in seconds; //IN PROGRESS
    double min_dvBdt;                         //IN PROGRESS
    double min_dvBdt_time;                    //IN PROGRESS

    double dc_offset;                         //CHECKED
    double thd;  //total harmonic distortion; //IN PROGRESS
    double rms;                               //CHECKED
    double avg;                               //CHECKED
    
    bool is_harmonic;                         //IN PROGRESS
    bool is_periodic;                         //CHECKED




    double avg_sample_time;                   //CHECKED
    double timeStart;                         //CHECKED
    double timeEnd;                           //CHECKED
    unsigned int samples_num;                          //CHECKED

    double periods_num;                       //CHECKED
    double base_frequency;                    //CHECKED
    double base_angular_frequency;            //CHECKED
    double periodic_time;                     //CHECKED
    double duty_cycle;                        //CHECKED
  }analytics;

    
    _signal_data signal_data;
//METHODS FOR SETTING AND GETTING VALUES FROM the signal dataset
    inline void putValue(double val, int row, int col){
      signal_data.data.insertData(val,row,col);
    }

    inline double getValue(int row, int col){
      return signal_data.data.getData(row,col);
    }

    inline void refreshData(){
      signal_data.data.refresh();
    }



    double accuracy_for_min_max = stod(settings.get_settings(signal_defaultAccuracyFactor));
    struct maximas_minimas{
      std::vector<double> value;
      std::vector<double> time;
    };

    maximas_minimas val_maximas;
    maximas_minimas val_minimas;
    maximas_minimas dvBdt_maximas;
    maximas_minimas dvBdt_minimas;


    
    double _dvBdt(double v1, double v2, double t1, double t2);
    double _dv2Bdt2(double v1, double v2, double v3, double t1, double t2);
    double _vdt(double v1, double v2, double t1, double t2);
    

    //Makes variable data out of the time-value data such as slopes and areas wrt to time
    bool pre_analyze();

    bool soft_analyze();
    /// @brief EVALUATE MAXIMAS/MINIMAS using SLOPE DATA
    bool update_local_maximas_minimas();
    /// @brief filter local MAXIMAS and MINIMAS and update ptp data (only top maximas and lowest minimas) 
    bool post_local_maximas_minimas();
    /// @brief deduce base frequency + angular + number of periods for the signal 
    bool deduce_baseFrequency();
    /// @brief rms and avg based on integer number of signals analysis only
    bool deduce_avg_rms();

    bool update_slope_maximas_minimas();
        //frequency analytics
    std::vector<double[2]> forrierTransform(signal base_sig); //IN PROGRESS
  


    bool timeDomain_analysed = false;
    bool _has_data = false;
    bool data_viable = false;
  public:




    maximas_minimas const get_valMaximas(){
      return val_maximas;
    }
    maximas_minimas const get_valMinimas(){
      return val_minimas;
    }
    maximas_minimas const get_dvBdtMaximas(){
      return dvBdt_maximas;
    }
    maximas_minimas const get_dvBdtMinimas(){
      return dvBdt_minimas;
    }


    const bool isTimeAnalysed(){
      return timeDomain_analysed; 
    }
    bool hasData();
    bool dataViable();
    bool loadData(string name = "signal", string fileLocation = settings.get_settings(signal_defaulSignalExportPath));
    bool analyse();
    bool exportSignal(string name = "signal" , string fileLocation = settings.get_settings(signal_defaulSignalImportPath));
    const _analytics get_analytics();
    dataTable get_sig_data()const;
};



inline bool isNear(double v1, double v2, double acc){
  if( (v1 >= v2*(1  - acc)) &&   (v1 <= v2*(1  + acc)) ){
    return true;
  }else{
    return false;
  }
}