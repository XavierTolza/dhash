# About
A simple program to compute the dhash of images for all images recursively found in a given folder and its subfolders.

The dHash is a uint64 value that summarizes a given image. Two likely images might have a close dhash (with a hamming distance)

# Compilation
Fend for yourself bro.

# Usage
```
./DHashExample -i path/to/your/folder | tee out.csv
```

This exports a csv file you can later on open with python/pandas for further processing