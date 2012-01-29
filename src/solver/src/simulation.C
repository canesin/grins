//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// GRINS - General Reacting Incompressible Navier-Stokes 
//
// Copyright (C) 2010,2011 The PECOS Development Team
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

#include "simulation.h"

GRINS::Simulation::Simulation( const GetPot& input,
			       GRINS::PhysicsFactory* physics_factory,
			       GRINS::MeshBuilder* mesh_builder,
			       GRINS::SolverFactory* solver_factory,
			       GRINS::VisualizationFactory* vis_factory )
  :  _mesh( mesh_builder->build() ),
     _equation_system( new libMesh::EquationSystems( *_mesh ) ),
     _solver( solver_factory->build() ),
     _vis( vis_factory->build() ),
     _print_mesh_info( input("screen-options/print_mesh_info", false ) ),
     _print_log_info( input("screen-options/print_log_info", false ) ),
     _print_equation_system_info( input("screen-options/print_equation_system_info", false ) ),
     _output_vis( input("vis-options/output_vis", false ) ),
     _output_residual( input( "vis-options/output_residual", false ) )
{
  // Only print libMesh logging info if the user requests it
  libMesh::perflog.disable_logging();
  if( this->_print_log_info ) libMesh::perflog.enable_logging();

  GRINS::PhysicsList physics_list = physics_factory->build();

  _solver->initialize( input, _equation_system, physics_list );

  return;
}

GRINS::Simulation::~Simulation()
{
  return;
}

void GRINS::Simulation::run()
{
  this->print_sim_info();
  
  _solver->solve( _equation_system, _vis, _output_vis, _output_residual );

  return;
}

void GRINS::Simulation::print_sim_info()
{
  // Print mesh info if the user wants it
  if( this->_print_mesh_info ) this->_mesh->print_info();

   // Print info if requested
  if( this->_print_equation_system_info ) this->_equation_system->print_info();

  return;
}

std::tr1::shared_ptr<libMesh::EquationSystems> GRINS::Simulation::get_equation_system()
{
  return _equation_system;
}

void GRINS::Simulation::attach_dirichlet_bound_func( const std::string& physics_name, 
						     const GRINS::BoundaryID bc_id, 
						     const GRINS::VariableIndex var,
						     GRINS::DirichletFuncObj* bound_func )
{
  _solver->attach_dirichlet_bound_func( physics_name, bc_id, var, bound_func );
  return;
}

void GRINS::Simulation::attach_neumann_bound_func( const std::string& physics_name, 
						   const GRINS::BoundaryID bc_id, 
						   const GRINS::VariableIndex var,
						   GRINS::NeumannFuncObj* bound_func )
{
  _solver->attach_neumann_bound_func( physics_name, bc_id, var, bound_func );
  return;
}

#ifdef USE_GRVY_TIMERS
void GRINS::Simulation::attach_grvy_timer( GRVY::GRVY_Timer_Class* grvy_timer )
{
  _solver->attach_grvy_timer( grvy_timer );
  return;
}
#endif