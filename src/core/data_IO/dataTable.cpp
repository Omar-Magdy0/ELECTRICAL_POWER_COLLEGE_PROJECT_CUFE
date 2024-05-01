#include "dataTable.h"



dataTable::dataTable(){
  _rows_num = 0;
  _cols_num = 0; 

}


unsigned int dataTable::get_row_num(){
  return _rows_num;
}



unsigned int dataTable::get_col_num(){
  return _cols_num;
}

#include <iostream>

    /**
      @brief inserts a value to a cell of dataTable instance with that address
    */
void dataTable::insertData(double d,  unsigned int row , unsigned int col){
  if(row >= _rows_num){
    _rows_num = row + 1;
  }
  if(col >= _cols_num){
    _cols_num = col + 1;
  }

  if(row >= _table.size())_table.resize(row + 1);
  if(col >= _table.at(row).size())_table.at(row).resize(col + 1);
  _table.at(row).at(col) = d;
}

    /**
      @brief returns the value of a cell with that address
    */

double dataTable::getData(int row, int col){
  return _table.at(row).at(col);
}
    /**
      @brief entirely deletes the rows and frees their memory
      @param start_row the row to start deleting from
      @param rows_num how many rows to keep deleting after the start row
    */

void dataTable::eraseRow(int start_row, int rows_num){
  _table.erase(_table.begin() + start_row, _table.begin() + start_row + rows_num);
  _rows_num-= rows_num;
} 
    /**
      @brief entirely deletes colunms and frees thier memory while keeping order
      @param start_col the column to start deleting from
      @param cols_num how many columns to keep deleting after the start row
    */

void dataTable::eraseColumn(int start_col, int cols_num){
  for(int i = 0; i < _table.size(); i++){
    _table.at(i).erase(_table.at(i).begin() + start_col , _table.at(i).begin() + start_col + cols_num);
  }
  _cols_num-= cols_num;
} 
    /**
      @brief refreshes the data Table for any mistake probability or bad handling
    */

void dataTable::refresh(){
  for(unsigned int i = 0; i < _rows_num; i++){
    if(_table.at(i).size() <= _cols_num){
      _table.at(i).resize(_cols_num);
    }
  }
}


  