#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "ymath.cpp"

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

		//std::cout << "ETF: " << ETF_list[index] << '\n';
		//std::cout << "First Price: " << prices[index][0] << '\n';

        single_price.clear();
        index++;
    }

    //Close the file
    ip.close();

    normal SPY = normal(percgrowth(prices[0]));
    normal IWF = normal(percgrowth(prices[1]));
    comb_normal SPYIWF1 = comb_normal(SPY, IWF, 0.1);
    comb_normal SPYIWF2 = comb_normal(SPY, IWF, 0.2);
    comb_normal SPYIWF3 = comb_normal(SPY, IWF, 0.3);
    comb_normal SPYIWF4 = comb_normal(SPY, IWF, 0.4);
    comb_normal SPYIWF5 = comb_normal(SPY, IWF, 0.5);
    comb_normal SPYIWF6 = comb_normal(SPY, IWF, 0.6);
    comb_normal SPYIWF7 = comb_normal(SPY, IWF, 0.7);
    comb_normal SPYIWF8 = comb_normal(SPY, IWF, 0.8);
    comb_normal SPYIWF9 = comb_normal(SPY, IWF, 0.9);

    std::cout << "SPY Daily Percent Growth Mean: " << SPY.mean << "\n" <<
        "SPY Daily Percent Growth Standard Deviation: " << SPY.stdev << "\n" << "\n" <<
        "IWF Daily Percent Growth Mean: " << IWF.mean << "\n" <<
        "IWF Daily Percent Growth Standard Deviation: " << IWF.stdev << "\n" << "\n" <<
        "SPY/IWF covariance: " << covar(SPY.vec, IWF.vec) << "\n" << "\n" <<
        "10/90 SPY/IWF Daily Percent Growth Mean: " << SPYIWF1.mean << "\n" <<
        "10/90 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF1.stdev << "\n" << "\n" <<
        "20/80 SPY/IWF Daily Percent Growth Mean: " << SPYIWF2.mean << "\n" <<
        "20/80 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF2.stdev << "\n" << "\n" <<
        "30/70 SPY/IWF Daily Percent Growth Mean: " << SPYIWF3.mean << "\n" <<
        "30/70 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF3.stdev << "\n" << "\n" <<
        "40/60 SPY/IWF Daily Percent Growth Mean: " << SPYIWF4.mean << "\n" <<
        "40/60 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF4.stdev << "\n" << "\n" <<
        "50/50 SPY/IWF Daily Percent Growth Mean: " << SPYIWF5.mean << "\n" <<
        "50/50 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF5.stdev << "\n" << "\n" <<
        "60/40 SPY/IWF Daily Percent Growth Mean: " << SPYIWF6.mean << "\n" <<
        "60/40 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF6.stdev << "\n" << "\n" <<
        "70/30 SPY/IWF Daily Percent Growth Mean: " << SPYIWF7.mean << "\n" <<
        "70/30 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF7.stdev << "\n" << "\n" <<
        "80/20 SPY/IWF Daily Percent Growth Mean: " << SPYIWF8.mean << "\n" <<
        "80/20 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF8.stdev << "\n" << "\n" <<
        "90/10 SPY/IWF Daily Percent Growth Mean: " << SPYIWF9.mean << "\n" <<
        "90/10 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF9.stdev;
}

int main()
{
    read_csv();
}
