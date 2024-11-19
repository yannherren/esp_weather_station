#!/bin/bash

# Stop any existing monitor session
pkill -f "idf.py monitor"

idf.py build

idf.py flash

idf.py monitor