# Clas12root installation

#### Valerio, Dec. 16 2020

Before, read the instructions on the official repo https://github.com/JeffersonLab/clas12root.

Then, this works for me

``` bash
git clone --recurse-submodules https://github.com/jeffersonlab/clas12root.git
cd clas12root/
export CLAS12ROOT=$PWD
export PATH="$PATH":"$CLAS12ROOT/bin"
export HIPO=$CLAS12ROOT/hipo4
mkdir build
cd build
cmake ../
make
cmake ../
make install
```
Then set the CLAS12ROOT, PATH and HIPO env variables in the `.bashrc` file!
