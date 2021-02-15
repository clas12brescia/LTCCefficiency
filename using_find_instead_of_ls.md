# Using `find` instead of `ls`

If an *Argument list too long* error arises from an `ls` command, `find` can be used.

Example: a long file list production for the LTCC efficieny program is needed, but `ls` is not working:
```bash
ls -1 /cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/{006616..006783}/*.hipo > input_all_spring2019.dat
bash: /usr/bin/ls: Argument list too long
```
If no selection on the run number is needed, it can be replaced by a simple `find`:
```bash
find /cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/ -type f -name "*.hipo" > input_all_spring2019.dat
```
If some kind of selection on the run number is required, the command output can be piped into awk like this:
```bash
find /cache/clas12/rg-a/production/recon/spring2019/torus-1/pass1/v0/dst/recon/ -type f -name "*.hipo" | awk '{count=split($1,a,"/");run=a[count-1];if ((run>=6616)&&(run<=6783)) print}' > input_all_spring2019.dat
```