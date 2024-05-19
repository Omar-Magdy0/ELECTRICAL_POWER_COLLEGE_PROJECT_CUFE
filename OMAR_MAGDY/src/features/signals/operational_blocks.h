#pragma once



class hysteresis{
  bool state;
  double low_threshold;
  double high_threshold;

  public:
  hysteresis(double _low_threshold,  double _high_threshold, bool _initial_state = false){
    low_threshold = _low_threshold;
    high_threshold = _high_threshold;
    state = _initial_state;
  };

  bool update_state(double value){
    if(value >= high_threshold){
      state = true;
    }else if(value <= low_threshold){
      state = false;
    }
    return state;
  }
};


class timer_counter{
  double start;

  public:
  timer_counter(double _start){
    start = _start;
  }
  double get_count_duration(double termination){
    return termination - start;
  };
}; 