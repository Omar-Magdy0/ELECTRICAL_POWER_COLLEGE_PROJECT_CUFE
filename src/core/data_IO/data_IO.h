#pragma once

#include "dataTable.h"
#include <fstream>
#include <string>

/*
  THIS FILE IS FOR IMPORTING OR EXPORTING DIFFERENT FILE FORMATS INTO THE APPLICATION AND WRAPPING The DATA with the dataTable class
  mainly revolves around   (data_import) and (data_export) functions
*/




enum dataType{
  csv,
  pdf,
  // and any file file formats you can think of that have function assosiated and can handle
};

struct file_IO{
/**
  @brief csv File I/O functions 
*/
private:
bool csv_import(string file_address, dataTable<double> *table);
bool csv_export(string file_address, dataTable<double> table);

/**
  @brief pdf File I/O functions 
*/
bool pdf_import(string file_address, dataTable<double> *table);
bool pdf_export(string file_address, dataTable<double> table);
public:

/**
  @brief a function that imports files with supported file formats
  @param file_address as a string containing the path to the file and the file name
  @param data modifies the dataTable instance to hold the data from the file
  @param type file extension
  @return true if operation is successfull false if anything else happens
*/
bool data_import(string file_address,  dataTable<double> *data,  int type);
bool data_export(string file_address, dataTable<double> data, int type);
};