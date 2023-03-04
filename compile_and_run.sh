cd code
g++ -O2 --std=c++14 process.cpp -o process 
mv process ../
cd ../
# executable_file input_csv_path output_csv_path data_class_type
./process data/csv_test.csv data/output.csv 0
