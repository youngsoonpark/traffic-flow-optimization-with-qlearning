#README
## OVERVIEW
Roadster implements an editor which allows you to build custom maps to test
learning policies on. It also features a full naive simulator and a
reinforcement learner in addition to some benchmarking policies.

## INSTALL
To install the project you first need to build the graphics engine.
cd lib/irrlicht/source/Irrlicht && make

Then you need to build the project itself (note, you want to cd back to 
the parent folder).

cd source && make

Finally you can run the application.

./Roadster
