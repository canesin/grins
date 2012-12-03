//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// GRINS - General Reacting Incompressible Navier-Stokes 
//
// Copyright (C) 2010-2012 The PECOS Development Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the Version 2 GNU General
// Public License as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA
//
//-----------------------------------------------------------------------el-
//
// $Id$
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef BC_FACTORY_H
#define BC_FACTORY_H

#include <string>
#include <map>

// libMesh stuff
#include "getpot.h"
#include "equation_systems.h"

// GRINS stuff
#include "var_typedefs.h"
#include "grins_physics_names.h"
#include "dbc_container.h"
#include "nbc_container.h"

namespace GRINS
{
  //! Object for constructing boundary condition function objects.
  /*! BoundaryConditionsFactory will construct the function objects, for each physics,
      within the returned containers. By default, the build methods return a default (empty)
      container. The user must construct their own structures in derived classes. */
  class BoundaryConditionsFactory
  {
  public:
    
    BoundaryConditionsFactory( );

    virtual ~BoundaryConditionsFactory();

    //! Builds all required libMesh::DirichletBoundary objects and adds them to DofMap
    /*! This needs to be done for all the physics in the simulation, for all
      "special" dirichlet boundary conditions. */     
    virtual std::multimap< GRINS::PhysicsName, GRINS::DBCContainer > build_dirichlet();


    //! Builds all Neumann boundary condition function objects needed
    /*! This is done for each physics. Thus, the container returned maps between each physics 
      and set of Neumann boundary conditions for that physics. */
    virtual std::multimap< GRINS::PhysicsName, GRINS::NBCContainer > build_neumann( libMesh::EquationSystems& equation_system );
 
  }; // class BoundaryConditionsFactory

} // namespace GRINS

#endif //BC_FACTORY_H
