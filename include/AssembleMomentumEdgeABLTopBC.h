// Copyright 2017 National Technology & Engineering Solutions of Sandia, LLC
// (NTESS), National Renewable Energy Laboratory, University of Texas Austin,
// Northwest Research Associates. Under the terms of Contract DE-NA0003525
// with NTESS, the U.S. Government retains certain rights in this software.
//
// This software is released under the BSD 3-clause license. See LICENSE file
// for more details.
//


/** @file ActuatorLineFAST.h
 *  @brief A class to implement an open top boundary condition for atmospheric
 *  boundary layer simulations.
 *
 *  abltop will allow for inflows and outflows generated by terrain or by 
 *  wind turbine blockage.  It can also enable a vertical temperature gradient 
 *  at the upper boundary as dictated by the background atmosphere.  The
 *  open boundary component works by solving a potential flow problem in a
 *  thin sub-domain extending down from the top boundary to a 'sampling plane'
 *  located about 10% of the vertical domain height below the upper boundary.
 *  Fourier transform methods are used to generate a solution for the 
 *  velocity field at the upper boundary from the vertical velocity 
 *  distribution on the sampling plane.  The current implementation assumes
 *  a structured Cartesian mesh, where nodes are tagged with a global mesh
 *  index (as done by the abl_mesh program).  The current implementation also
 *  assumes that the the grid points are uniformly spaced in the horizontal
 *  directions at the elevation of the sampling plane.  The structured
 *  Cartesian mesh restriction could be removed by using the 
 *  BdyLayerVelocitySampler function to interpolate arbitrarily-placed data 
 *  onto a uniformly-sampled plane.
 *
 */

#ifndef AssembleMomentumEdgeABLTopBC_h
#define AssembleMomentumEdgeABLTopBC_h

#include<SolverAlgorithm.h>
#include<FieldTypeDef.h>
#include<complex> // Must proceed fftw3.h in order to get native c complex
#include<fftw3.h>

namespace stk {
namespace mesh {
class Part;
}
}

namespace sierra{
namespace nalu{

class Realm;

/** AssembleMomentumEdgeABLTopBC class definition.
  *
  * The main AssembleMomentumEdgeABLTopBC class contains member functions 
  * initialize_connectivity, execute, initialize, potentialBCPeriodicPeriodic,
  * potentialBCInflowPeriodic, and potentialBCInflowInflow.
  *
  */

class AssembleMomentumEdgeABLTopBC : public SolverAlgorithm
{
public:

  AssembleMomentumEdgeABLTopBC(
    Realm &realm,
    stk::mesh::Part *part,
    EquationSystem *eqSystem, std::vector<int>& grid_dims_,
    std::vector<int>& horiz_bcs_, double z_sample_);
  virtual ~AssembleMomentumEdgeABLTopBC();
  virtual void initialize_connectivity();

  /** Main function to compute and set the boundary values.
    */
  virtual void execute();

  /** Function to initialize static data on the first call.  It discovers
    * the computational box size, the distance between the sampling plane
    * and the upper boundary, forms lists of the sampling plane and upper 
    * boundary plane node ids, and intitializes the FFT templates.
    */
  virtual void initialize();

  /** Solves the potential flow problem for periodic-periodic conditions
    * in x and y.
    * @param wSamp Input array containing the vertical velocity on the 
    * sampling plane.
    * @param UAvg Input array containing the average velocity over the
    * sampling plane.
    * @param uBC Output array containing the u velocty component at the
    * upper boundary.
    * @param vBC Output array containing the v velocty component at the
    * upper boundary.
    * @param wBC Output array containing the w velocty component at the
    * upper boundary.
    */
  virtual void potentialBCPeriodicPeriodic(
    std::vector<double>& wSamp,
    std::vector<double>& UAvg,
    std::vector<double>& uBC,
    std::vector<double>& vBC,
    std::vector<double>& wBC );

  /** Solves the potential flow problem for inflow-periodic conditions
    * in x and y.
    * @param wSamp Input array containing the vertical velocity on the
    * sampling plane.
    * @param UAvg Input array containing the average velocity over the
    * sampling plane.
    * @param uBC Output array containing the u velocty component at the
    * upper boundary.
    * @param vBC Output array containing the v velocty component at the
    * upper boundary.
    * @param wBC Output array containing the w velocty component at the
    * upper boundary.
    */
  virtual void potentialBCInflowPeriodic(
    std::vector<double>& wSamp,
    std::vector<double>& UAvg,
    std::vector<double>& uBC,
    std::vector<double>& vBC,
    std::vector<double>& wBC );

  /** Solves the potential flow problem for inflow-inflow conditions
    * in x and y.
    * @param wSamp Input array containing the vertical velocity on the
    * sampling plane.
    * @param UAvg Input array containing the average velocity over the
    * sampling plane.
    * @param uBC Output array containing the u velocty component at the
    * upper boundary.
    * @param vBC Output array containing the v velocty component at the
    * upper boundary.
    * @param wBC Output array containing the w velocty component at the
    * upper boundary.
    */
  virtual void potentialBCInflowInflow(
    std::vector<double>& wSamp,
    std::vector<double>& UAvg,
    std::vector<double>& uBC,
    std::vector<double>& vBC,
    std::vector<double>& wBC );

  /** Class variable definitions.
    */
  VectorFieldType *velocity_;
  VectorFieldType *bcVelocity_;
  ScalarFieldType *density_;
  GenericFieldType *exposedAreaVec_;
  int imax_, jmax_, kmax_;
  std::vector<double> xInflowWeight_, yInflowWeight_;
  std::vector<stk::mesh::Entity> nodeMapSamp_, nodeMapBC_, nodeMapM1_,
                                 nodeMapXInflow_, nodeMapYInflow_;
  std::vector<int> indexMapSampGlobal_, indexMapBC_, sampleDistrib_, displ_,
                   horizBC_;
  double xL_, yL_, deltaZ_, zSample_;
  int nBC_, nXInflow_, nYInflow_, horizBCType_;
  bool needToInitialize_;
  fftw_plan planFourier2dF_, planFourier2dB_, planSinx_, planCosx_,
            planFourierxF_, planFourierxB_,   planSiny_, planCosy_,
            planFourieryF_, planFourieryB_;
};

} // namespace nalu
} // namespace Sierra

#endif
