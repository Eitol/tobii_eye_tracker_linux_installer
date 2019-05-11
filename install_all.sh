#!/usr/bin/env bash

LIB_DIR=./lib


sudo apt --fix-broken install libsqlcipher0 ./deps/libuv0.10_0.10.22-2_amd64.deb libpthread-stubs0-dev build-essential

# Targeted Linux distribution is Ubuntu Linux 16.04 LTS
# Tested on  Ubuntu 18.10 x86_64 with Cinnamon. Kernel  4.18.0-16-generic

# Install Tobii USB Host (mandatory) that handles connections to the tracker:
sudo dpkg -i tobiiusbservice_l64U14_2.1.5-28fd4a.deb

# Install Tobii Engine daemon (recommended) that offers extended functionality
sudo dpkg -i tobii_engine_linux-0.1.6.193_rc-Linux.deb

#Install Tobii Config (recommended) to do screen setup and calibration:
sudo dpkg -i tobii_config_0.1.6.111_amd64.deb

#Install Stream Engine Client library to develop for your eye tracker:
if [[ ! -e "${LIB_DIR}" ]]; then
    mkdir ${LIB_DIR}
    tar -xzvf stream_engine_linux_3.0.4.6031.tar.gz -C ${LIB_DIR}
else
    echo "${LIB_DIR} already exist. Continue..."
fi

sudo mkdir /usr/lib/tobii
sudo cp -pR ${LIB_DIR}/lib/x64/*.so /usr/lib/tobii/
sudo cp ./tobii.conf /etc/ld.so.conf.d/
sudo mkdir /usr/include/tobii
sudo cp -R ${LIB_DIR}/include/tobii/* /usr/include/tobii

echo "DONE :)"

sudo apt install ./Tobii.Pro.Eye.Tracker.Manager.Linux-1.12.1.deb
