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
/*
    Generate and output proving key(pk) and vefification key(vk).
    Arguments are
        ･ output directory(pk)
        ･ output directory(vk)
    Output files are "pk_data" and "vk_data"
*/
./keygen /example/directory/ /example/directory/

/*  
    Coumpute a proof.
    Arguments are 
        ・ pk 
        ・ name (256bits)
        ・ password (256bits) 
    Packed hash value and proof are displayed in stdout.
*/
./prover ./pk_data 0 0 0 0 0 0 0 0 


/*
    verify the proof computed by prover.
    arguments are
        ・ vk
        ・ packed hash value
        ・ name (256bits)
        ・ proof
*/
./verifier ./vk_data 289655217040483233596024481866217137583 280298902915668251224293029255625572546 0 0 0 0 MCEunvV2mESag8cwnJPDAlns0NyguO91+IMWPAcQvmABMCAwxOP3bT6BC6Ec0J5JVhTf7uXsattQcaE8qkLf3Ju5tRcxCjDIGqVnBmpTUXyondurxMUv7TbYHL9ZZWs6a5SjvAAFEfBbO6LOCo8XDKLcNy1lB+OrVmbNuIfK5cL3CtIv8rEuMSAwKiZrugeADd3ZqzNLhdHxgJ8WwX4l+BYFNtSd+kFMdBAxCjAp/UW4JbQusVdXB7OecDsOSiGZsFDA0b6wp+AXCkCpGzAgMLzVSGsmXE3souqZRdNcDJlhQ4sooaGjjDHZ073FqCwcMQowarha/LZPRMtnZGUVOrxhzo4CPmURa9CM2/wFsu1OuSEwCjBerqMuq1mnwaJGWiJx2jD+8gA7Q0J7n1DDgRoyTspABzEK

```

