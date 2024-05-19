#pragma once 
#include "../signals/signal.h"
#include "signal_bus.h"

class source : public signal{
  private:
    std::string label;

  public: 
  source(signal sig,std::string _label): signal(sig), label(_label){

  }
  void connectTo(signal_bus *bus){
      
  }
};