# code-exercise-LP
code exercise

enviroment: linux

compile codes: 

```shell
g++ -O2 --std=c++14 process.cpp -o process 
```

meaning: executable file input_csv_path output_csv_path data_class_type
test command:

```shell
 ./process ../data/csv_test.csv ../data/output.csv 0
```

you can alse run the auto-test script directly as follow：

```shell
sh compile_and_run.sh
```

spend time: 1.8h