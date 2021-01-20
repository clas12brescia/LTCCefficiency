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

Then, the tree can be found in `LTCCefficiency_tree.root`:
```bash
root
root[1] new TBrowser
```
and then click on `LTCCefficiency_tree` icon. 
Then, click on the branches to see the histograms.

**WARNING**: 
If you want to see all branches in TTreeViewer, click right button of 
mouse on the `LTCCefficiency_tree` icon; then, click on _StartViewer_.
Due to an error in the code, a lot of branches will appear, after some 
seconds of loading.
 

