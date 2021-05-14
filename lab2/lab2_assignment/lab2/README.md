# ```============== [HWSec 2020 LAB. 0x2] ==============```


# ```TASK #1 FLUSH + RELOAD```

The goal of this task is to leak with a Flush+Reload attack **a secret key of 8 Bytes** 
which is defined in ```flush_reload.c```. 

### Note
You should leak the secret key one byte at the time 

## ```Tips```

You can define a custom key to test your Flush+Reload code by defining the
```SECRET``` compiler flag when building your program as follows:

```
$ CFLAGS=-DSECRET=0x6162636465666768 make flush_reload
$ ./flush_reload
```

# ```Bonus Task: EVICT + RELOAD```

You need to leak the same secret key by performing the Evict+Reload attack, 
which means you cannot make use of the ```clflush``` instruction anymore to 
flush your buffer cachelines but instead you need to build a cache eviction set 
of addresses and access them to evict the cachelines you want.

More specifically, if you want to evict the first cache line in a 4K page, you
can achieve this by accessing the first cache line in a sufficient amount of 4K
pages.

Your eviction code should be built by defining the ```EVICT``` compiler flag
when building the ```flush_reload``` target, you do so as follows:

```
$ CFLAGS=-DEVICT make flush_reload
$ ./flush_reload
```

# ```Task #2 Meltdown & Task #3 Spectre```

Now that you have implemented F+R, you need to use this cache side-channel
attack to leak the content of a 32 bytes secret string placed into a buffer in 
the Write-Only-Memory kernel memory.

The ```wom/``` directory contains a kernel module that maps a buffer in memory.
The goal of the assignment is to leak the contents of this buffer.

The wom module is already loaded in kernel memory in all the cluster nodes.

You can check that by running:

```
$ lsmod | grep wom
```

The wom module exposes different funcionalities which you could use to 
manipulate the secret string.

You can set the secret string to leak to a custom one to test your code as 
follows:
```
$ echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" > /dev/wom
```

Your tasks are to leak the content of the buffer pointed to by ```secret_addr```
using the following techniques:

- Leak the generated secret using Meltdown TSX
- Leak the generated secret using Meltdown in a Spectre speculative branch

To build and run your Meltdown attack program, you can do that as follows:
```
$ make meltdown
$ ./meltdown
```

To build and run your Spectre attack program, you can do that as follows:
```
$ make spectre
$ ./spectre
```

## ```Tips```

You can find the ```test_meltdown_spectre.sh``` script which you can use to test
your implementation of both tasks #2 Meltdown and #3 Spectre, it will change the 
contents of secret buffer and see if your program can leak it correctly. Assume 
you want to leak the first 32 bytes of the buffer and that they are all hex 
digits.

If you run:
```
$ ./test_meltdown_spectre.sh
```

The script executes one time your implementation of ```./meltdown``` and
```./spectre``` and compares the leaked secret with the generated one. The
script tells you at the end of each test whether your corresponding
implementation is a ```PASS``` or ```FAIL```.

If you run:
```
$ ./test_meltdown_spectre.sh batch
```

The script executes 100 times your implementation of ```./meltdown``` and
```./spectre``` and tells you the success rate of each binary.

Make sure that both implementations of tasks #2 and #3 ```PASS``` the
```test_meltdown_spectre.sh``` script.

# ```Deliverable```

## ```Task #1 FLUSH+RELOAD```

The source code of ```flush_reload.c``` that once compiled is able to retrieve 
the secret key by performing Flush+Reload.

## ```Task #2 MELTDOWN```
The source code of ```meltdown.c``` that implements the Meltdown attack using
**Intel TSX** and leaks the content of the secret buffer created by the WOM
kernel module.

## ```Task #3 SPECTRE```
The source code of ```spectre.c``` that implements the Meltdown attack using
**Spectre speculative branch** and leaks the content of the secret buffer 
created by the WOM kernel module.

## ```Bonus Task EVICT+RELOAD```
Within the ```EVICT``` section of ```flush_reload.c``` implement the **EVICT** +
RELOAD attack to leak the secret key, make sure we can enable this attack by 
specifying ```-DEVICT``` compiler when building the ```flush_reload``` target.

# ```Notes```

Make sure to test your implementations of **ALL TASKS** on the **HWSec cluster**
before submitting your code. We are going to grade your submissions on the same
cluster.

# ```Deadline```

You're required to submit before **Fri, November 6th 2020 @ 23:59**.
Every late day causes 1 penalty point on the grade (max. delay of 3 days).

# ```Grades```

- **5 : (Task #1)** You managed to recover the secret key through Flush+Reload
- **9 : (Task #2)** You managed to recover the second secret key from kernel
memory by implementing Meltdown attack using Intel TSX.
- **10: (Task #3)** You managed to recover the second secret key from kernel
memory by implementing Meltdown attack using Spectre speculative branch.
- **11: (Bonus #1)** You managed to recover the secret key Evict+Reload
