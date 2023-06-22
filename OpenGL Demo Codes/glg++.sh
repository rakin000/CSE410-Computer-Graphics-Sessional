#!bin/bash
filename=$1
extension="${filename##*.}"
filename="${filename%.*}"
g++ $1 -o $filename -lglut -lGLU -lGL 