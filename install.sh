#!/bin/bash

if [ -f /etc/os-release ]; then
    # shell will source this file and set the $ID variable
    . /etc/os-release
fi

install_deps() {
    echo "Installing dependencies..."
    $@ -y install ncurses libnoise-dev libssl-dev libcrypto++-dev libsdl2-dev libsdl2-mixer-dev libstdc++6
    echo "Done!"
}

case $ID in
    "ubuntu"|"debian")
        install_deps apt-get update && apt-get upgrade
        ;;
    "centos"|"rhel"|"fedora")
        install_deps yum update && yum upgrade
        ;;
    "suse"|"opensuse")
        install_deps zypper refresh && zypper up
        ;;
    *)
        echo "Sorry, your distribution is not supported by this script."
        exit 1
        ;;
esac


