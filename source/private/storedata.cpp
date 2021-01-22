#ifdef _WIN32
 #include "..\public\ymath.h"
 #include "..\public\date.h"
 #include "..\public\storedata.h"
#else
 #include "../public/ymath.h"
 #include "../public/date.h"
 #include "../public/storedata.h"
#endif
 

#define CSV_FILE "ETF_data.csv"

DataFrame read_csv()
{
    //Open ETF_data.csv and store file in "ip"
	std::ifstream ip(CSV_FILE);

    //If file fails to open, throw error
    if (!ip.is_open()) {
        std::cout << "ERROR: Failed to open CSV file \n";
        exit(EXIT_FAILURE);
    }

    //This is very poorly written right now
    //ETF tracks the name, single_price tracks the prices for a single ETF
    //Afterward, the ETF and single_price vector are stored in ETF_list and prices, respectively
	std::vector<std::string> ETF_list; //used in DataFrame
	std::string ETF;
	std::vector<std::vector<double> > prices;
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
    std::vector<std::vector<double> > invprice;
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
}
