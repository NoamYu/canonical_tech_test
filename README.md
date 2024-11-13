# canonical_tech_test
A technical test as part of the interview process to canonical. 

The project is built using CMake.
To build the project run 

```
mkdir build
cd build
cmake ..
make
```

The project uses nlohmann_json and libcurl and they should be installed before buildings using brew or apt-get

```
brew install nlohmann-json
```

The program is run with CLI

- To get all suported versions run:
```
./SimpleStreamInfoFetcher -supported
```
- To get the latest LTS version run:
```
./SimpleStreamInfoFetcher --LTS
```
- To get the sha256 of an image run:
```
./SimpleStreamInfoFetcher -sha <version_name> <sub_version>
```
version name is the key of the product array in the jason file, e.g. com.ubuntu.cloud:server:17.04:amd64. sub version is a version key of the same product e.g. 20171208.
* will only fetch amd64 images.