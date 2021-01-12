# LTCCefficiency

Code to analyze the efficiency of the LTCC CLAS12 detector.

## test7

The code is divided in two different macros:

* **test7_pion.cxx**: pions are identified using the Particle ID (PID=211).
* **test7_charged.cxx**: pions are identified using an algorithm that take events with two charged tracks, one electron and another particle, and the electron is taken outside the active sectors of LTCC (sectors 3 and 5). Other constraints are applied.

A method to read hipo files and add them to the HipoChain of clas12root is provided in the file `readfiles1.cxx`.
At the present, only a text file reader for list of hipo file is implemented.

More details can be found in the following sections.

### test7_pion

This macro reads a list of hipo files, draws and saves histograms of some distributions of pions' momentum with different constraints.
These distributions are the following:

* **Missing Mass**: the peak is in the correspondence of the neutron mass;
* **Momentum of pions in LTCC**: only the pions that entered in the geometrical region of the LTCC, in the active sectors (3 and 5) are selected;
* **Momentum of hits in LTCC**: only particles with a signal in LTCC (N<sub>phe</sub>>1) are selected.    
* **Efficiency**: estimation of the efficiency for identification of pions in LTCC. The estimation is calculated as the ratio of the previous two histograms.

Other constraints are applied to obtain a clear missing mass peak and to select the events more accurately.
These cuts are:

* cut on missing mass (between 0.88 and 1 GeV), so that is near the neutron mass (~0.94 GeV);
* cut on the ratio between transverse momentum and transferred momentum on z (beam energy - p<sub>z</sub>): p<sub>t</sub>/q<sub>z</sub> < 0.12;
* exclude electrons in sectors 3 and 5 and "heavy" particles (proton, neutron, deuton) using PID identification.

Run with:
```bash
clas12root test7_pion.cxx --in=input_filename.dat   
```
Then, the histograms can be found in the root file `out_pion.root`:
```bash
root -l out_pion.root
root[1] histo0->Draw()
```

![Missing mass](LTCCefficiency/fig/missingmass_pion.png)


```bash
root[2] histo1->Draw()
```

![Pions candidates in the LTCC region](./fig/candidates_ltcc_pion.png)

```bash
root[3] histo2->Draw()
```

![Candidates with hits in the LTCC active region](./fig/candidates_hits_pion.png)

```bash
root[4] hratio->Draw()
```

![Efficiency of LTCC for pions (ratio between histo2 and histo1)](./fig/pi_ratio_pion.png)

### test7_charged

Same of the previous macro, but the selection of the pions is made without the use of PID.
Pions are identified in the following way:

* Two charged track events are selected, which one is the electron;
* The electron must not be in the active sectors of LTCC (sectors 3 and 5);
* The same constraints and cuts of the previous macro.

Two more histograms are showed and saved:
* **Charge of candidates**
* **Charge of candidates after the cut on missing mass**

Run with:
```bash
clas12root test7_charged.cxx --in=input_filename.dat
```
Then, the histograms can be found in the root file `out_charged.root`:
```bash
root -l out_charged.root
root[1] histo0->Draw()
```

![Missing mass](./fig/missingmass_charged.png)

```bash
root[2] histo1->Draw()
```

![Pions candidates in the LTCC region](./fig/candidates_ltcc_charged.png)

```bash
root[3] histo2->Draw()
```

![Candidates hits in the LTCC active region](./fig/candidates_hits_charged.png)

```bash
root[4] hratio->Draw()
```

![Efficiency of LTCC for pions (ratio between histo2 and histo1](./fig/pi_ratio_charged.png)

```bash
root[5] histo9->Draw()
```

![Charge of selected particle (two charged track)](./fig/charge.png)

```bash
root[6] histo9b->Draw()
```

![Charge of selected particle after missing mass cut](./fig/charge_cut.png)

### viewHistos

This ROOT macro provides a rapid method to view and save all the histograms produced by the test7 macros.

Run with:

```bash
root -l viewHistos.cxx
```

With this syntax, the pre-defined output are the histograms from `out_charged.root`. One can also specify which root file to use:

```bash
root -l 'viewHistos.cxx("out_pion.root")'
```

## Note

All the figures were obtained using these macros on a file list of about 500 hipo files.
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
