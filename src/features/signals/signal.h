#pragma once 

#include <stdlib.h>
#include <vector>
#include "../../core/core.h"
#include <string>

struct _signal_data{
  std::string dataLabels[4] = {"time","val","dv/dt","vdt"};
  dataTable data;
};



class signal{
  protected:
  //Basic data
  struct _analytics{
    double base_frequency;
    
    double avg_ptp; //avg peak to peak

    double max_ptp;
    double min_ptp;


    double avg_max_val; //avg amplitude
    double avg_min_val;


    //VALUES MAX/MIN
    double max_val;
    double max_val_time; //time in seconds;
    double min_val;
    double min_val_time;
 
    //SLOPE MAX/MIN
    double max_dvBdt;
    double max_dvBdt_time; //time in seconds;
    double min_dvBdt;
    double min_dvBdt_time;

    double dc_offset;
    double thd;  //total harmonic distortion;
    double rms;
    double avg;
    
    bool is_harmonic;
    bool is_periodic;



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



    double accuracy_for_min_max = 0.03;
    struct maximas_minimas{
      std::vector<double> value;
      std::vector<double> time;
    };

    maximas_minimas val_maximas;
    maximas_minimas val_minimas;
    maximas_minimas dvBdt_maximas;
    maximas_minimas dvBdt_minimas;

    bool timeDomain_analysed;
    
    double _dvBdt(double v1, double v2, double t1, double t2);
    double _dv2Bdt2(double v1, double v2, double v3, double t1, double t2);
    double _vdt(double v1, double v2, double t1, double t2);
    

    //Makes variable data out of the time-value data such as slopes and areas wrt to time
    bool pre_analyze();

    bool soft_analyze();
    /// @brief VALUES ANALYTICS THAT EVALUATE MAXIMAS_MINIMAS using SLOPE DATA
    bool update_maximas_minimas();
    /// @brief filter local MAXIMAS and MINIMAS and update ptp data (only top maximas and lowest minimas) 
    bool post_maximas_minimas();
    bool update_slope_maximas_minimas();
  



    double timeStart;
    double timeEnd;
    int samples_num;
    bool _has_data;
    bool data_viable;
  public:

   //ENUM FOR EASE OF ACCESSING SIGNAL DATA and ARRANGMENT OF COLUMNS
    enum{
      time,
      val,
      first_deriv,
      second_deriv,
      area
    };

    bool hasData();
    bool dataViable();
    bool loadData(string name = "signal", string fileLocation = settings.get_settings<std::string>(defaulSignalExportPath));
    bool signal_analytics();
    bool exportSignal(string name = "signal" , string fileLocation = settings.get_settings<std::string>(defaulSignalImportPath));
    const _analytics get_analytics();
    dataTable get_sig_data()const;
};



//Scalar with signal or signal with signal operations
struct _signal_operation{

      //signal with a signal arthimetic
    signal add(signal base_sig1,  signal base_sig2);
    signal subtract(signal base_sig1, signal base_sig2);
    signal multiply(signal base_sig1, signal base_sig2);
    signal divide(signal base_sig1, signal base_sig2);
    double phase_diff(signal base_sig1, signal base_sig2);
    
    double integ_vpower(signal base_sig1, int power);
    

    //Scalar with a signal arthimetic
    signal add(signal base_sig1,  double val);
    signal subtract(signal base_sig1, double val);
    signal multiply(signal base_sig1, double val);
    signal divide(signal base_sig1, double val);
    signal logarithmic(signal base_sig);

  //filtering
    enum filterType{
      cascade
    };
    //first order filters returns filtered signals
      //second order filters yet to come
    signal lowPass_filter(signal base_sig, int order=1, int t = cascade);
    signal highPass_filter(signal base_sig, int order=1, int t = cascade);
    signal bangReject_filter(signal base_sig, int order=1, int t = cascade);
    signal bandPass_filter(signal base_sig, int order=1, int t = cascade);
    signal factor_frequency(signal base_sig, float increase_ratio);



    //extracts a subset of a signal with time boundaries
    signal subsignal_time(signal base_sig, double time_start,  double time_end);
    //extracts a subset of a signal with value boundaries
    signal subsignal_value(signal base_sig, double value1,  double value2);
    //extract a subset of a signal starting from a value for a certain period
    signal subsignal_value_till(signal base_sig, double value1,  double time_after);

    //frequency analytics
    std::vector<double[2]> forrierTransform(signal base_sig);
};