## The program

This example is a Hello World program using [Open MPI](https://www.open-mpi.org/).

# To build and run

```
cd cpp-samples/batch/cluster/application
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake -G Ninja
cmake --build build
build/main
```

# To create docker image

```
docker build --tag=cluster-image:latest .
```

## To run and enter your image

```
docker run -it --entrypoint bash cluster-image:latest
```

To exit container, type `exit`
