# Building & running tests

To install the dependencies:

On Ubuntu 16.04 LTS:

```
sudo apt-get install build-essential cmake git libgmp3-dev libprocps4-dev python-markdown libboost-all-dev libssl-dev
```

In order to download and build the repo:

```
git clone https://github.com/kskhasegawa/libsnark-ctf.git
cd libsnark-ctf
git submodule update --init --recursive
mkdir build && cd build && cmake ..
make
```

To run the tests go to the `build` directory and run:

```
//generate proving key and vefification key.
./keygen

/*  
    generate a proof.
    arguments are 
        ・ name (256bits)
        ・ password (256bits)
*/
./prover 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000


/*
    verify the proof generated by prover.
    arguments are
        ・hash value. hash(name | value | password)
        ・name (256bits)
*/
./verifier 289655217040483233596024481866217137583 280298902915668251224293029255625572546 0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000

```
