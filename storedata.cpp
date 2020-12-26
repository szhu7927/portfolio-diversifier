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
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> datemap;
    std::unordered_map<std::pair<std::string, int>, float, hash_pair> indexmap;
};

hash_map_collection read_csv(std::string csv_file)
{
    //Open ETF_data.csv and store file in "ip"
	std::ifstream ip(csv_file);
    
    //If file fails to open, throw error
    if (!ip.is_open()) exit(EXIT_FAILURE);

    //Hashmap
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> datemap;
    std::unordered_map<std::pair<std::string, int>, float, hash_pair> indexmap;
    
    //Stores the first row in csv in dates
    std::vector<std::string> dates;

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
        std::string ETF;
        float price;
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
            datemap[pdate] = (float)atof(token.c_str());

            std::pair<std::string, int> pindex(ETF, col);
            indexmap[pindex] = (float)atof(token.c_str());

            col++;
        }

        col = 0;
    }

    //Close the file
    ip.close();

    hash_map_collection h;
    h.datemap = datemap;
    h.indexmap = indexmap;

    return h;
}

int main()
{
    hash_map_collection data = read_csv("ETF_data.csv");
    std::unordered_map<std::pair<std::string, std::string>, float, hash_pair> datemap = data.datemap;
    std::unordered_map<std::pair<std::string, int>, float, hash_pair> indexmap = data.indexmap;

    std::string ETF = "VTV";
    std::string date = "2017-12-21";
    int index = 3;

    std::pair<std::string, std::string> ex1(ETF, date);
    std::cout << "ETF: " << ETF << "\n" << "Date: " << date << "\n" << "Price: " << data.datemap[ex1] << "\n";

    std::cout << "\n";

    std::pair<std::string, int> ex2(ETF, index);
    std::cout << "ETF: " << ETF << "\n" << "Index: " << index << "\n" << "Price: " << data.indexmap[ex2] << "\n";
}
