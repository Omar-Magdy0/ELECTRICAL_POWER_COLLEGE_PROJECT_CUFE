#include "signal.h"




//Scalar with signal or signal with signal operations
//FRIEND CLASS TO SIGNALS CAN EDIT THEIR VALUES

enum operation_mode{
  UNION,
  INTERSECT,
};


class _signal_operation{
    private:
      bool lastOperationSuccess;
      float sampleTime_diff_factor = stof(settings.get_settings(sample_time_diff_factor));
      float frequency_diff_factor = stof(settings.get_settings(freq_diff_factor));
    public:
      //signal with a signal arthimetic
    signal add(signal* base_sig1, signal* base_sig2, int mode = INTERSECT);
    signal multiply(signal* base_sig1, signal* base_sig2, int mode = INTERSECT);
    double phase_diff(signal* base_sig1, signal* base_sig2);
    
    double integ_vpower(signal base_sig1, int power);
    

    //Scalar with a signal arthimetic
    signal add(signal *base_sig1,  double val);
    signal multiply(signal *base_sig1, double val);
    signal logarithmic(signal *base_sig);

  //filtering
    enum filterOrder{
      cascade
    };
    //first order filters returns filtered signals
      //second order filters yet to come
    signal lowPass_filter(signal base_sig, int order=1, int t = cascade);
    signal highPass_filter(signal base_sig, int order=1, int t = cascade);
    signal bangReject_filter(signal base_sig, int order=1, int t = cascade);
    signal bandPass_filter(signal base_sig, int order=1, int t = cascade);
    signal factor_frequency(signal base_sig, float increase_ratio);

    /// @brief takes two Vectors one for time and and the other for values respectively and returns a signal
    signal toSignal(std::vector<double> value,  std::vector<double> time);
    /// @brief takes a vector of vectors one for time and and the other for values respectively and returns a signal
    signal toSignal(std::vector<std::vector<double>> table);
    //extracts a subset of a signal with time boundaries
    signal subsignal_time_based(signal base_sig, double time_start,  double time_end);
    //extracts a subset of a signal with value boundaries
    signal subsignal_value_based(signal base_sig, double value1,  double value2);
    //extract a subset of a signal starting from a value for a certain period
    signal subsignal_value_time_based(signal base_sig, double value1,  double time_after);
    signal timeShift(signal base_sig, double timeShift);
    signal concatenate(signal firstSignal, signal secondSignal);


    bool isSuccessfull(){
      return lastOperationSuccess;
    }


};