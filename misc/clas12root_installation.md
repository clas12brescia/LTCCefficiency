# Clas12root installation

#### Valerio, Dec. 16 2020

Before, read the instructions on the official repo https://github.com/JeffersonLab/clas12root.

Then, this works for me (copied from the install script in the repo but removing the first 2 lines about hipo files copying)

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


## Examples

Besides the examples on the [clas12root github repo](https://github.com/JeffersonLab/clas12root), a set of elementary clas12root macros is available in a dedicated branch:
```bash
git checkout clas12root-examples
cd example
```
