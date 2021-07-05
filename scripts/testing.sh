if [ -d "../test_build" ]
then
  rm -r ../test_build
fi
mkdir ../test_build
cmake -S .. -B ../test_build
cd ../test_build || exit
make seir.t
echo "target successfully built"
./tests/seir.t