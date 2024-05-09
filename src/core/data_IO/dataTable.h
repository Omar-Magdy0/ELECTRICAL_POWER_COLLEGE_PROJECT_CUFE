#pragma once

/*
SINCE CLASSES ARE TEMPLATED 
IMPLEMENTATION AND PROTOTYPES ARE IN HEADER FOR NO-ERROR COMPILING

*/

#include <vector>
#include <string>
#include <iostream>
using namespace std;
#define VECTOR_OVERSIZE 1

/*
  DYNAMIC DATA HOLDER THAT KEEPS DATA IN RECTANGULAR 2D arrays for now
*/

/*DERIVED CLASS EFFICIENT FOR HORIZONTAL DATA STORAGE*/
template <typename HELD_DATA>
class dataTable{

  private:
    unsigned int _cols_num = 0;
    unsigned int _rows_num = 0;
    vector<vector<HELD_DATA>> _table; 


  public:
    unsigned int get_row_num(){
      return _rows_num;
    }
    unsigned int get_col_num(){
      return _cols_num;
    }

    ///  @brief inserts a value to a cell of dataTable instance with that address
    void insertData(HELD_DATA d, unsigned int row , unsigned int col){
      if(row >= _rows_num){
        _rows_num = row + 1;
      }
      if(col >= _cols_num){
        _cols_num = col + 1;
      }

    if(col >= _table.size())_table.resize(col + VECTOR_OVERSIZE);
    if(row >= _table.at(col).size())_table.at(col).resize(row + VECTOR_OVERSIZE);
      _table.at(col).at(row) = d;
    }

    /// @brief return the data at some address
    /// @param row the row number of data you want
    /// @param col column number of this data
    /// @return the data you want
    HELD_DATA getData(int row , int col)const {
        return _table.at(col).at(row);
    }


    /**
      @brief entirely deletes the rows and frees their memory
      @param start_row the row to start deleting from
      @param rows_num how many rows to keep deleting after the start row
    */
    void eraseRow(int start_row, int rows_num = 1){
      for(int i = 0; i < _table.size(); i++){
        _table.at(i).erase(_table.at(i).begin() + start_row , _table.at(i).begin() + start_row + rows_num);
      }
      _rows_num-= rows_num;
    }

    /**
      @brief entirely deletes colunms and frees thier memory while keeping order
      @param start_col the column to start deleting from
      @param cols_num how many columns to keep deleting after the start row
    */
    void eraseColumn(int start_col, int cols_num = 1){
      _table.erase(_table.begin() + start_col, _table.begin() + start_col + cols_num);
      _cols_num-= cols_num;
    }


    /**
      @brief refreshes the data Table for any mistake probability or bad handling
    */
    void refresh(){
      for(unsigned int i = 0; i < _cols_num; i++){
        if(_table.at(i).size() <= _rows_num){
          _table.at(i).resize(_rows_num);
        }
      }
    }

    /// @brief Extracts a row from the dataTable in a vector
    /// @param rowNumber the row number of the row you want to extract starting from 0
    /// @param returnArray the vector the row is returned in
    /// @return boolean for success of operation
    bool extractRow(unsigned int rowNumber, std::vector<HELD_DATA>& returnArray){
      if(rowNumber > this->_rows_num){
        return  false;
      }else{
        for(unsigned int idx = 0; idx < _cols_num; idx++){
          returnArray->push_back(getData(rowNumber, idx));
        }
        return true;
      }
    }
 
    /// @brief Extracts a row from the dataTable in a vector
    /// @param rowNumber the row number of the row you want to extract starting from 0
    /// @param returnArray the vector the row is returned in
    /// @return boolean for success of operation
    bool extractColumn(unsigned int columnNumber, std::vector<HELD_DATA>& returnArray){
      if(columnNumber > this->_cols_num){
        return false;
      }else{
        returnArray = _table.at(columnNumber);
      }
      return true;
    }

    /// @brief inserts a row in the table replacing existing if they exist or adds them
    /// @param rowNumber the position of the row to be added
    /// @param putArray the vector that holds the data to be put
    /// @return boolean for success of operation
    bool insertRow(unsigned int rowNumber,const std::vector<HELD_DATA>& putArray){
      for(unsigned int idx = 0; idx < putArray->size(); idx++){
        this->insertData(putArray->at(idx),  rowNumber ,  idx);    
      }
      return true;
    }

    /// @brief inserts a column in the table replacing existing if they exist or adds them
    /// @param rowNumber the position of the column to be added
    /// @param putArray the vector that holds the data to be put
    /// @return boolean for success of operation
    bool insertColumn(unsigned int columnNumber,const std::vector<HELD_DATA>& putArray){
      if(columnNumber >= this->_table.size()){
        this->_table.resize(columnNumber + 1);
        _cols_num = columnNumber + 1;
      }
      if(putArray.size() >= this->_rows_num){
        _rows_num = putArray.size();
      }
      _table.at(columnNumber) = putArray;
      return true;
    }
};



