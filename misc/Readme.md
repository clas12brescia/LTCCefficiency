# Miscellanea

## Plas12root installation

...

## Passwordless git

...

## Useful scripts

**get_hipo_info.sh**

Retrieve infos from `hipo-utils -info` and print them in a useful way. 

Usage (both a single file or multiple files with/without keywords can be used):
```bash
./get_hipo_info.sh <HIPOFILES>
```
Example with multiple files:

```bash
./get_hipo_info.sh /cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/006616/rec_clas_006616.evio.000*

rec_clas_006616.evio.00000-00004.hipo 803855 803855
rec_clas_006616.evio.00005-00009.hipo 805425 1609280
rec_clas_006616.evio.00010-00014.hipo 809773 2419053
rec_clas_006616.evio.00015-00019.hipo 809535 3228588
rec_clas_006616.evio.00020-00024.hipo 807907 4036495
rec_clas_006616.evio.00025-00029.hipo 808929 4845424
rec_clas_006616.evio.00030-00034.hipo 821563 5666987
rec_clas_006616.evio.00035-00038.hipo 548357 6215344
```
