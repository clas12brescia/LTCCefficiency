# Clas12root installation

Before, read the instructions on the official repo https://github.com/JeffersonLab/clas12root.

This set of instructions worked for me on Jan 2022. 

``` bash
# Download clas12root source
git clone --recurse-submodules https://github.com/jeffersonlab/clas12root.git
# Clone the clasqaDB as well and set the QADB enviroment variable
git clone --recurse-submodules https://github.com/JeffersonLab/clasqaDB.git

cd clas12root/
export CLAS12ROOT=$PWD
export PATH="$PATH":"$CLAS12ROOT/bin"
export HIPO=$CLAS12ROOT/hipo4
export QADB=/home/valerio/CLAS_SOFTWARE/clasqaDB

mkdir build
cd build
cmake ../
# change 8 to the cores of your CPU
make -j8
make install
```

Optiona: set the CLAS12ROOT, PATH, HIPO and QADB env variables in the `.bashrc` file!


## Examples

Besides the examples on the [clas12root github repo](https://github.com/JeffersonLab/clas12root), a set of elementary clas12root macros is available in a dedicated branch:
```bash
git checkout --track origin/clas12root-examples # just once to activate the local tracking of the remote branch
git checkout clas12root-examples
cd example
```
