# CSB Movie Maker

Based on inoryy's CSB starter kit at https://github.com/inoryy/csb-ai-starter .

Some scripts for making replay movie of CSB match.

Can parse replays from brutaltester https://github.com/dreignier/cg-brutaltester and
using judge from
https://github.com/robostac/coders-strike-back-referee .

# Explanation of weird paintball lines seen in movie

 - MadKnight  03:24PM: just tell me what are those weird lines emh
 - emh: MadKnight those are a few pixels colored at the point the pod would be for every solution evaluated.
   6 turns ahead that is.
   or whatever depth you use.
   supposed to help to visualize how your bot "thinks".
 - emh 03:32PM
  it's drawn by https://github.com/emnh/csb-movie-maker/blob/master/img.cpp#L145
  which is called by https://github.com/emnh/csb-movie-maker/blob/master/main.cpp#L664

# How to make movie from replay in input.txt

First install ffmpeg
```
apt-get install ffmpeg
```

Main script to make movie is run.sh.
It calls ./img on input.txt, which is included example replay in the repo.
Search all files for /mnt/g/out and replace with a suitable output path.

```
fgrep /mnt/g/out *
```

Make the output directory.

```
mkdir /mnt/g/out

```

I use a RAM disk for /mnt/g/out made by ImDisk for faster output and not wear out my SSD.

https://sourceforge.net/projects/imdisk-toolkit/

Size of PPM image outputs is around 1.2 GiB for one simple match. Can be much
more for a longer match of course. Compressed output movie from PPMs is around
10 MiB.

# How to make replay: input.txt

It is input for one player for whole game.

To produce input.txt run the brutal tester on a normal CSB program first:

```
#!/bin/bash
java -jar cg-brutaltester.jar -o -r "coders-strike-back-referee/coders-strike-back-referee" -p1 "./main" -p2 "./main" -t 2 -n 1 -l "./logs/"
```

Then parse judge logs to a suitable format:

```
./parselog.sh logs/game1.log > input.txt
```
