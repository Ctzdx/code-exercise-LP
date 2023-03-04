#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <assert.h>
template<typename T>
class base_sub_symbol
{
public:
    base_sub_symbol()
    {
        last_timestap = -1;
        max_price = static_cast<T>(0);
        cur_sum = static_cast<T>(0);
        WeightedAveragePrice = static_cast<T>(0);
        volume = 0;
        max_time_interval = 0;
    };
    virtual void push_data(std::vector<std::string>& piece_data); // process every line 
    virtual std::string get_data();                               // return the rlt in a definite format
    
protected:
    int last_timestap;
    T max_price;
    int volume;
    T cur_sum;
    int max_time_interval;
    T WeightedAveragePrice;
};

template<typename T>
class exercise
{
public:
    exercise()
    {
        assert((std::is_same<T, int>::value || std::is_same<T, float>::value));
    };
    void parse_data(std::string& one_trade, int symbol_type); // parse each line and update sub symbol
    void re_sort();                                           // sort symbol in order
    void read_data(std::string& input_csv, int symbol_type);  // read csv
    void output_data(std::string& output_csv);                // output scsv
    void create_sub_symbol(std::shared_ptr<base_sub_symbol<T>>& this_symbool, int symbol_type); // symbol factory
private:
    /* data */
    std::vector<std::string>symbol_pools;  //store symbols and output in order
    std::unordered_map<std::string, std::shared_ptr<base_sub_symbol<T>>>all_data;
};


