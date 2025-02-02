#!/bin/bash

function build() {
    echo $1

    rm -rf build
    mkdir -p build
    cd build || exit 1
    cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    clang-tidy ../src/*.cpp
    cmake --build . --config Release -j $(nproc)
}

function release() {
    echo $1

    rm -rf build
    mkdir -p build
    cd build || exit 1

    cmake ..
    cmake --build . --config Release -j $(nproc)

    ls -lah
    ls -lah ./bin/

    zip -j -r $CI_PROJECT_DIR/athena-release-$CI_COMMIT_TAG-linux.zip bin/*

    echo "BUILD_JOB_ID=$CI_JOB_ID" >> $CI_PROJECT_DIR/build.env
}

if [ "$1" == "Release" ]; then
    release
fi

if [ "$1" == "Build-Only" ]; then
    build
fi
