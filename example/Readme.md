# Examples

Very very simple macros for clas12root

### simple.C

Open a file and print for each event the number of particles of that event
```bash
clas12root -b -q simple.C
```
### simple2.C

Same as simple.C without printing, save the number of particles distribution in a histogram into a `out_histos.root` file
```bash
clas12root -b -q simple2.C
```
Then the histogram can be found in the ROOT file:
```bash
root -l out_histos.root
root[1] multip->Draw()
```

![](./multip.png)
