# TomasuloSimulator

## Developing environment
Ubuntu 23.10, clang 16.0.6, cmake 3.27.4, CPP_Standard = C++17

## How to build the simulator
- easily, just `make` on the directory **/TomasuloSimulator**
- advancedly, `mkdir build`, then `cd build`, `cmake ..`, `make`

## How to run the simulator
`./build/TomasuloSimulator [your MIPS assembly code file]` ,  
e.g., `./build/TomasuloSimulator ./tests/CODE1.S`
## Usage:
- v [i | l | r | s | a] : display instructions status | load and reservation stations | registers result status | statistics | all information aforesaid
- s [n(optional)] : step 1/n cycle(s)
- r : run to the end
- b [n] : look back the info of the simulator at the nth clock cycle
- q : quit the simulator