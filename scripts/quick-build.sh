if [ ! -d "../sim_build" ]
then
mkdir ../sim_build
fi
cd ../sim_build || exit
cmake .. -DCMAKE_BUILD_TYPE=Release
make
echo "All built targets are in sim_build directory"
echo "(use cd ../sim_build to checkout)"