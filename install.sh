#!/bin/bash 

# create the TRACK database in the .config folder
mkdir $HOME/.config/track
touch $HOME/.config/track/TRACK.db

#run track and move the executable into usr/local/bin 
gcc -o track tracker.c main.c -lsqlite3 -std=c99
sudo mv track /usr/local/bin






