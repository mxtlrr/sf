# sf
Generates seeds based off a path given in a map maker. Built for
SCP: Containment Breach v1.3.11. Written in C++.

## Flowchart / how it works
TODO..

## Compilation
The GUI is coming soon. It'll use Raylib or SDL2 or something. Undecided
for know. You'll need a C compiler, like C or C++ and Makefile. Then you
can just run
```
make
```

If you get an issue about unknown `_start`, just run `make` again. It'll
work.

# TODO
- [ ] Finish porting SCP:CB's map gen
  - [ ] Generate more than zone 1
    - [ ] zone 2 (HCZ)
    - [ ] zone 3 (EZ)
    - [ ] Gate A / Gate B (doesn't really matter)
  - [ ] Potentially optimize to make it faster?
- [ ] Make a GUI to make the zone
- [ ] Keep pre-computed maps saved on disk (so recomputation isn't an issue)
  - [ ] Store in own file with own format to compress to store as little 
  disk space as possible

Other stuff TBA.