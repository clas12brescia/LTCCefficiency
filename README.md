# LTCCefficiency

Code to analyze the efficiency of the LTCC CLAS12 detector.

## What this macro does?

LTCCefficiency.cxx is a clas12root macro to calculate the efficiency of the LTCC CLAS12 detector for pions.
To do this, a list of hipo files' paths in a .dat or .txt file is required as input (by terminal).
For details on the reading method, see the following sections. 

## How does the macro work?

This macro reads a list of hipo files, draws and saves histograms of some distributions of pions' momentum with different cuts and filters.
The histograms of interest for this analysis are the following:

* **Missing Mass**: the peak is in the correspondence of the neutron mass (~0.94 GeV);
* **Momentum of pions in LTCC**: only the pions that entered in the geometrical region of the LTCC, in the active sectors (3 and 5) are selected;
* **Momentum of hits in LTCC**: only particles with a signal in LTCC (N<sub>phe</sub>>1) are selected;    
* **Efficiency**: estimation of the efficiency for identification of pions in LTCC. The estimation is calculated as the ratio of the previous two histograms.

Cuts on kinetic variables are applied to obtain a clear missing mass peak and to select the events the most accurately possible.
These cuts are:

* cut on missing mass (between 0.88 and 1 GeV), so that is near the neutron mass (~0.94 GeV);
* cut on the ratio between transverse momentum and transferred momentum on z (beam energy - p<sub>z</sub>): p<sub>t</sub>/q<sub>z</sub> < 0.12.

Pions in this macro are identified **without** the use of PID (=211).
Instead, they are selected using the following requests:

1. events with no more than one or two charged tracks are selected;
1. one of these tracks must be an electron;
1. the electron must not be in the active LTCC region (sectors 3 and 5);
1. no "heavy" particles (deuton, proton or heavier baryons) are admitted in the active region.

The particles used for this selection (e.g. electron, proton, deuton) are identified by their PID.

## How to run the macro

Run with:
```bash
clas12root LTCCefficiency.cxx --in=input_filename.dat   
```
where `input_filename.dat` contains the paths of the hipo files used for the analysis (see section *Note*).
This file can be created using the command:
```bash
ls -1 /directory/of/hipo/files/*.hipo > input_filename.dat
```
that put the paths of all hipo files present in the directory.


After the run finish, the histograms can be found in the root file `out_LTCCefficiency.root`.
To see the main histograms of the analysis, run the ROOT macro `viewHistos.cxx` with:

```bash
root -l viewHistos.cxx
```

As default, this macro looks for the file named `out_LTCCefficiency.root`. 
If changes on the output file name have been done, use:

```bash
root -l 'viewHistos.cxx("output_name.root")'
```

The ouput will be something like this:

![](./fig/out_LTCCefficiency.png)

This output is also saved in a pdf file with the same name.

Two extra histograms are present in this analysis. They are charges distribution of the candidated particles, before and after the missing mass cut.
Too draw these histograms, run:

```bash
root -l out_LTCCefficiency.root
root[1] histo9->Draw()
root[2] histo9b->Draw()
```

![](./fig/charge.png)

![](./fig/charge_cut.png)
 
## Note

All the figures were obtained using this macro on a file list of about 500 hipo files.
The hipo files used are from spring2019 run, directories 006616, 006695 and 006710. 
Path of the files:

`/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/006616`

`/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/006695`

`/cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/006710`

The complete list of this paths for all the file used in these macros can be found in the dat file `input_spring2019.dat`.

<!---
#### Miscell.
---
[Passwordless git access](https://github.com/clas12brescia/LTCCefficiency/blob/main/misc/passwordless-git.md)
---
[clas12root installation](https://github.com/clas12brescia/LTCCefficiency/blob/main/misc/clas12root_installation.md)
--->  
