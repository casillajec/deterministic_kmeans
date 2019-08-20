#!/bin/bash


while [ $# != 0 ]; do
	time ./cluster_image $1 2 -w
	time ./cluster_image $1 4 -w
	time ./cluster_image $1 8 -w
	time ./cluster_image $1 16 -w
	time ./cluster_image $1 32 -w
	time ./cluster_image $1 64 -w
	time ./cluster_image $1 128 -w
	shift
done
