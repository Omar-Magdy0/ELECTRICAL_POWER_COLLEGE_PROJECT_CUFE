#pragma once

#include "data_IO/data_IO.h"
#include <stdlib.h>
#include <type_traits>
#include <string>
#include "settings/setting.h"

  using namespace std;

extern _settings settings;

template <typename t>
struct eng_multiplier{

  t to_millis(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e-3;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e-3";
    }
  }


  t to_micros(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e-6;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e-6";
    }
  }


  t to_nanos(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e-9;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e-9";
    }
  }


  t to_kilos(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e3;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e3";
    }
  }


  t to_megas(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e6;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e6";
    }
  }


  t to_gigas(t v)
  {
    if(is_arithmetic<t>::value){
      return v*10e9;
    }
    else if(is_same_v<t,string>)
    {
      return v+"10e9";
    }
  }
};


#include <ctime>
#include <cmath>
inline float getCurrentTime(){
  return float(clock()/CLOCKS_PER_SEC);
}
inline void replace(char c){
  cout << "\b" << c << endl;
}
 

struct process_feedback{
  private:

  float lastPerc = 0;
  float startTime;
  bool update_str = false;
  int char_place_idx = 0;

  public:

  void update_state(float perc,char c,std::string str);
  void load_string(std::string str);
  float time_ellapsed();
  process_feedback();
};