if [ -d "../sim_build" ]
then
rm -r ../sim_build
fi
mkdir ../sim_build
cmake -S .. -B ../sim_build
cd ../sim_build || exit
make sim-graphics
echo "Target successfully built"
./apps/sim-graphics