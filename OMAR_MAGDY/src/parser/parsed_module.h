#pragma once
#include "../core/core.h"

#include "../features/features.h"
#include <map>


struct parsed_module{
  std::string name;
  virtual void parse_function(std::vector<string>);
};



struct _signal_parser{

  std::map<std::string, signal> signal_instances;
  void parse_function(std::vector<std::string> parsed_list){
    std::string command = parsed_list[1];
    cout << command << endl;


    if(command == "new"){
      
    }else if(command == "multiply"){


    }else if(command == "filter"){


    }else{
      std::cerr << "UNIDENTIFIED COMMAND : " << command << endl;
    }


  }
};
