#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void read_csv()
{
    //Open ETF_data.csv and store file in "ip"
    ifstream ip("ETF_data.csv");
    
    //If file fails to open, throw error
    if (!ip.is_open()) exit(EXIT_FAILURE);

    //This is very poorly written right now
    //ETF tracks the name, single_price tracks the prices for a single ETF
    //Afterward, the ETF and single_price vector are stored in ETF_list and prices, respectively
    vector<string> ETF_list;
    string ETF;
    vector<vector<float>> prices;
    vector<float> single_price;

    //Parse through the excel document row by row
    //getline(ip, line) exists here to skip the first row
    string line;
    getline(ip, line);

    //Keeps track of the index to print
    int index = 0;

    //While data still exists, parse through each row
    while (getline(ip, line)) {
        //Tokenizes a single row into each data cell
        istringstream ss(line);
        string token;

        //Get the first entry in the row, which is the name of the ETF
        //Store the value in ETF
        getline(ss, ETF, ',');
        ETF_list.push_back(ETF);

        //Get the rest of the data in the row, which contains the stock prices
        while (getline(ss, token, ',')) {
            single_price.push_back((float)atof(token.c_str()));
        }

        //Store single_price in prices
        prices.push_back(single_price);

        cout << "ETF: " << ETF_list[0] << '\n';
        cout << "First Price: " << prices[index][0] << '\n';

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