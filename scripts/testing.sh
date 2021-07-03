mkdir ../test_build
cmake -S ../ -B ../test_build 
cd ../test_build
make seir.t
echo "target successfully built"
./tests/seir.t
