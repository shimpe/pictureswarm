#!/bin/sh

for i in ../inputs/africanmovie/*.jpg; do echo $i; gmic $i -lylejk -o ../render/dream_${i##*/}; done

