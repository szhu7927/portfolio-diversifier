#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include ".\public\ymath.h"
#include ".\public\dataframe.h"
#include ".\public\date.h"
#include ".\public\datapt.h"
#include ".\public\frontieralgo.h"

#define CSV_FILE "ETF_data.csv"


DataFrame read_csv()
{
    //Open ETF_data.csv and store file in "ip"
	std::ifstream ip(CSV_FILE);

    //If file fails to open, throw error
    if (!ip.is_open()) exit(EXIT_FAILURE);

    //This is very poorly written right now
    //ETF tracks the name, single_price tracks the prices for a single ETF
    //Afterward, the ETF and single_price vector are stored in ETF_list and prices, respectively
	std::vector<std::string> ETF_list; //used in DataFrame
	std::string ETF;
	std::vector<std::vector<double>> prices;
	std::vector<double> single_price;

    //Parse through the excel document row by row
    //Get first line for dates
    std::string line;
    std::vector<std::string> datelist;
    std::getline(ip, line);
    std::istringstream date_ss(line);
    std::string date_token;

    //Skip first entry in row, which is the word "date".
    std::getline(date_ss, date_token, ',');
    while (getline(date_ss, date_token, ',')) {
        datelist.push_back(date_token);
    }

    //Converts each element in datelist into a Date object. This list of Date objects is used in DataFrame.
    std::vector<Date> dateobjlist;
    for (int i = 0;  i < datelist.size(); ++i) {
        dateobjlist.push_back(Date(datelist[i]));
    }

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
            single_price.push_back((double)atof(token.c_str()));
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

    //Swap rows and columns in 2D price vector, so that invprice[i][j] refers to the ith date and the jth ETF. invprice is used in DataFrame.
    std::vector<std::vector<double>> invprice;
    std::vector<double> vectorbuilder;

    for (int a = 0; a < prices[0].size(); ++a) {
        for (int b = 0; b < prices.size(); ++b) {
            vectorbuilder.push_back(prices[b][a]);
        }
        invprice.push_back(vectorbuilder);
        vectorbuilder.clear();
    }

    //Stores in a DataFrame object
    DataFrame AlphaFrame(dateobjlist, ETF_list, invprice);
    return AlphaFrame;

    //Printing values:
    /*
    Normal SPY = Normal(percgrowth(prices[0]));
    Normal IWF = Normal(percgrowth(prices[1]));
    CombNormal SPYIWF1 = CombNormal(SPY, IWF, 0.1);
    CombNormal SPYIWF2 = CombNormal(SPY, IWF, 0.2);
    CombNormal SPYIWF3 = CombNormal(SPY, IWF, 0.3);
    CombNormal SPYIWF4 = CombNormal(SPY, IWF, 0.4);
    CombNormal SPYIWF5 = CombNormal(SPY, IWF, 0.5);
    CombNormal SPYIWF6 = CombNormal(SPY, IWF, 0.6);
    CombNormal SPYIWF7 = CombNormal(SPY, IWF, 0.7);
    CombNormal SPYIWF8 = CombNormal(SPY, IWF, 0.8);
    CombNormal SPYIWF9 = CombNormal(SPY, IWF, 0.9);

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
        "90/10 SPY/IWF Daily Percent Growth Standard Deviation: " << SPYIWF9.stdev << "\n";
        */
}

int main()
{
    DataFrame AlphaFrame = read_csv();
    std::vector<std::string> input_etf_list = { "SPY", "IWF" }; //User input to determine which/how many ETFs would be analyzed. Variable length
    std::vector<DataPT> bestpointset = find_optimal_points(AlphaFrame, input_etf_list);
    std::cout << sizeof(DataPT);
    system("pause");
}
