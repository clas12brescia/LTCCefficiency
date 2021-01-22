# Write a Hipo and make a TTree

This macros performs different but linked features:
* **customHipo.cxx**: write a hipo file from a group of hipo files with some cuts and filters;
* **LTCCefficiency.cxx**: create a tree from a list of hipo files or a customized hipo file.

For details on working of the macros, see the next sections.

## customHipo.cxx

This clas12root macro creat a new hipo file from a list of hipo files. This list must be in a .dat or .txt format, containing all the files paths.
Running `customHipo.cxx`, a selection based on some kinetic cuts and particles filters is done. 
The resulting hipo file is a smaller file with the selected data.

Run with:
```bash
time clas12root -b -q customHipo.cxx --in=input_filename.dat
```
where `input_filename.dat` is the list of paths of the hipo files. 
**WARNING**: the code is designed to receive as input a file that begin with `input_`. 
In that way, the new hipo file will be named correctly. 

A new hipo file named `custom_filename.hipo` is created.
Then, you can run the other macro to make the TTree. 

## LTCCefficiency.root

Run with:
```bash
time clas12root -b -q LTCCefficiency.cxx --in=custom_filename.hipo
or 
time clas12root -b -q LTCCefficiency.cxx --in=input_filename.dat
```
Then, the TTree can be found in the ROOT file `LTCCefficiency_tree_filename.root`. 
The user can access this tree (named `treeHisto`) using:
```bash
root -l LTCCefficiency_tree_filename.root
root[1] treeHisto->Command("options")
```
For example:
```bash
root[1] treeHisto->Print()						//Print the branches of the TTree (variables "Var") and the entries
root[2] treeHisto->Draw("Var")				//Draw the 1D-histogram of the variable "Var"
root[3] treeHisto->Draw("Var2:Var1")	//Draw the 2D-histogram of the variables "Var1" (x) and "Var2" (y)
```
For other details on TTree, see the[TTree class reference](https://root.cern.ch/doc/master/classTTree.html).

## Comments

Some question to be answered:
1. In this specific case, is it helpful?
1. How can we run the customHipo.cxx on ifarm efficiently?
1. Is it better to be more or less selective (= more or less rigid cuts) in the customization of hipo files?
