make clean
make no-cache

./no-cache --QPnumber=1000000 --read_num=1000000 --write_num=1000000 --block_size=64 --cache_size=32768 --associativity=1 --trace_file=./trace/trace_1M.txt