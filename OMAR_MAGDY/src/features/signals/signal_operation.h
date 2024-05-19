#pragma once
#include "signal.h"



/*!
  @file signal_operation.h
  @brief this file includes the basic signal_operation class responsible for doing any required manipulation or state-altering functions to signals
*/



//Scalar with signal or signal with signal operations
//FRIEND CLASS TO SIGNALS CAN EDIT THEIR VALUES

enum operation_mode{
  UNION,
  INTERSECT,
};



 
class _signal_operation{
    private:
     bool lastOperationSuccess;
     static float samplingRate_diff;
     static float freq_diff_accuracy;
    public:
      //signal with a signal arthimetic
    signal add(signal& base_sig1, signal& base_sig2, int mode = INTERSECT){
      signal temp;
      add(base_sig1,base_sig2,temp,mode);
      return temp;
    }

    void add(signal& base_sig1, signal& base_sig2,  signal& resultant, int mode = INTERSECT);
    
    signal multiply(signal& base_sig1, signal& base_sig2, int mode = INTERSECT){
      signal temp;
      multiply(base_sig1,base_sig2,temp,mode);
      return temp;
    }

    void multiply(signal& base_sig1, signal& base_sig2, signal& resultant, int mode = INTERSECT);
    double phase_diff(signal& base_sig1, signal& base_sig2);
    
    double integ_vpower(signal base_sig1, int power);
    

    //Scalar with a signal arthimetic
    signal add(signal &base_sig1,  double val){
      signal temp;
      add(base_sig1,  temp,  val);
      return temp;
    }
    void add(signal &base_sig1, signal &resultant,  double val);
    signal multiply(signal &base_sig1, double val){
      signal temp;
      multiply(base_sig1, temp, val);
      return temp;
    }
    void multiply(signal &base_sig1, signal &resultant , double val);
    signal logarithmic(signal *base_sig);


    /// @brief  first order IIR(INFINITE IMPULSE RESPONSE)  low pass filter   
    ///         output = (1 - filter_parameter)*last_output + filter_parameter*input for the resultant signal
    /// @param  base_sig  The signal that needs filtering
    /// @param  resultant   The signal that holds the filtered signal can be the base_signal
    /// @param  cutOff_freq   The cutoff frequency for this filter
    /// @param  order  Optional argument for cascading the filter for higher orders default is 1 for first order
    /// @param  avg_sample_time  Optional argument for providing the avg_sample_time and not doing the analysis
    void firstO_lowPass_filter(signal& base_sig,  signal& resultant,double cutOff_freq,int order=1, double avg_sample_time = -1);
    signal highPass_filter(signal base_sig, int order=1);
    signal bangReject_filter(signal base_sig, int order=1);
    signal bandPass_filter(signal base_sig, int order=1);
    signal factor_frequency(signal base_sig, float increase_ratio);

    /// @brief takes two Vectors one for time and and the other for values respectively and returns a signal
    signal toSignal(std::vector<double> value,  std::vector<double> time);
    /// @brief takes a vector of vectors one for time and and the other for values respectively and returns a signal
    signal toSignal(std::vector<std::vector<double>> table);
    



    /// @brief create subSignal from bigger ones based on time boundaries
    /// @param base_sig the Bigger signal we create subsets from
    /// @param sub_sig the smaller signal we extracted
    /// @param time_start the start time of the sub signal
    /// @param time_end the end time of the sub signal
    /// @return true if the sub_signal was extracted successfully .ie time domain intersection else return false
    bool subsignal_time_based(signal &base_sig, signal &sub_sig, double time_start,  double time_end);
    //extracts a subset of a signal with value boundaries
    signal subsignal_value_based(signal &base_sig, double value1,  double value2);
    //extract a subset of a signal starting from a value for a certain period
    signal subsignal_value_time_based(signal &base_sig, double value1,  double time_after);

    signal timeShift(signal &base_sig, double timeShift);
    signal concatenate(signal firstSignal, signal secondSignal);


    bool isSuccessfull(){
      return lastOperationSuccess;
    }
    
    static void refreshSettings();

};

extern _signal_operation signal_operation_global;