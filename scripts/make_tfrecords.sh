#!/bin/bash
echo "Generating TFRecords for Training dataset..."
python scripts/generate_tfrecords.py -f ./data/training/training.csv -o ./data/training/train.tfrecord
echo "Generating TFRecords for Testing dataset..."
python scripts/generate_tfrecords.py -f ./data/testing/testing.csv -o ./data/testing/test.tfrecord
