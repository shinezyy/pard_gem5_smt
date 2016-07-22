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

I have not found information about how to compile it in parallel.
Then you will get a list about compilation success and error.

## how to set benchmarks as workloads for gem5 ?

Copy three files from 172.18.11.212:/home/zhouyaoyang/projects/gem5 :

- run\_gem5\_alpha\_spec06\_benchmark.sh
- configs/spec/spec06\_benchmarks.py
- configs/spec/spec06\_config.py

to your corresponding directory.

Then your should search "zhouyaoyang" in these files to modify some environment variables, like:

- GEM5\_DIR and SPEC\_DIR in run\_gem5\_alpha\_spec06\_benchmark.sh
- spec\_dir in spec06\_benchmarks.py (this spec\_dir is different from the one above, see my current configuration for detail)

Note that only perlbench is well configured now. And if you want to run other benchmarks, you can define xxx\_input\_ref\_dir variables
like perlbench.

Note that most commented lines are not useable, which need modifications above.

Fianally, go to your gem5 directory, and run
```
./run_gem5_alpha_spec06_benchmark.sh perlbench $output_dir
```

Note that $output\_dir should be an **absolute position**, not a relative one. Otherwise, you have to go to exe directory to find you results.

## new:
Double benchmarks form one binary is supported:
like
```
./run_gem5_alpha_spec06_benchmark.sh "perlbench;perlbench" ~/tmp/perlbench2/ --smt
```

But I have not tested it...
