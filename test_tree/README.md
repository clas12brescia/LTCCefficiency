# LTCCefficiency (tree)

**WORK IN PROGRESS**

This macro is the same as LTCCefficiency.cxx of the parent directory, 
but it also create and save a TTree with the useful variables to reproduce the 
histograms of Histogram_Vectors class.

Run with:
```bash
clas12root -b -q LTCCefficiency.cxx --in=input.dat
```
You can use `input_006616.dat` of the directory `test_hipowrite`, if 
you are working from ifarm.

Then, the tree can be found in `LTCCefficiency_tree.root`. Use:
```bash
root -l LTCCefficiency_tree.root
treeHisto->Print()
```
to see the structure of the tree. If you want to see the tree in 
TTreeViewer, instead of `Print()` use:
```bash
treeHisto->StartViewer()
```
## makeplot.cxx

The macro `makeplot.cxx` is a ROOT macro that draw and save some useful 
histograms. Run with:
```bash
root -l makeplot.cxx
```
Four canvas of three 1D-histograms are showed and saved in `out.root` 
file.

## makeHistos

The macro `makeHistos.cxx` is an upgrade of `makeplot.cxx` that draw and save some useful 
histograms, both 1D and 2D. Run with:
```bash
root -l makeHistos.cxx
```
Eight canvas of three 1D- and 2D-histograms are showed and saved in `out.root` and in `out.pdf` files.
