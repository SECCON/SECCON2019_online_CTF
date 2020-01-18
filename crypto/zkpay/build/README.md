# zkpay

```
pushd flask/src/snark/depends/
git clone https://github.com/scipr-lab/libsnark.git
pushd libsnark
git submodule update --init --recursive
popd; popd
docker-compose build
docker-compose up -d
```
