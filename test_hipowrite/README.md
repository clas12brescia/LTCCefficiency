# test_hipowrite

This folder is for test with clas12writer method.

## simple_write.cxx

This clas12root macro select particles of events with at least 1 electron
with momentum between 9.5 and 9.505 GeV/c. 
These events are written in a new hipo file named `simple.hipo`.
During the run, each time the selection request is satisfied, a 
`+` is printed on terminal.

Run with:
```bash
clas12root simple_write.cxx --in=input_file.dat
```

where `input_file.dat` is a list of hipo files paths, created with commands:

```bash
ls -1 /dir/to/hipo/files/*.hipo > input_file.dat
```

After the run, a hipo file `simple.hipo` can be found in the same 
directory where the macro is executed.
Using the `view.cxx` macro, one can verify if the hipo file was written 
correctly. Run with:
```bash
clas12root view.cxx
```
and a 1D-histogram is showed.

### Notes
1.  A HipoChain is used in this case. Using TChain, the clas12writer 
does not recognize the chain.
1. A readfiles_mod method for reading input is implemented. This 
method can read both .dat/.txt list of hipo files and .hipo files 
directly. So, one can in principle execute:
```bash
clas12root simple_write.cxx --in=file.hipo
```
to obtain a hipo file with selected events from a single hipo file.
