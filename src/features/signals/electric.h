#pragma once
#include "signal.h"
#include "signal_operation.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


/*!
  @file electric.h
  @brief electric specific classes current/voltage/power  that inherit from signals class while adding thier specific properties 
*/

class _current: public signal{
  public:
    _current(signal sig): signal(sig){
    }
    _current(){
    }
};

class _voltage: public signal{
  public:
    _voltage(signal sig): signal(sig){
    }
    _voltage(){
    }
};


enum power_type{
  INDUCTIVE = 1,
  CAPACITIVE,
  RESISTIVE
};

enum pf_type{
  LAGGING = 1,
  LEADING,
  SYNC
};

class _power: public signal{
  private:
    double active = 0;
    double reactive = 0;
    double apparent = 0;
    double frequency = 0;
    double powerFactor = 0;
    double phase = 0xFFFFFFFFFFFFFFFF;  //just initialize with an impossible phase number to detect if we change it
    bool dynamic_loading = false;
    //We keep the pointers assosiated with the multiplication if any happens
    _current* base_current = NULL;
    _voltage* base_volt = NULL;

  public:

    _power(_voltage &volt, _current &current){
      //GET POWER FROM CURRENT AND VOLTAGE MULTIPLICATION

      signal_operation_global.multiply(volt,current,*this, INTERSECT);


      if(!this->isTimeAnalysed())this->analyse();
      frequency = this->analytics.base_frequency;
      active = this->analytics.avg;
      
      if(stoi(settings.get_setting("electrical","use_rms_for_apparent"))){
        apparent = current.get_analytics()->rms * volt.get_analytics()->rms;
      }else{
        apparent = (this->analytics.avg_ptp)/2;
      }


     //CHECK FOR THE HOW THE LOAD BEHAVIOR IS
     //LOADS WITH VITH VARIABLE BEHAVIOR IE.CHANGED FREQUENCIES/CHANGED PHASE SHIFTS LEAD TO NO PHASE ANGLE CALCULATION TILL NOW
     //WHICH LEADS TO FAULTY REACTIVE COMPONENT
     //HOWEVER LIKE THIS WILL ALWAYS GET REACTIVE COMPONENT AND IGNORE ITS SIGN
      if(stoi(settings.get_setting("electrical","assume_static_load"))){
        phase = -signal_operation_global.phase_diff(volt, current);
        reactive = (apparent - active)*sign(phase);
      }else{
        reactive = (apparent - active);
      }

      powerFactor = (active/apparent);
      base_current = &current;
      base_volt = &volt;
    }

    double get_energy(double time_start,double time_end){
      if(!this->isTimeAnalysed())this->analyse();
      //if time_start is smaller than time_end swap both and multiply by -1(sign) in the end
      int sign = 1;
      if(time_start > time_end){
        double temp = time_end;
        time_end = time_start;
        time_start = temp;
        sign = -1;
      }

      if(time_start > this->get_analytics()->timeEnd)return 0;
      if(time_end < this->get_analytics()->timeStart)return 0;
      unsigned int idx = 0;
      double energy_0 = 0;
      double energy_1 = 0;
      //Search for the time corresponding index in the data and iterate index 
      while(this->getValue(idx, _time) < time_start)idx++;
      if(time_start < this->get_analytics()->timeStart)return 0;
      //capture energy_0
      
      energy_0 = getValue(idx,  _area);
      //Find time end and energy at it
      while( (this->getValue(idx,_time) < time_end) && (idx < analytics.samples_num - 1) ) idx++;
      //capture energy_1
      energy_1 = getValue(idx, _area);
      
      return (energy_1 - energy_0)*sign;
    }

    double get_active()const{
      return active;
    }

    double get_apparent()const{
      return apparent;
    }

    double get_reactive()const{
      return reactive;
    }

    double get_frequency()const{
      return active;
    }

    double get_PF()const{
      return powerFactor;
    }

    int get_loadType()const{
      switch (sign(roundTo(reactive, 0.001)))
      {
      case positive:
        return INDUCTIVE;
        break;
      
      case negative:
        return CAPACITIVE;
      
      case zero:
        return RESISTIVE;

      default:
        return -1;
        break;
      }
    }

    double get_phase()const{
      return phase;
    }

    int pf_type()const{
      switch (sign(roundTo(phase, 0.01)))
      {
      case positive:
        return LAGGING;  
        break;
      case negative:
        return LEADING;
        break;
      case zero:
        return SYNC;
        break;        
      
      default:
        return -1;
        break;
      }
    }

};


/// @brief calculate energy cost based on how much energy was consumed in a month according to egyptian tarrif
/// @param energy_in_month (in Kilo Watt HOURS (KWH))
/// @return cost in EGP
inline double tarrif_calc(double energy_in_month)
{
	double cost=0;
	if (energy_in_month <= 100)
	{
		if (energy_in_month > 50) cost = 0.68 * (energy_in_month - 50) + 50 * 0.58;
		else cost = energy_in_month * 0.58;
	}
	else if (energy_in_month <= 650)
	{
		if (energy_in_month > 350) cost = 1.4 * (energy_in_month - 350) + 1.25 * 150 + 0.83 * 200;
		else if (energy_in_month > 200) cost = 1.25 * (energy_in_month - 200) + 0.83 * 200;
		else cost = 0.83 * energy_in_month;
	}
	else if (energy_in_month <= 1000) cost = energy_in_month * 1.5;
	else cost = energy_in_month * 1.65;
	return cost;
}
