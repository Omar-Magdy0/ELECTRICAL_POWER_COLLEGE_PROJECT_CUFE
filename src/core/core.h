#pragma once


/*!
  @file core.h
  @brief wrap the core folder / include base helper methods and macros used accross the program

*/



#include "data_IO/data_IO.h"
#include "data_IO/pdfWrapper.h"
#include <stdlib.h>
#include <type_traits>
#include <string>
#include "settings/setting.h"


#define toRad(_deg)((_deg/180)  * M_PI) 
#define toDeg(_rad)((_rad/M_PI) * 180)


#define positive 1
#define zero 0
#define negative -1
#define sign(c)((c>0) ? positive : ((c<0) ? negative : zero))


using namespace std;

extern _settings settings;



template <typename t>
struct eng_multiplier{

static inline t to_millis(t v)
  {
    if(is_arithmetic<t>::value){
      return v/1e-3;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e-3)+"10^-3";
    }
  }


static inline t to_micros(t v)
  {
    if(is_arithmetic<t>::value){
      return v/10e-6;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e-6)+"10^-6";
    }
  }


static inline t to_nanos(t v)
  {
    if(is_arithmetic<t>::value){
      return v/10e-9;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e-9)+"10^-9";
    }
  }


static inline t to_kilos(t v)
  {
    if(is_arithmetic<t>::value){
      return v/10e3;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e3)+"10^3";
    }
  }


static inline t to_megas(t v)
  {
    if(is_arithmetic<t>::value){
      return v/10e6;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e6)+"10^6";
    }
  }


static inline t to_gigas(t v)
  {
    if(is_arithmetic<t>::value){
      return v/1e9;
    }
    else if(is_same_v<t,string>)
    {
      return std::to_string(stod(v)/1e9)+"10^9";
    }
  }
};


#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime>
#include <cmath>
//--------------HELPER METHODS---------------------//
#define toRad(_deg)((_deg/180)  * M_PI) 
#define toDeg(_rad)((_rad/M_PI) * 180)

inline double constrain(double _rad){
  _rad = fmod(_rad + M_PI , 2 * M_PI);
  if(_rad < 0)_rad += 2*M_PI;
  return _rad - M_PI;
}


inline bool isInDomain(double start,double end,double current){
  if((start <= current)&&(current <= end)){
    return true;
  }else{
    return false;
  }
}

inline bool isNear(double v1, double v2, double acc){
  if( (v1 >= v2*(1  - acc)) &&   (v1 <= v2*(1  + acc)) ){
    return true;
  }else{
    return false;
  }
}

template <typename t>
inline t roundTo(t num,t n){
  return round(num/n)*n;
}



inline float getCurrentTime(){
  return float(clock()/CLOCKS_PER_SEC);
}
inline void replace_char(char c){
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

inline std::string bool_to_string(int v){
  if(v != 0){
    return "true";
  }else{
    return "false";
  }
}
