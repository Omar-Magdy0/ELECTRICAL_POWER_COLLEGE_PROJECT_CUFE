#pragma once 

#include <stdlib.h>
#include <vector>
#include "../../core/core.h"
#include <string>



  





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

  
//METHODS FOR SETTING AND GETTING VALUES FROM the signal dataset
    inline void putValue(double val, int row, int col){
      signal_data.insertData(val,row,col);
    }

    inline double getValue(int row, int col){
      return signal_data.getData(row,col);
    }

    inline void refreshData(){
      signal_data.refresh();
    }

using json = nlohmann::json;
    double min_max_accuracy = stod(settings.get_setting("signal","min_max_accuracy"));
    double maxima_diff_rounding = stod(settings.get_setting("signal","maxima_diff_rounding"));
    double minima_diff_rounding = stod(settings.get_setting("signal","minima_diff_rounding"));
    bool smaller_extremas_ignored = stoi(settings.get_setting("signal","smaller_extremas_ignored"));
    double period_diff_accuracy = stod(settings.get_setting("signal","period_diff_accuracy"));
    bool periodic_avg_rms = stoi(settings.get_setting("signal","periodic_avg&rms")); 

    struct maximas_minimas{
      std::vector<double> value;
      std::vector<double> time;
    };

    dataTable signal_data;
    maximas_minimas val_maximas;
    maximas_minimas val_minimas;
    maximas_minimas dvBdt_maximas;
    maximas_minimas dvBdt_minimas;


    
    double _dvBdt(double v1, double v2, double t1, double t2);
    double _dv2Bdt2(double v1, double v2, double v3, double t1, double t2);
    double _vdt(double v1, double v2, double t1, double t2);
    

    /**
     * @brief Makes variable data out of the time-value data such as slopes and areas wrt to time
     * @return expected returns are the followin
     * check for data viability
     * (--time --values --first derivative --second derivative --integration) table 
     * 
     * */
    bool pre_analyze();
    //INTERPOLATIONS FOR FIXING LOW SAMPLE SIGNALS/DIFFERENT SAMPLING RATES
    void interpolate();
    
    /**
      * @brief soft time tomain analysis
      * @return expected after successfull analysis the following
      * --frequency detection , --local minimas/maximas : time map
      * --periodic avg/rms (per_period) , peak to peak / dc offset data
      * --interpolate data 
    */ 
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
    bool data_viable = false;


    
  public:




    const maximas_minimas* get_valMaximas()const{
      return &val_maximas;
    }
    const maximas_minimas* get_valMinimas()const{
      return &val_minimas;
    }
    const maximas_minimas* get_dvBdtMaximas()const{
      return &dvBdt_maximas;
    }
    const maximas_minimas* get_dvBdtMinimas()const{
      return &dvBdt_minimas;
    }


    const bool isTimeAnalysed(){
      return timeDomain_analysed; 
    }
    bool dataViable();
    bool loadData(string name = "signal", string fileLocation = settings.get_setting("signal","import_path"));
    bool loadData(dataTable _data);
    bool loadData(std::vector<double> time,std::vector<double> vals);
    bool analyse();
    bool exportSignal(string name = "signal" , string fileLocation = settings.get_setting("signal","export_path"));
    const _analytics* get_analytics()const;
    const dataTable* get_signal_data()const;
};



