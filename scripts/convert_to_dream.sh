#!/bin/sh

for i in ../inputs/africanmovie/*.jpg; do echo $i; gmic $i -dream -o ../render/dream_${i##*/}; done

