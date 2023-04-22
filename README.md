Triplecross is a puzzle made by Binary Arts. More information here:

  https://www.jaapsch.net/puzzles/port.htm

This program solves a Triplecross position if you pass it on the
command line. If you don't pass any arguments, it prints out solutions
for the 7 hardest triplecross starting positions. The program only
considers the positions where the left plunger is down, so the number
of positions is half what Jaap has on his site.

These are the positions followed by the solution to each:

```
    ..
.b4.--.
3..1a-.
 2.

rucd rucd rucd uldc uldc rucd rucd lucd urdc uldc uldc urdc lucd rucd rucd lucd
lucd urdc lucd lucd rucd lucd urdc urdc

    ..
3..a---
.4..b..
 21

rucd rucd lucd urdc urdc urdc lucd lucd rucd uldc rucd lucd lucd lucd rucd lucd
rucd rucd uldc uldc rucd rucd rucd uldc

    .-
24.13.-
....b-a
 ..

rucd rucd rucd lucd rucd lucd lucd rucd lucd urdc lucd lucd urdc lucd rucd uldc
urdc uldc uldc urdc lucd lucd rucd uldc

    -.
.42.-..
.1a..-b
 3.

rucd rucd rucd uldc rucd lucd rucd rucd rucd lucd lucd urdc lucd lucd urdc uldc
rucd lucd lucd urdc lucd lucd rucd lucd

    -.
2.....-
3.ba..-
 14

rucd rucd uldc uldc urdc lucd urdc uldc rucd lucd urdc lucd rucd uldc rucd lucd
urdc urdc lucd rucd lucd urdc urdc uldc

    b-
2....-a
.1..-..
 43

rucd rucd rucd uldc rucd uldc urdc lucd lucd rucd rucd lucd rucd uldc rucd rucd
lucd lucd urdc lucd rucd uldc uldc rucd

    -b
2..4-..
.1...-a
 .3

rucd rucd lucd lucd lucd urdc urdc lucd rucd uldc urdc urdc urdc uldc urdc urdc
urdc lucd rucd lucd rucd lucd urdc uldc
```

The first time you run the program it builds a lookup table. Subsequent runs
load the table without needing to recompute. The file is 702 megabites. It
stores two bits for each position. The information stored is the distance from a
solved state mod 3. This is sufficient because for any position we can look at
all the adjacent positions and choose one that is one less.

Here's the output of the program as it builds the lookup table:

```
   depth             num            total    time to
                  positions                  compute
depth  0 - count          1 total          1 0:00:00
depth  1 - count          4 total          5 0:00:00
depth  2 - count         12 total         17 0:00:00
depth  3 - count         36 total         53 0:00:00
depth  4 - count        104 total        157 0:00:00
depth  5 - count        303 total        460 0:00:00
depth  6 - count        884 total       1344 0:00:00
depth  7 - count       2579 total       3923 0:00:00
depth  8 - count       7521 total      11444 0:00:00
depth  9 - count      21937 total      33381 0:00:01
depth 10 - count      63923 total      97304 0:00:01
depth 11 - count     186196 total     283500 0:00:01
depth 12 - count     542124 total     825624 0:00:02
depth 13 - count    1577700 total    2403324 0:00:04
depth 14 - count    4585298 total    6988622 0:00:12
depth 15 - count   13282991 total   20271613 0:00:34
depth 16 - count   38149858 total   58421471 0:01:37
depth 17 - count  107133350 total  165554821 0:04:59
depth 18 - count  283555419 total  449110240 0:13:55
depth 19 - count  644626804 total 1093737044 0:29:26
depth 20 - count 1014637237 total 2108374281 0:48:24
depth 21 - count  719688345 total 2828062626 1:00:19
depth 22 - count  111515380 total 2939578006 1:02:08
depth 23 - count     959587 total 2940537593 1:02:09
depth 24 - count          7 total 2940537600 1:02:09
```

You can provide a position on the command line by removing all whitespace, e.g.:

```
./tc -b2..4-...1...-a.3
rucd rucd lucd lucd lucd urdc urdc lucd rucd uldc urdc urdc urdc uldc urdc urdc urdc lucd rucd lucd rucd lucd urdc uldc
```
