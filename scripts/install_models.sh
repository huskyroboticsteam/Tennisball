#!/bin/bash
dir=`pwd`
cd /tmp
git clone https://github.com/tensorflow/models
cd $dir
rsync -ravP /tmp/models/research/object_detection ./
protoc object_detection/protos/*.proto --python_out=./object_detection/

