#!/bin/sh
apt-get update  # To get the latest package lists

# installs dependenecies
apt-get install pkgconfig -y
apt-get install libglfw3 -y
apt-get install libglfw3-dev -y
apt-get install libglew-dev -y

# builds program
make
