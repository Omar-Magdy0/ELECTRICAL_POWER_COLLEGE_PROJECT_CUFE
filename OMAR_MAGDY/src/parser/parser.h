
#pragma once

#include <iostream>
#include <istream>
#include "../core/core.h"
#include <sstream>
#include <streambuf>
#include <string>
#include "parsed_module.h"


extern _signal_parser signal_parser;


class parser{
  private:
    void(*custom_handler)() = NULL;
    std::istream *input_stream;
    std::ostream *output_stream;
    std::string string_held;
    std::vector<std::string> module_nd_params;
  
  
  
  public:
    parser(std::istream *_input_stream, std::ostream *_output_stream){
      input_stream = _input_stream;
      output_stream = _output_stream;
    }
    void parse(){
      unsigned char c = input_stream->get();
      switch (c)
      {
      case ' ':
        module_nd_params.push_back(string_held);
        string_held.clear();
        break;

      case '\n':
        module_nd_params.push_back(string_held);
        string_held.clear();
        if(module_nd_params.at(0) == "signal"){
          signal_parser.parse_function(module_nd_params);
        }else if(module_nd_params.at(0) == "electric"){
    
        }else if(module_nd_params.at(0) == "dataTable"){

        }else{
          *output_stream << "UNIDENTIFIED MODULE : " << module_nd_params.at(0) << std::endl;
        }

        module_nd_params.clear();

        break;



      }
      
  
    }








};


