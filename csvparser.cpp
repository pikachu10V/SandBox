#include "csvparser.hpp"



void CSVParser::read_csv(int iloc_header)
{
	std::vector<std::string> vec_data;
	std::ifstream input_file(this->filename);
	if (!input_file.is_open()) {
		std::cout << "Could not open the file - '"<< filename << "'" << std::endl;
		exit;
	}

	std::ifstream file(this->filename);
	std::string line;
	int header_count = 0;
	while (std::getline(file, line))
	{
		vec_data.clear();
		std::stringstream lineStream(line);
		std::string cell;

		while (std::getline(lineStream, cell, ','))
		{
			vec_data.push_back(cell);
		}
		this->arr_data.push_back(vec_data);
	}

	// columns
	this->columns = this->arr_data[iloc_header];
	for (int i = 0; i < this->columns.size(); i++) {
		this->mp_columns[this->columns[i]] = i;
	}
	if (this->mp_columns.size()!= this->columns.size()) {
		std::cout << "Warnings: There are duplicate columns" << std::endl;
	}

	// data
	this->arr_data = { this->arr_data.begin() + iloc_header + 1, this->arr_data.end()};

	// Store the number of rows and columns
	this->m_row = this->arr_data.size();
	this->m_column = this->arr_data[0].size();
}


void CSVParser::to_csv(std::string m_fileName_save, std::string sep)
{
	std::ofstream output_file(m_fileName_save);
	if (!output_file.is_open()) {
		std::cout << "Could not open the file - '" << m_fileName_save << "'" << std::endl;
		exit;
	}

	for (int j = 0; j < this->m_column; j++) {
		output_file << this->columns[j] << ",";
	}
	
	for (int i = 0; i < this->m_row; i++) {
		for (int j = 0; j < this->m_column; j++) {
			output_file << this->arr_data[i][j] << ",";
		}
		output_file << std::endl;
	}
	output_file << std::endl;
	
}


std::vector<std::string> CSVParser::get_colvector(int num_col, int num_begin)
{
	/*
	num_col: target column number
	num_begin: skiprows (default value is 1)
	*/
	std::vector<std::string> vec_data_col;
	for (int i = 0; i < this->m_row; i++) {
		vec_data_col.push_back(this->arr_data[i][num_col]);
	}

	vec_data_col = { vec_data_col.begin() + num_begin, vec_data_col.end() };

	return vec_data_col;
}


std::vector<std::string> CSVParser::get_colvector(std::string str_col, int num_begin)
{
	int num_col = this->mp_columns[str_col];
	return this->get_colvector(num_col, num_begin);
}


std::vector<double> CSVParser::get_colvector_asdouble(int num_col, int num_begin)
{
	std::vector<std::string> x = this->get_colvector(num_col, num_begin);
	std::vector<double> y(x.size());

	std::transform(x.begin(), x.end(), y.begin(), ToDouble());
	return y;
}


std::vector<double> CSVParser::get_colvector_asdouble(std::string str_col, int num_begin)
{
	int num_col = this->mp_columns[str_col];
	return this->get_colvector_asdouble(num_col, num_begin);
}




int debug_csvparser()
{
	
	std::string filename = "csv\\JPYOIS_220530.csv";
	std::string filename_save = "csv\\JPYOIS_220530_s.csv";

	CSVParser parser(filename);
	parser.read_csv(0);
	parser.to_csv(filename_save);
	std::vector<std::string> hoge = parser.get_colvector(3);
	std::vector<std::string> hoge2 = parser.get_colvector("MID");
	std::vector<double> hoge_double = parser.get_colvector_asdouble(3);
	std::vector<double> hoge2_double = parser.get_colvector_asdouble("MID");

	return 0;
}

