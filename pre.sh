#!/usr/bin/env bash
VENDORS_DIR=`pwd`/vendors
INSTALL_DIR=`pwd`/local
FIRST_DIR=`pwd`

get_hyperscan()
{
    if [[ ! -f  $VENDORS_DIR/boost_1_67_0.tar.gz ]]; then
        wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz  -O  $VENDORS_DIR/boost_1_67_0.tar.gz
    fi

    if [[ ! -d  $VENDORS_DIR/boost_1_67_0 ]]; then
        tar -xvf $VENDORS_DIR/boost_1_67_0.tar.gz -C $VENDORS_DIR
    fi

    cd  $VENDORS_DIR
    if [[ ! -d  hyperscan ]]; then
        git clone https://github.com/intel/hyperscan
    else
        cd hyperscan && git pull
    fi
    cd  $FIRST_DIR
}

#http://intel.github.io/hyperscan/dev-reference/getting_started.html
build_hyperscan()
{
    build_dir=$VENDORS_DIR/hyperscan/build
    mkdir -p ${build_dir} && cd  ${build_dir}
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DBOOST_ROOT=$VENDORS_DIR/boost_1_67_0 -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR  .. 
    make -j2 install
    cd  $FIRST_DIR
}


get_hyperscan
build_hyperscan
