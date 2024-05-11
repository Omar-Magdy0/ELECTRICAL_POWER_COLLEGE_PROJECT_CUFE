#include "data_IO.h"

#include <string>
#include <iostream>
#include <iomanip>

using v_container = dataTable<double>;

bool file_IO::csv_import(string file_address, v_container *table){
  ifstream file;
  string cell;
  char c;

  table->get_row_num();

  int row_index = 0;
  int col_index = 0;
  file.open(file_address);
  if(file.is_open()){
    //operations to fill the v_container object from the file
      while(file.get(c)){

        if(c == '\n' || c == ','){

          try{
            table->insertData(stod(cell),  row_index, col_index);
            cell = "";
          }
          catch(const std::invalid_argument& e){
            std::cerr << "FAILED TO READ CORRUPTED FILE  "<< e.what() << endl;
            return false;
          }
          catch(const std::out_of_range& e){
            std::cerr << "VERY LARGE OR VERY SMALL values IN THE FILE  "<<  e.what()  << endl;
            return false;
          }

          if(c == '\n'){
            col_index = 0;
            row_index++;
          }else{
            col_index++;
          }
        }else
        {
          cell.push_back(c);
        }
      }
    file.close();
    return true;
  }
  else{
    return false;
  }
}




bool file_IO::csv_export(string file_address, v_container table){
  ofstream outputFile;
  outputFile << std::setprecision(10);
  outputFile.open(file_address);
  if(!outputFile.is_open()){
    cout << "unexpected error" << endl;
    return false;
  }else{

    unsigned int col_index;
    unsigned int row_index;
    for(row_index = 0; row_index  < table.get_row_num(); row_index++){
      for(col_index = 0; col_index < (table.get_col_num() - 1); col_index++){
        outputFile << table.getData(row_index,col_index) << ',';
      }
      outputFile << table.getData(row_index,col_index) << endl;
    }
    outputFile.close();
    return true;
  }
};



bool file_IO::data_import(string file_address,  v_container *data,  int type){
  switch (type)
  {
  case csv:
    return csv_import(file_address, data);
    break;
  default:
    return false;
    break;
  }
}

bool file_IO::data_export(string file_address, v_container data, int type){
  switch (type)
  {
  case csv:
    return csv_export(file_address, data);
    break;
  default:
    return false;
    break;
  }
}