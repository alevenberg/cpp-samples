# We chose ubuntu as the base image because we are familiar it, you can pick any image you'd like.
FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git

# Create a working directory
WORKDIR /root

# Copy source code into the container
COPY . /root

# Build the application with CMake.
RUN cmake -S . -B .build
RUN cmake --build .build

# Define environment variables.
ENV INPUT_FILE=data/input/hello.txt
ENV OUTPUT_FILE=data/output/hello.txt

# Define entrypoint to script.
ENTRYPOINT ./.build/src/main "${INPUT_FILE}" "${OUTPUT_FILE}"
