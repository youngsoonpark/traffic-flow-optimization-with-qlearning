#include "SimulationPolicyCellular.hpp"

namespace road
{
namespace sim
{

void SimulationPolicyCellular::update(core::State& state)
{
  /**
   * Alright, here's where stuff gets interesting.
   * Unlike in Naive, Cellular will attempt to replicate a known, working traffic model.
   * This is called the Nagel–Schreckenberg model and serves as the underlying basis of the extended simulation.
   *
   * What to expect in the Cellular policy:
   *
   * 1 <= Speed limit <= 11
   * 0.00 <= Bad boy factor <= 1.00
   * 45% <= Chance for car to appear <= 65%
   *
   * At least 2 way traffic (possibly more sources & sinks)
   * 4 lanes per road
   * Pedestrians (simple, source/sink is put at a different location)
   * Speeding and running red lights
   *
   * How speeding and running red lights will work:
   *
   * It's simple: We eat the Batman.
   *
   * bad boy factor is assigned a random double between 0.00 and 1.00
   * A bad boy factor of 0.0 is regarded as the perfect driver: i.e. will not speed/run red light, nor will do < speed limit
   * Accidents probably won't be resolved; possible problem and needs discussion
   * Chance of speeding is independent of chance of running a red light; it is possible for a bad boy to do both :(
   *
   * IF bad boy factor == 0.00 THEN
   *     Perfect driver
   *     May need to modularise Naive simulator, since all Naive drivers are perfect also
   * ELSE IF 0.00 < bad boy factor <= 0.25 THEN
   *    Chance of running red light = 0.0
   *    Chance of speeding = (bad boy factor) / 100
   *    Maximum speed = ((bad boy factor) / 2 * speed limit) + speed limit
   *
   *    IF NOT speeding THEN
   *        To do
   *    END IF
   * ELSE IF 0.25 < bad boy factor <= 0.50 THEN
   *     Chance of running red light = (bad boy factor) * 0.25
   *     Chance of speeding = (bad boy factor) * 0.3
   *     Maximum speed = ((bad boy factor) * speed limit) + speed limit
   *
   *     IF NOT speeding THEN
   *         To do
   *     END IF
   * ELSE
   *    Chance of running red light = (bad boy factor) * 0.50
   *    Chance of speeding = (bad boy factor) * 0.70
   *    Maxmimum speed = ((bad boy factor) * 2.00 * speed limit) + speed limit
   *
   *    IF NOT speeding THEN
   *      To do
   *    END IF
   * END IF
   */
}

} // End of namespace sim.
} // End of namespace road.
