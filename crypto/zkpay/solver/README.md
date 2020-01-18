# zkpay solver

```
pushd src/snark/depends/
git clone https://github.com/scipr-lab/libsnark.git
pushd libsnark
git submodule update --init --recursive
popd; popd
```

```
docker build -t zkpay-solver .
docker run --env URL=<URL> --rm zkpay-solver
```
