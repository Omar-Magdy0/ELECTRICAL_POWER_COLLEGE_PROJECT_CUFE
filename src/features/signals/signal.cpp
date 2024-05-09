#include "signal.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

// EXPECTED DATA FORMAT FIRST COLUMN TIME SECOND COLUMN VALUES
//FUTURE WORK SUBDIVIDE SIGNALS WHEN FREQUENCY CHANGE OR PHASE ANGLE CHANGES SUDDENLY
//FOR SUB_DIVIDED ANALYSES FOR EACH PART



bool signal::loadData(string name, string fileLocation){
    //CONSTRUCTOR
  file_IO importFile;
  if(importFile.data_import(fileLocation+name,  &signal_data, csv)){
    refreshData();
    return true;
  }else{
    return false;
  }
}

bool signal::loadData(v_container _data)
{
  signal_data = _data;
  return true;
}

bool signal::loadData(std::vector<double> time, std::vector<double> vals)
{
  if(time.size() != vals.size()){
    std::cerr<< "TIME VECTOR SIZE DIFFERENT THAN VALUE VECTOR" << endl;
    return false;
  }else{
    for( int idx = 0; idx < time.size(); idx++){
      putValue(vals.at(idx) , idx , _val);
      putValue(vals.at(idx) , idx , _time);
    }
    return true;
  }
}

bool signal::dataViable(){
  return this->data_viable;
}

double signal::_dvBdt(double v1, double v3, double t1, double t2){
  return (v1 - v3)/((t1 - t2)*2); //function for first derivative of discrete data
}


double signal::_dv2Bdt2(double v1, double v2, double v3,double t1, double t2){
  return (v1 - 2*v2 + v3) / ((t1-t2)*(t1-t2));    //function for second derivative of discrete data 
}

double signal::_vdt(double v1, double v2, double t1, double t2){
  return (v1 + v2) * (t1 - t2) * 0.5; //RETURNS _AREA OF TRAPEZOID
}




/**
 * @brief does pre analytic operation like getting slopes and _areas wrt and adds them to the datatable data 
 * 
*/
bool signal::pre_analyze()
{

    this->data_viable = true;



//CHECK IF THE ONCE PASSED TABLE HAS AT LEAST 2 COLUMNS for TIME and Values respectively
  if(signal_data.get_col_num() < 2)
  {
    std::cerr << "THE CHOSEN DATA TABLE HAS AT LEAST A ROW MISSING" << endl;
    return false;
  }else if(signal_data.get_row_num() < 10){
    std::cerr << "TOOO FEW SAMPLES PRE ANALYZER TERMINATE" << endl;
    return false;
  }else{

  

  //START DOING BASIC ANALYTICS   
    unsigned int row_index = 0;
    double current_val = getValue(row_index,_val);
    double next_val = getValue(row_index + 1,_val);
    double current_time = getValue(row_index,_time);
    analytics.timeStart = current_time;
    double max_amp = current_val;
    double min_amp = current_val;

    double vdt = 0;

    double last_val = current_val;
    double last_time = current_time;
    //ENABLE THE DATA VIABLE Variable and if data is not disable it

    //START FILLING COLUMNS FOR integration with respect to time , first derivative , second derivative;
    for(row_index = 1; row_index < (signal_data.get_row_num() - 1); row_index++){
      current_val = getValue(row_index,_val);
      next_val = getValue(row_index + 1,_val);
      current_time = getValue(row_index,_time);

      if(last_time > current_time){
        std::cerr << "CORRUPTED DATA  " << row_index << endl;
        data_viable = false;
        return false;
      }
      
      double current_dvBdt = _dvBdt(last_val,  next_val, last_time, current_time);
      double current_dv2Bdt2 = _dv2Bdt2(last_val,current_val,next_val, current_time, last_time);
      
      vdt += _vdt(last_val,  current_val, current_time, last_time);

      putValue(current_dvBdt,row_index,_first_deriv);
      putValue(current_dv2Bdt2,row_index,_second_deriv);
      putValue(vdt,row_index,_area);


      last_val = current_val;
      last_time = current_time;
    }

    current_val = getValue(row_index,_val);
    current_time = getValue(row_index,_time);
    analytics.samples_num = row_index + 1;

    vdt += _vdt(last_val,  current_val, current_time, last_time);
    putValue(vdt,row_index,_area);
    
    analytics.timeEnd = current_time;
    analytics.avg_sample_time = (analytics.timeEnd - analytics.timeStart)/(analytics.samples_num - 1) ;
    refreshData();
    return true;
  }
}


/// @brief VALUES ANALYTICS THAT EVALUATE MAXIMAS_MINIMAS using SLOPE DATA
bool signal::update_local_maximas_minimas()
{
  unsigned int index = 1;
  int current_firstDeriv_sign = sign(getValue(index,_first_deriv));
  int last_firstDeriv_sign = current_firstDeriv_sign;
  //CLEAR ARRAY IF THEY HAD ANY DATA BEFORE SINCE WE ARE DOING PUSHBACKS
  val_maximas.value.clear();
  val_maximas.time.clear();
  val_minimas.value.clear();
  val_minimas.time.clear();
  //ONLY UPDATE LAST DERIV SIGN WHEN SIGN CHANGES FROM NEGATIVE TO POSITIVE OR VICEVERSA
  for(index = 2;  index < (analytics.samples_num - 2);  index++){

    current_firstDeriv_sign = sign(getValue(index,_first_deriv));

    if((last_firstDeriv_sign !=  current_firstDeriv_sign)){
      //IF THE SLOPE CHANGE AS FOLLOW POSITIVE--> NEGATIVE/ZERO ** or NEGATIVE --> POSITIVE/ZERO 
      //IGNORE ZERO SLOPES for now 
      //WE ARE GOING TO GET THE BIGGEST OR THE SMALLEST point from the last change to this change
      last_firstDeriv_sign = current_firstDeriv_sign;
      if((last_firstDeriv_sign != zero)){
        //WE IGNORE CHANGES FROM ZERO SLOPE TO A VALUED SLOPE BUT TAKE CHANGES FROM A VALUED SLOPE TO ZERO
        if(current_firstDeriv_sign == positive){
          //SLOPE was NEGATIVE SIGN AND CHANGED TO POSITIVE ------ MINIMA
          int ridx = -1;
          unsigned int i = index + ridx;
          double localMinima = getValue(i,_val); 
          for(i;  i < (index + 1); i++){
            if(getValue(i,_val) < localMinima){
              localMinima = getValue(i,_val);
              ridx++;
            }
          }
          val_minimas.value.push_back(localMinima);
          val_minimas.time.push_back(getValue(index + ridx,_time));
        }
        else if(current_firstDeriv_sign == negative){
        //SLOPE was POSITIVE SIGN AND CHANGED TO NEGATIVE --------  MAXIMA
                  //SLOPE was NEGATIVE SIGN AND CHANGED TO POSITIVE MINIMA
          int ridx = -1;
          unsigned int i = index + ridx;
          double localMaxima = getValue(i,_val); 
          for(i;  i < (index + 1); i++){
            if(getValue(i,_val) > localMaxima){
              localMaxima = getValue(i,_val);
              ridx++;
            }
          }
            val_maximas.value.push_back(localMaxima);
            val_maximas.time.push_back(getValue(index + ridx,_time));
          }

        else if(current_firstDeriv_sign == zero){
          //SLOPE WAS POSITIVE OR NEGATIVE THEN BECAME ZERO ACT ACCORDINGLY
          int ridx = -1;
          unsigned int i = index + ridx;
          double localMinima = getValue(i,_val); 
          for(i;  i < (index + 1); i++){
            if(getValue(i,_val) < localMinima){
              localMinima = getValue(i,_val);
              ridx++;
            }
          }
            val_minimas.value.push_back(localMinima);
            val_minimas.time.push_back(getValue(index + ridx,_time));
          }
          else if(last_firstDeriv_sign == positive){
            //LAST SLOPE WAS POSITIVE AND NOW ZERO (MAXIMA)
          int ridx = -1;
          unsigned int i = index + ridx;
          double localMaxima = getValue(i,_val); 
          for(i;  i < (index + 1); i++){
            if(getValue(i,_val) > localMaxima){
              localMaxima = getValue(i,_val);
              ridx++;
            }
            val_maximas.value.push_back(localMaxima);
            val_maximas.time.push_back(getValue(index + ridx,_time));
          }
        }
      }
    }
  }
  return true;
  //END OF update_local_maximas_minimas
}

bool signal::post_local_maximas_minimas()
{
  //NOW WE SHOULD HAVE two dataSets of local (maximum and minimum) values and their times respectively
  //+add some analytics data top min/max vals and their times
  /// **BLOCK FOR UPDATING
  // (analytics.min_val -- analytics.max_val)and their times -- analytics.avg_max -- analytics.avg_min
  // ++ updating the local maximas and manimas to hold only the biggest maximas and minimas

  analytics.min_val = val_minimas.value.at(0);
  analytics.min_val_time = val_minimas.time.at(0);
  for(int i = 1; i < val_minimas.value.size(); i++){
    if(val_minimas.value.at(i) < analytics.min_val){
      analytics.min_val = val_minimas.value.at(i);
      analytics.min_val_time = val_minimas.time.at(i);
    }
  }  

  analytics.max_val = val_maximas.value.at(0);
  analytics.max_val_time = val_maximas.time.at(0);
  for(int i = 1; i < val_maximas.value.size(); i++){
    if(val_maximas.value[i] > analytics.max_val){
      analytics.max_val = val_maximas.value[i];
      analytics.max_val_time = val_maximas.time[i];
    }
  }

  //filter the maximas and minimas for top maximas and minimas only and other local one are ignored for now
  /*WE ROUND THE DIFFERENCE BY A FACTOR TO (minima_rounding) COMPENSATE FOR VERY CLOSE TO ZERO VALUES 
    THEN COMPARE THE RATIO OF THE DIFFERENCE TO THE MIN_MAX TO ANOTHER FACTOR(min_max_accuracy) to compensate for offset sampling or low sampling rate 
  */
 if(smaller_extremas_ignored){
  for(int i = 0; i < val_minimas.value.size();){
    double diff =  (val_minimas.value.at(i) - analytics.min_val);
    if( abs( ( roundTo(diff, minima_diff_rounding)/analytics.min_val) ) >  min_max_accuracy ) {
      //erase minimas or maximas that are far than the smallest local minima by a certain factor
      val_minimas.value.erase(val_minimas.value.begin() + i);
      val_minimas.time.erase(val_minimas.time.begin() + i);
    }else{
      i++;
    }
  }
  
  for(int i = 0; i < val_maximas.value.size();){
    double diff = (val_maximas.value.at(i) - analytics.max_val);
    if( abs( ( roundTo(diff, maxima_diff_rounding)/analytics.max_val) ) >  min_max_accuracy ){
      //erase maval_maximas or maximas that are far than the biggest local maxima by a certain factor
      val_maximas.value.erase(val_maximas.value.begin() + i);
      val_maximas.time.erase(val_maximas.time.begin() + i);
    }else{
      i++;
    }
  }
 }
  //CALL SHRINK TO FIT FOR THE VECTORS
  val_maximas.value.shrink_to_fit();
  val_maximas.time.shrink_to_fit();
  val_minimas.value.shrink_to_fit();
  val_minimas.time.shrink_to_fit();


  double sum_maxes = 0;
  for(int i = 0; i < val_maximas.value.size(); i++){
    sum_maxes+=val_maximas.value[i];
  }
  analytics.avg_max_val = sum_maxes/val_maximas.value.size();

  double sum_mins = 0;
  for(int i = 0; i < val_minimas.value.size(); i++){
    sum_mins+=val_minimas.value[i];
  }
  analytics.avg_min_val = sum_mins/val_minimas.value.size();


  //get peak to peak data
  size_t ptp_num = val_minimas.value.size();
  if(val_maximas.value.size() < val_minimas.value.size()) ptp_num = val_maximas.value.size();
  double max_ptp = 0;
  double min_ptp = 0;
  if(ptp_num > 0){
    max_ptp = val_maximas.value.at(0) - val_minimas.value.at(0);
    min_ptp = val_maximas.value.at(0) - val_minimas.value.at(0);

    double sum_ptp = max_ptp;
    
    for(int i = 1; i < ptp_num; i++){
      double ptp = val_maximas.value[i] - val_minimas.value[i];
      sum_ptp+=ptp;
      if(ptp < min_ptp){
          min_ptp = ptp;
        }else if(ptp > max_ptp){
          max_ptp = ptp;
        }
    }
      analytics.max_ptp = max_ptp;
      analytics.min_ptp = min_ptp;
      analytics.avg_ptp = sum_ptp/ptp_num;
      analytics.dc_offset = (analytics.avg_max_val + analytics.avg_min_val)/2;
  }else{
    std::cerr << "NOT ENOUGH PEAKS " << endl;
  }
  return true;
  //END OF post_local_maximas_minimas
}

bool signal::deduce_baseFrequency()
{
  size_t least_extrema_num = val_maximas.value.size() - 1;
  if((val_minimas.value.size() - 1) < least_extrema_num) least_extrema_num = val_minimas.value.size() - 1;
  double sumFrequency_maximaBased = 0;
  double sumFrequency_minimaBased = 0;
  std::vector<double> _minima_periods;
  std::vector<double> _maxima_periods;
  bool maxima_periodic;
  bool minima_periodic;
  for(int i = 0;  i < least_extrema_num; i++){
    double maxima_subPeriod = 1/(val_maximas.time.at(i + 1) - val_maximas.time.at(i));
    sumFrequency_maximaBased += maxima_subPeriod;
    _maxima_periods.push_back(  (val_maximas.time.at(i + 1) - val_maximas.time.at(i)) );
  }
  double base_frequency_maximaBased = sumFrequency_maximaBased/(least_extrema_num);

  for(int i = 0;  i < least_extrema_num; i++){
    double minima_subPeriod = 1/(val_minimas.time.at(i + 1) - val_minimas.time.at(i));
    sumFrequency_minimaBased += minima_subPeriod;
    _minima_periods.push_back(  (val_minimas.time.at(i + 1) - val_minimas.time.at(i)) );
  }

  double base_frequency_minimaBased = sumFrequency_minimaBased/(least_extrema_num);

  //CHECK that all maxima-to-maxima periods where equal 
  if(_maxima_periods.size() > 1){
    //START by assuming it is periodic unless its not
    maxima_periodic = true;
    for(int i = 0;  i < (_maxima_periods.size() - 1); i++){
      if(!isNear(_maxima_periods.at(i),  _maxima_periods.at(i + 1), period_diff_accuracy)){
        maxima_periodic = false;
      }
    }
  }else{
    maxima_periodic = true;
  }
  if(_minima_periods.size() > 1){
    minima_periodic = true;
    for(int i = 0;  i < (_minima_periods.size() - 1); i++){
      if(!isNear(_minima_periods.at(i), _minima_periods.at(i + 1),  period_diff_accuracy)){
        minima_periodic = false;
      }
    }
  }else{
    minima_periodic = true;
  }

  //check that all minima-to-minima periods where equal

  //DEDUCE IF THE FUNCTION IS PERIODIC IF time between maximas equals time between minimas and act accordingly
    if(maxima_periodic && minima_periodic){
      if( isNear(base_frequency_minimaBased,  base_frequency_maximaBased,   period_diff_accuracy) ){
      analytics.base_frequency = (base_frequency_maximaBased + base_frequency_minimaBased)/2;
      analytics.base_angular_frequency = analytics.base_frequency*M_PI*2;
      analytics.periods_num = (analytics.timeEnd - analytics.timeStart)*analytics.base_frequency;
      analytics.periodic_time = 1/analytics.base_frequency;
      analytics.is_periodic = true;

      unsigned int idx = 0;
      double t_on = 0;
      double t_off = 0;
      double sumDuty = 0;
      for(idx;  idx < least_extrema_num; idx++){
        t_on = val_minimas.time.at(idx+1) - val_minimas.time.at(idx); 
        t_off = val_maximas.time.at(idx+1) - val_maximas.time.at(idx);
        sumDuty += t_on/(t_on + t_off);
      }
      analytics.duty_cycle = sumDuty/idx;
    }
  }else{
    return false;
  }
  return true;
}

bool signal::deduce_avg_rms()
{  
  double startTime_stamp = analytics.timeStart; 
  double endTime_stamp = 0;
  if(periodic_avg_rms){
    endTime_stamp = analytics.periodic_time * floor(analytics.periods_num) ; 
  }else{
    endTime_stamp = analytics.timeEnd;
  }
  
  unsigned int index = 0;
  double currentTime = 0;
  double lastTime = getValue(index,_time);
  double currentVal = 0;
  double lastVal = getValue(index,_val);
  double vdt = 0;
  double v2dt = 0;
  index++;
  //NOW WE GET THE DATA START INDEX AGAIN
  for(index; (currentTime <= (endTime_stamp)) && (index < analytics.samples_num); index++){
    currentTime = getValue(index,_time);
    currentVal = getValue(index,_val);
    vdt += _vdt(currentVal, lastVal, currentTime, lastTime);
    v2dt += _vdt((currentVal*currentVal),  (lastVal*lastVal),  currentTime,  lastTime);
    lastTime = currentTime;
    lastVal = currentVal;
  }
  analytics.avg = vdt/(endTime_stamp - startTime_stamp);
  analytics.rms = sqrt( v2dt/(endTime_stamp - startTime_stamp) );
  return true;
}

bool signal::soft_analyze(){

//CHECK FOR LOCAL AND MINIMUM MAXIMAS
  update_local_maximas_minimas();
  post_local_maximas_minimas();
  deduce_baseFrequency();
  deduce_avg_rms();
  return true;
}




bool signal::analyse(){

  if(pre_analyze()){
    //FUTURE WORK THAT INCLUDES TRANSFORM NON EQUALY TIME-SPACED discrete signal into equally time-spaced discrete signal using approximation techniques
    soft_analyze();
    timeDomain_analysed = true;
    return true;
  }
  else{
    return false;
  } 
}


const signal::_analytics* signal::get_analytics()const
{
  return &analytics;
}

const v_container* signal::get_signal_data() const
{ 
  return &signal_data;
}


bool signal::exportSignal(string name, bool export_all, sig_exp exportType, string fileLocation){
  if(exportType == sig_exp::csv){  
    file_IO file;
    refreshData();
    if(export_all){
      if(file.data_export((fileLocation+name), signal_data, csv)){
        return true;
      }
    //if something bad happens when exporting
      return false;
    }else{
      std::vector<double> time;
      signal_data.extractColumn(_time,time);
      std::vector<double> values;
      signal_data.extractColumn(_val,values);
      v_container wrap;
      wrap.insertColumn(_time,time);
      wrap.insertColumn(_val,values);
      if(file.data_export((fileLocation+name), wrap, csv)){
        return true;
      }
    //if something bad happens when exporting
      return false;
    }
  }else if(exportType == sig_exp::sig){
    std::ofstream file(fileLocation+name+".sig", std::ios::out | std::ios::binary);
    if(file.is_open()){
      if(!this->timeDomain_analysed)this->analyse();
      double startTime = this->analytics.timeStart;
      double endTime = this->analytics.timeEnd;
      double t_sample = this->analytics.avg_sample_time;
      double time_comp[] = {startTime , t_sample, endTime};

      std::vector<double> values;
      signal_data.extractColumn(_val,values);

      file.write(reinterpret_cast<const char*>(time_comp) ,sizeof(double)*3);
      file.write(reinterpret_cast<const char*>(values.data()), values.size()*sizeof(double));

      file.close();
      return true;
    }else{
      return false;
    }
  }else{
    return false;
  }
}
