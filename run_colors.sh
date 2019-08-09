#!/bin/bash


while [ $# != 0 ]; do
	./cluster_image $1 2
	./cluster_image $1 4
	./cluster_image $1 8
	./cluster_image $1 16
	./cluster_image $1 32
	./cluster_image $1 64
	./cluster_image $1 128
	shift
done
