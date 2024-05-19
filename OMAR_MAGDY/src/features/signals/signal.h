#pragma once 

#include <stdlib.h>
#include <vector>
#include "../../core/core.h"
#include <string>


enum class sig_exp{
  csv = 0,
  sig = 1
};
  
/*!
  @file   signal.h
  @brief  this file includes the base class "signal" for signals modeling and analysing thier time-domain / frequency-domain properties

*/




using v_container = dataTable<double>;


    //ENUM FOR EASE OF ACCESSING SIGNAL DATA and ARRANGMENT OF COLUMNS
    /// @brief represent the column order of elements in signal_table
    enum dataTable_order{
      _time = 0,
      _val = 1,
      _first_deriv = 2,
      _second_deriv = 3,
      _area = 4,
    };


/// @class signal
/// @brief signal class the parent class for every other signal or any form of time-sorted (time,value) vectors
class signal{
  protected:

  friend class _signal_operation;


  //Basic data
  /// @struct analytics
  /// @brief a basic structure that holds basic analysis results  
  struct _analytics{
    /// @brief avg peak to peak 
    double avg_ptp;                           //CHECKED
    /// @brief max peak to peak
    double max_ptp;                           //CHECKED
    /// @brief min peak to peak
    double min_ptp;                           //CHECKED

    //VALUES MAX/MIN
    /// @brief average value of peaks
    double avg_max_val; //avg amplitude       //CHECKED
    /// @brief average value of troughs
    double avg_min_val;                       //CHECKED
    /// @brief absolute maximum value
    double max_val;                           //CHECKED
    /// @brief absolute maximum value time
    double max_val_time; //time in seconds;   //CHECKED
    /// @brief absolute minimum value
    double min_val;                           //CHECKED
    /// @brief absolute minimum value time
    double min_val_time;                      //CHECKED
 
    //SLOPE MAX/MIN
    double max_dvBdt;                         //IN PROGRESS
    double max_dvBdt_time; //time in seconds; //IN PROGRESS
    double min_dvBdt;                         //IN PROGRESS
    double min_dvBdt_time;                    //IN PROGRESS

    /// @brief dc_offset of the signal
    double dc_offset;                         //CHECKED
    /// @brief total harmonic distorsion for the signal (will be supported with fft implementation)
    double thd;  //total harmonic distortion; //IN PROGRESS
    /// @brief rms for the signal
    double rms;                               //CHECKED
    /// @brief avg of the signal
    double avg;                               //CHECKED
    
    bool is_harmonic;                         //IN PROGRESS
    bool is_periodic;                         //CHECKED

    /// @brief average sampling time of the signal
    double avg_sample_time;                   //CHECKED
    /// @brief the start time of the signal
    double timeStart;                         //CHECKED
    /// @brief the time end of the signal
    double timeEnd;                           //CHECKED
    /// @brief number of samples that the signal holds
    unsigned int samples_num;                 //CHECKED



    double periods_num = 0;                       //CHECKED
    /// @brief base frequency of the signal
    double base_frequency = 0;                    //CHECKED
    /// @brief base frequency but angular
    double base_angular_frequency = 0;            //CHECKED
    /// @brief periodic_time of the signal
    double periodic_time = 0;                     //CHECKED
    /// @brief duty cycle of the signal
    double duty_cycle = 0;                        //CHECKED
  }analytics;

  
//METHODS FOR SETTING AND GETTING VALUES FROM the signal dataset

    /// @brief easy abstracted values insersion
    inline void putValue(double val, int row, int col){
      signal_data.insertData(val,row,col);
    }
    /// @brief easy abstracted values extraction
    inline double getValue(int row, int col){
      return signal_data.getData(row,col);
    }
    /// @brief refresh the dataTable that we use
    inline void refreshData(){
      signal_data.refresh();
    }

using json = nlohmann::json;
    //SETTINGS
    double min_max_accuracy = stod(settings.get_setting("signal","min_max_accuracy"));
    double maxima_diff_rounding = stod(settings.get_setting("signal","maxima_diff_rounding"));
    double minima_diff_rounding = stod(settings.get_setting("signal","minima_diff_rounding"));
    bool smaller_extremas_ignored = stoi(settings.get_setting("signal","smaller_extremas_ignored"));
    double period_diff_accuracy = stod(settings.get_setting("signal","period_diff_accuracy"));
    bool periodic_avg_rms = stoi(settings.get_setting("signal","periodic_avg&rms")); 
    int frequency_calc_type = stoi(settings.get_setting("signal","frequency_calc_type"));

    /// @enum freq_calc_t
    /// @brief decide the frequency calculation type
    enum freq_calc_t{
      triggerLevel = 1,
      peakNdtrough = 2,
      triggerHysteresis = 3,
    };

    /// @struct maximas_minimas
    /// @brief  (time, value) representation of peaks and troughs of a signal
    struct maximas_minimas{
      std::vector<double> value;
      std::vector<double> time;
    };

    v_container signal_data;
    maximas_minimas val_maximas;
    maximas_minimas val_minimas;
    maximas_minimas dvBdt_maximas;
    maximas_minimas dvBdt_minimas;
    /// @brief rising edges detected are stored here mainly used by trigger level frequency calculation and hysteresis frequency calculation
    std::vector<double> rising_trigger_times;     
    /// @brief rising edges detected are stored here mainly used by trigger level frequency calculation and hysteresis frequency calculation
    std::vector<double> falling_trigger_times; 
    /// @brief rising edges detected are stored here mainly used by trigger level frequency calculation and hysteresis frequency calculation
    std::vector<double> rising_periods;
    /// @brief rising edges detected are stored here mainly used by trigger level frequency calculation and hysteresis frequency calculation
    std::vector<double> falling_periods;


    enum class pattern_type{
      periodic = 1,
      transient = 2,
    };


    struct pattern{
      double pattern_start_time;
      double pattern_end_time;
      unsigned int periodsCount = 0;
      pattern_type type = pattern_type::periodic;
      pattern(double  s_time, double e_time){
        pattern_start_time = s_time;
        pattern_end_time = e_time;
      }
    };

    std::vector<pattern> periods_pattern; 

    struct _subSignals{
      std::vector<signal> subSignals;
      unsigned int transients_count = 0;
      unsigned int unique_periods_count = 0;
    } subSignals_period_based,  subSignals_value_based;


    /// @brief signal value for detecting edges and calculating frequency based on
    double _trigger_level = 0;
    double _hysteresis_high_threshold = 5;
    double _hysteresis_low_threshold = -5; 
    int minimum_periodic_periodNum = stoi(settings.get_setting("signal", "minimum_periodic_periodNum") );


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
    /// @brief INTERPOLATIONS FOR FIXING LOW SAMPLE SIGNALS/DIFFERENT SAMPLING RATES
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
    /// @brief calculate frequency based on Local maximas and minimas and their times respectively 
    bool frequency_peakNdtrough();
    /// @brief calculate frequency based on crossing trigger level times
    bool frequency_triggerLevel();
    /// @brief calculate the base_frequency with hysteresis added for noise ignorance
    bool frequency_triggerHysteresis();
    /// @brief deduce base frequency + angular + number of periods for the signal
    bool deduce_baseFrequency();
    /// @brief rms and avg based on integer number of signals analysis only
    bool deduce_avg_rms();
    /// @brief VALUES ANALYTICS THAT EVALUATE MAXIMAS_MINIMAS using SLOPE DATA
    bool update_slope_maximas_minimas();
    /// @brief  analyse regions of the signal basesd on their periodic times (connected similar periodic times = 1 continous signal)
    ///         periodic time rapid change/phase shifts in a period = transients;
    ///         connected transients are considered a singular non periodic signal;
    bool period_pattern_analysis();

    /// @brief analyse changes in the signal and store any continous patterns detected for future interval Based signal analysis 
    bool pattern_analyze();


    std::vector<double[2]> forrierTransform(signal base_sig); //IN PROGRESS
  
    bool timeDomain_analysed = false;
    bool data_viable = false;


    
  public:
    /// @brief get value peaks in the signal
    /// @return structure with time,value vector (maximas_minimas)
    const maximas_minimas* get_valMaximas()const{
      return &val_maximas;
    }
    /// @brief get value troughs in the signal
    /// @return structure with time,value vector (maximas_minimas)
    const maximas_minimas* get_valMinimas()const{
      return &val_minimas;
    }

    const maximas_minimas* get_dvBdtMaximas()const{
      return &dvBdt_maximas;
    }
    const maximas_minimas* get_dvBdtMinimas()const{
      return &dvBdt_minimas;
    }

    /// @brief check if the signal was analysed in the time domain
    /// @return true if it was analysed before 
    const bool isTimeAnalysed(){
      return timeDomain_analysed; 
    }

    bool dataViable();

    /// @brief load data from a file directly if it has the following format (time,value)
    /// @param name the name of the file including the extension
    /// @param fileLocation the location or path to the file default argument is path from config.json file
    /// @return boolean for success of operation
    bool loadData(std::string name, std::string fileLocation = settings.get_setting("signal","import_path"));

    /// @brief load data first two columns from a dataTable structure into this signal with order (time,value)
    /// @param _data the dataTable that gets passed by value (seperate copy for signal data)
    /// @return boolean for success of operation
    bool loadData(v_container _data);

    /// @brief load data from a time vector and a values vector
    /// @param time the time vector passed by value
    /// @param vals the values vector passed by value
    /// @return boolean for success of operation
    bool loadData(std::vector<double> time,std::vector<double> vals);
  
    /// @brief generalized huge analysis in the time domain fetches basic data
    /// @return boolean for success of operation
    bool analyse();


    /// @brief export valuable signal report (images and text)//in the pdf format
    /// @param name the name of the file to export to
    /// @param file_address  the file address to to export to
    /// @return boolean for success of operation
    bool pdf_export(std::string name, std::string file_address = settings.get_setting("signal","export_path"));
    
    /// @brief export the signal data to a specific file
    /// @param name the name of the file to export to
    /// @param export_all if true will export all time mapped signal data including areas and slopes if false only values and times get exported
    /// @param expType the format to export the signal in Options (sig_exp::csv default) 
    ///                 (sig_exp::sig compressed binary format representation of the signal)
    /// @param fileLocation the file location to export to default is export_path from config.json 
    bool exportSignal(std::string name , bool export_all = false, sig_exp expType = sig_exp::csv, std::string fileLocation = settings.get_setting("signal","export_path"));
    
    /// @brief import a signal in the .sig binary format
    /// @param name name of the file
    /// @param fileLocation path to the file default is import_path in config.json
    bool importSignal(std::string name , std::string fileLocation = settings.get_setting("signal","import_path"));
    
    /// @brief sets the trigger level for the trigger level based frequency calculation
    /// @param v value of the trigger level
    void set_trigger_level(double v);
  
    /// @brief sets the hysteresis parameters for the hysteresis trigger frequency calculations
    /// @param upThreshold upper threshold for the hysteresis block
    /// @param lowThreshold lower threshold for the hysteresis block
    void set_hysteresis(double upThreshold, double lowThreshold);
      
    /// @brief get analytics of the time_domain analysed signal 
    ///@return analysis structure member of this signal as const  
    const _analytics* get_analytics()const;
    
    /// @brief get any values in the signal data table
    /// @return dataTable holding signal_data as const 
    const v_container* get_signal_data()const;
    
    
    /// @brief a function that keeps generating subsignals based on the pattern provided that holds pattern start time and pattern endtime
    /// @param pattern the pattern vector provided for subsignals generation
    /// @param sig the vector of type signal to return subsignals in
    void make_subsignals(std::vector<pattern> &pattern, _subSignals &sig);


    /// @brief get the subSignals generated from this signal based on their periodic times pattern
    /// @return a pointer to the subSignal structure
    _subSignals* subSignal_periodBased();
    /// @brief get the subSignals generated from this signal based on their rms for periods times pattern
    /// @return a pointer to the subSignal structure
    _subSignals* subSignal_valueBased();

};







