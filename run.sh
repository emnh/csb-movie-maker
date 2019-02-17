#!/bin/bash
video=video.mp4
make
rm -f /mnt/g/out/*.ppm
time ./img < input.txt
rm -f $video
#ffmpeg -i /mnt/g/out/csb%05d.ppm -r 60 -pix_fmt yuv420p video.webm
ffmpeg -i /mnt/g/out/csb%05d.ppm -r 60 -pix_fmt yuv420p -g 1 $video
chrome.exe $video
