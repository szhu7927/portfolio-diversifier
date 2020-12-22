#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define CSV_FILE "ETF_data.csv"


void read_csv()
{
    //Open ETF_data.csv and store file in "ip"
	std::ifstream ip(CSV_FILE);
    
    //If file fails to open, throw error
    if (!ip.is_open()) exit(EXIT_FAILURE);

    //This is very poorly written right now
    //ETF tracks the name, single_price tracks the prices for a single ETF
    //Afterward, the ETF and single_price vector are stored in ETF_list and prices, respectively
	std::vector<std::string> ETF_list;
	std::string ETF;
	std::vector<std::vector<float> > prices;
	std::vector<float> single_price;

    //Parse through the excel document row by row
    //getline(ip, line) exists here to skip the first row
	std::string line;
	std::getline(ip, line);

    //Keeps track of the index to print
    int index = 0;

    //While data still exists, parse through each row
    while (std::getline(ip, line)) {
        //Tokenizes a single row into each data cell
		std::istringstream ss(line);
		std::string token;

        //Get the first entry in the row, which is the name of the ETF
        //Store the value in ETF
		std::getline(ss, ETF, ',');
        ETF_list.push_back(ETF);

        //Get the rest of the data in the row, which contains the stock prices
        while (getline(ss, token, ',')) {
            single_price.push_back((float)atof(token.c_str()));
        }

        //Store single_price in prices
        prices.push_back(single_price);

		std::cout << "ETF: " << ETF_list[index] << '\n';
		std::cout << "First Price: " << prices[index][0] << '\n';

        single_price.clear();
        index++;
    }

    //Close the file
    ip.close();
}

int main()
{
    read_csv();
}
