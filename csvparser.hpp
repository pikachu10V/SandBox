#ifndef csvperaser_hpp
#define csvperaser_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <map>
#include <algorithm>


class CSVParser
{
private:
    std::string filename;
    std::string m_delimiter;
    int m_row = 0;
    int m_column = 0;
    struct ToDouble { double operator()(std::string x) { return ::atof(x.c_str()); } };
	
public:
    std::map<std::string, int> mp_columns;
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> arr_data;


    CSVParser(std::string m_fileName_, std::string m_delimiter_ = ",", int iloc_header=0) {
        filename = m_fileName_;
        m_delimiter = m_delimiter_;
        read_csv(iloc_header=iloc_header);
    }
    void read_csv(int iloc_header);
    void to_csv(std::string m_fileName_save, std::string sep = ",");
    std::vector<std::string> get_colvector(int iloc, int num_begin = 0); // get column vector 
    std::vector<std::string> get_colvector(std::string str_col, int num_begin = 0); // get column vector (from array)
	std::vector<double> get_colvector_asdouble(int iloc, int num_begin = 0); // get column vector as double
    std::vector<double> get_colvector_asdouble(std::string str_col, int num_begin = 0); // get column vector as double
};


int debug_csvparser();

#endif