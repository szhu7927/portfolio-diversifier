#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

struct hash_map_collection {
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> date_map;
    std::unordered_map<std::string, std::vector<float>> index_arr_map;
};

hash_map_collection read_csv(std::string csv_file)
{
    //Open ETF_data.csv and store file in "ip"
	std::ifstream ip(csv_file);
    
    //If file fails to open, throw error
    if (!ip.is_open()) exit(EXIT_FAILURE);

    //Hashmap
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> date_map;
    std::unordered_map<std::string, std::vector<float>> index_arr_map;
    
    //Stores the first row in csv in dates
    std::vector<std::string> dates;
    std::string ETF;
    std::vector<float> prices;

    //Parse through the excel document row by row
    //Get first line for dates
	std::string line;
	std::getline(ip, line);
    
    std::istringstream date_ss(line);
    std::string date_token;

    //Skip first entry, which is "date"
    std::getline(date_ss, date_token, ',');
    while (getline(date_ss, date_token, ',')) {
        dates.push_back(date_token);
    }

    //While data still exists, parse through each row
    while (std::getline(ip, line)) {
        int col = 0;

        //Tokenizes a single row into each data cell
		std::istringstream ss(line);
		std::string token;

        //Get the first entry in the row, which is the name of the ETF
        //Store the value in ETF
		std::getline(ss, ETF, ',');

        //Get the rest of the data in the row, which contains the stock prices
        while (getline(ss, token, ',')) {
            std::pair<std::string, std::string> pdate(ETF, dates[col]);
            date_map[pdate] = (float)atof(token.c_str());

            prices.push_back((float)atof(token.c_str()));

            col++;
        }

        index_arr_map[ETF] = prices;
        prices.clear();

        col = 0;
    }

    //Close the file
    ip.close();

    hash_map_collection h;
    h.date_map = date_map;
    h.index_arr_map = index_arr_map;

    return h;
}

float get_price(std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> date_map, std::string name, std::string date)
{
    std::pair<std::string, std::string> p(name, date);
    return date_map[p];
}

int main()
{
    hash_map_collection data = read_csv("ETF_data.csv");
    
    //date_map is a hash map that takes in 2 key values (ETF name and date) and returns 1 value (price)
    //We can use the get_price function to get any price given a name and date
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> date_map = data.date_map;

    //index_arr_map is a hash map that takes in 1 key value (ETF name) and returns 1 value (vector of prices)
    std::unordered_map<std::string, std::vector<float>> index_arr_map = data.index_arr_map;

    std::string ETF = "VTV";
    std::string date = "2017-12-21";
    int index = 3;

    std::vector<float> ETF_arr = index_arr_map[ETF];

    std::cout << "ETF: " << ETF << "\n" << "Date: " << date << "\n" << "Price: " << get_price(date_map, ETF, date) << "\n";

    std::cout << "\n";

    std::cout << "ETF: " << ETF << "\n" << "Index: " << index << "\n" << "Price: " << ETF_arr[index] << "\n";
}
