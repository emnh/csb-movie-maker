# CSB Movie Maker

Based on inoryy's CSB starter kit at https://github.com/inoryy/csb-ai-starter .

Some scripts for making replay movie of CSB match.

Can parse replays from brutaltester https://github.com/dreignier/cg-brutaltester and
using judge from
https://github.com/robostac/coders-strike-back-referee .

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

# How to make replay: input.txt

It is input for one player for whole game.

To produce input.txt run the brutal tester on a normal CSB program first:

```
#!/bin/bash
java -jar cg-brutaltester.jar -o -r "coders-strike-back-referee/coders-strike-back-referee" -p1 "./main" -p2 "./main" -t 2 -n 1 -l "./logs/"
```

Then parse judge logs to a suitable format:

```
./parse.sh < logs/game1.txt > input.txt
```
