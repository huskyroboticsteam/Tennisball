#!/bin/bash
########################################################
### NOTE: THIS SCRIPT SHOULD BE RUN FROM THE PROJECT ###
###     DIRECTORY, **NOT** THE SCRIPTS DIRECTORY!    ###
########################################################

dir=`pwd`
scripts_dir=$dir"/scripts"
cd /tmp
#clone the object detection models
git clone https://github.com/tensorflow/models
cd $dir
#copy the object detection models into working dir
rsync -ravP /tmp/models/research/object_detection ./
#compile protobufs
protoc object_detection/protos/*.proto --python_out=./object_detection/
#copy the modified model_main into object_detection dir
cp $scripts_dir/model_main.py ./object_detection/
