#ifndef ROAD_IO_SERIALIZATION_TEXT_H_
#define ROAD_IO_SERIALIZATION_TEXT_H_

#include  <string>

namespace road
{

namespace core
{
class State; // Prototype of the state.
}

namespace io
{

/**
 * @author Benjamin James Wright <bwright@cse.unsw.edu.au>
 */
struct SerializationXML {
  /**
   * Takes a filepath and loads the appropriate state.
   */
  void load(const std::string& filepath, core::State& state) const;

  /**
   * Takes a state and saves it to the specified file. Which is stored in the state.
   */
  void save(const core::State& state) const;
};

} // End of namespace io.
} // End of namespace road.

#endif  // ROAD_IO_SERIALIZATION_TEXT_H_
