#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "lib/xframe/include/xframe/xtensor/xarray.hpp"
#include "lib/xframe/include/xframe/xtensor/xbuilder.hpp"
#include "lib/xframe/include/xframe/xio.hpp"
#include "lib/xframe/include/xframe/xvariable.hpp"
#include "lib/xframe/include/xframe/xaxis_base.hpp"

#define CSV_FILE "ETF_data.csv"

float** vector_to_arr2d(std::vector<std::vector<float> >& vals, int N, int M)
{
    float** temp;
    temp = new float* [N];
    for (unsigned i = 0; (i < N); i++)
    {
        temp[i] = new float[M];
        for (unsigned j = 0; (j < M); j++)
        {
            temp[i][j] = vals[i][j];
        }
    }
    return temp;
}

void read_csv()
{
    using coordinate_type = xf::xcoordinate<xf::fstring>;
    using dimension_type = xf::xdimension<xf::fstring>;
    using variable_type = xf::xvariable<float, coordinate_type>;
    using data_type = variable_type::data_type;

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
    std::vector<std::string> dates;

    //Parse through the excel document row by row
    //getline(ip, line) exists here to take the first row
	std::string line;
	std::getline(ip, line);

    std::istringstream ss_date(line);
    std::string date_token;

    //Ignore first entry, which is "date"
    std::getline(ss_date, date_token, ',');
    while (getline(ss_date, date_token, ',')) {
        dates.push_back(date_token);
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

    //Store prices in data_type
    xt::xarray<float> prices_xarr = xt::zeros<float>({ prices.size(), prices[0].size() });
    for (int row = 0; row < prices.size(); row++) {
        for (int col = 0; col < prices[0].size(); col++) {
            prices_xarr(row, col) = prices[row][col];
        }
    }

    data_type d = prices_xarr;

    variable_type v(std::move(d),
        {
            {"ETF", xf::axis({1, 2, 3})},
            {"date",  xf::axis({"London", "Paris", "Brussels"})}
        });

    std::cout << "ROWS: " << prices.size() << "\n" << "COLS: " << prices[0].size() << "\n";
    std::cout << v;
}

int main()
{
    read_csv();
}
