## why the newest gem5 + alpha ?

It is the only version I found can run double "hello world"...
So **gem5 in this repo is unsable!!**

## where to get the newest gem5 ?
github

## where to get spec ?
Copy it from 172.18.11.212:/home/

## where to get alpha tool chain ?

- From m5sim: www.m5sim.org/dist/current/alphaev67-unknown-linux-gnu.tar.bz2
- extract it to certain place


## how to compile spec06 ?

- Copy alpha.cfg from 172.18.11.212:/home/zhouyaoyang/spec/cpu2006/config/alpha.cfg to your correpsponding directory
- delete content below __MD5__
- modify CC, CXX and FC: replace /home/zhouyaoyang/alpha\_tool\_chain/m5/ with your tool chain directory
- source shrc in cpu2006/
- runspec --config=alpha.cfg --action=build --tune=base int

(I have not found information about how to compile it in parallel.)
Then you will get a list about compilation success and error.

## how to set benchmarks as workloads for gem5 ?

Now, you can get configured gem5 from
git@http://10.30.7.141:pard/pard\_smt\_gem5.git;

The branch "pard\_smt\_gem5" is our default branch, containing newest scripts.

**You did not need to** modify environment variables in scripts anymore.

Just go to your gem5 directory, compile and run
```
./run_gem5_alpha_spec06_benchmark.sh perlbench $output_dir
```
or two workloads:
```
./run_gem5_alpha_spec06_benchmark.sh "perlbench;gcc" $output_dir --smt
```

Note that $output\_dir should be an **absolute path**, not a relative one.
Otherwise, you have to go to exe directory to find you results.

Double benchmarks form one binary is supported:
like
```
./run_gem5_alpha_spec06_benchmark.sh "perlbench;perlbench" ~/tmp/perlbench2/ --smt
```

## Tested benchmarks:
These benchmarks can run on gem5 (without smt), and get expected output:

- perlbench
- hmmer
- libquantum
- sjeng
- gobmk (too slow, only got a part of output)
- mcf (too slow, only got a part of output)
- omnetpp (too slow, only got a part of output)
- h264ref (too slow, only got a part of output)
- specrand\_i

## Issue:
Some benchmarks use relative path as inputs, like gobmk.
Inputs for gobmk are many relative paths point to data in ".../data/all",
which is not accessible if we stay in GEM5\_DIR.
For a single benchmark, we can change current directory to its "all"
directory to solve it.
However, if multiple benchmarks are set as workloads, condition is much more
complex. I proposed an ugly solution, and the following takes gobmk as an example:

```
cd $GEM5_DIR    # go to your gem5 directory
mkdir spec_run  # which is ignored by latest .gitignore
cd spec_run
cp ~/cpu2006/benchspec/CPU2006/445.gobmk/data/all/input/* ./ -r
# copy data to spec_run
../run_gem5_alpha_spec06_benchmark.sh "perlbench;gobmk" ~/tmp/perlbench_gobmk/ --smt
# Because demanded input files reside in current directory, it works.

```

Except two directories above, these files are suggested to be linked to "spec\_run":

- foreman\_qcif.yuv -> /home/zhouyaoyang/cpu2006/benchspec/CPU2006/464.h264ref/data/all/input/foreman_qcif.yuv
- lake.bin -> /home/zhouyaoyang/cpu2006/benchspec/CPU2006/473.astar/data/test/input/lake.bin
- omnetpp.ini -> /home/zhouyaoyang/cpu2006/benchspec/CPU2006/471.omnetpp/data/test/input/omnetpp.ini
