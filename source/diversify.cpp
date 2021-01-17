#include ".\public\storedata.h"
#include ".\public\frontieralgo.h"
#include ".\public\risktopointalgo.h"

int main()
{
    DataFrame AlphaFrame = read_csv();
    //AlphaFrame.print();
    std::vector<std::string> input_etf_list = { "SPY", "VTV" }; //User input to determine which/how many ETFs would be analyzed. Variable length
    std::vector<CombNormal> bestpointset = find_optimal_points(AlphaFrame, input_etf_list); //ONLY WORKS WITH 2 ETFs RIGHT NOW 
    system("pause");
}