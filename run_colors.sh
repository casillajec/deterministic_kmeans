#!/bin/bash


while [ $# != 0 ]; do
	time ./cluster_image $1 2
	time ./cluster_image $1 4
	time ./cluster_image $1 8
	time ./cluster_image $1 16
	time ./cluster_image $1 32
	time ./cluster_image $1 64
	time ./cluster_image $1 128
	shift
done
