#README
## OVERVIEW
Roadster implements an editor which allows you to build custom maps to test
learning policies on. It also features a full naive simulator and a
reinforcement learner in addition to some benchmarking policies.

## INSTALL
We have provided a build.sh file that should build the entire project
for you on a CSE machine. It would be a lot easier if we were not
restricted to what was already installed on the cse machines, so we
have had to include the source to two libraries we use (both open source).

If the script doesn't work you can try:

To install the project you first need to build the graphics engine.
cd lib/irrlicht/source/Irrlicht && make

Then you need to build the project itself (note, you want to cd back to 
the parent folder).

cd source && make

Finally you can run the application. Note that the application must be run in
its own directory.

./Roadster
