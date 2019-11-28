// Copyright 2017 National Technology & Engineering Solutions of Sandia, LLC
// (NTESS), National Renewable Energy Laboratory, University of Texas Austin,
// Northwest Research Associates. Under the terms of Contract DE-NA0003525
// with NTESS, the U.S. Government retains certain rights in this software.
//
// This software is released under the BSD 3-clause license. See LICENSE file
// for more details.
//



#ifndef StateTable_h
#define StateTable_h

#include <string>
#include <vector>

namespace sierra{
namespace nalu{

class StateTable
{
public:

  StateTable(
    const std::string propertyTableName );
  ~StateTable();

  std::vector<std::vector<double > > table_;

  std::vector<std::vector<double> > *get_entry();

};

} // namespace nalu
} // namespace Sierra

#endif
