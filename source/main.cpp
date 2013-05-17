#include "Roadster.hpp"

int main(int argc, char* argv) {
	// Create a naive roadster.
	RoadsterNaive roadster("data/map/default.conf");

	roadster.run();
	return 0;
}
