#include <fstream>
#include <sstream>
#include "process.h"
#include <algorithm>

//parse each line
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
};
template<typename T>
void exercise<T>::parse_data(std::string& one_trade, int symbol_type)
{
    auto rlt = split(one_trade, ",");
    //delete  '\r' 
    if(rlt.back().back() == '\r')
    {
        rlt.back() = rlt.back().substr(0, rlt.back().length() - 1);
    }
    if(all_data.find(rlt[1]) == all_data.end())
    {
        std::shared_ptr<base_sub_symbol<T>> tmp_symbol;
        create_sub_symbol(tmp_symbol, symbol_type);
        all_data[rlt[1]] = tmp_symbol;
        symbol_pools.push_back(rlt[1]);
    }
    all_data[rlt[1]]->push_data(rlt);
}
template<typename T>
void exercise<T>::re_sort()
{
    sort(symbol_pools.begin(), symbol_pools.end(), [](const std::string& symbol1, const std::string& symbol2){
        for(auto i = 0; i < symbol1.size(); i++)
        {
            if(static_cast<int>(symbol1[i]) == static_cast<int>(symbol2[i]))
            {
                continue;
            }
            else if(static_cast<int>(symbol1[i]) < static_cast<int>(symbol2[i]))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return true;
    });
}
template<typename T>
void exercise<T>::read_data(std::string& input_csv, int symbol_type)
{
    std::ifstream fp(input_csv); 
    std::string line;
    while (getline(fp,line)){ 
        parse_data(line, symbol_type);
    }
}
template<typename T>
void exercise<T>::output_data(std::string& output_csv)
{
    
    re_sort();
    std::ofstream file(output_csv);
    if (file)
    {
        for(auto& symbol:symbol_pools)
        {
            if(all_data.find(symbol) == all_data.end())
            {
                continue;
            }
            auto sub_symbol = all_data[symbol];
            auto single_rlt = symbol + sub_symbol->get_data();
            // <symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice>
            file << single_rlt << "\n";
        } 
    }
    file.close();

}
template<typename T>
void exercise<T>::create_sub_symbol(std::shared_ptr<base_sub_symbol<T>>& this_symbool, int symbol_type)
{
    if(symbol_type == 0)
    {
        this_symbool = std::shared_ptr<base_sub_symbol<T>>(new base_sub_symbol<T>());
    }
    //TODO:
}
template<typename T>
void base_sub_symbol<T>::push_data(std::vector<std::string>& piece_data)
{
    // <TimeStamp>,<Symbol>,<Quantity>,<Price>
    T cur_price;
    if(std::is_same<T, int>::value)
    {
        cur_price =  stoi(piece_data[3]);
    }
    else
    {
        cur_price =  stof(piece_data[3]);
    }
    max_price = std::max(max_price, cur_price);
    if(last_timestap != -1)
    {
        max_time_interval = std::max(max_time_interval, stoi(piece_data[0]) - last_timestap);
    }
    
    last_timestap = stoi(piece_data[0]);
    cur_sum += stoi(piece_data[2]) *  cur_price;
    volume += stoi(piece_data[2]);
    WeightedAveragePrice = cur_sum / static_cast<T>(volume);
}
template<typename T>
std::string base_sub_symbol<T>::get_data()
{
    std::string rlt = "";
    rlt += "," + std::to_string(max_time_interval);
    rlt += "," + std::to_string(volume);
    rlt += "," + std::to_string(WeightedAveragePrice);
    rlt += "," + std::to_string(max_price);
    return rlt;
}

int main(int argv, char* args[]){
    assert(argv == 4 && "please input as ./executable_file input_csv_path output_csv_path symbol_type");
    std::string input_csv = args[1];
    std::string output_csv = args[2];
    auto symbol_type = atoi(args[3]);

    exercise<int> main_data_pool;
    main_data_pool.read_data(input_csv,  symbol_type);
    main_data_pool.output_data(output_csv);
    printf("parse data over\n");
    return 0;
}

// compile codes: g++ -O2 --std=c++14 process.cpp -o process 
// test command: ./process csv_test.csv output.csv 0

