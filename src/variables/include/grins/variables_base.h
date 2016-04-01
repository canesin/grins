//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// GRINS - General Reacting Incompressible Navier-Stokes
//
// Copyright (C) 2014-2015 Paul T. Bauman, Roy H. Stogner
// Copyright (C) 2010-2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-


#ifndef GRINS_VARIABLES_BASE_H
#define GRINS_VARIABLES_BASE_H

// C++
#include <vector>
#include <string>

// GRINS
#include "grins/var_typedefs.h"

// libMesh forward declarations
class GetPot;
namespace libMesh
{
  class FEMSystem;
}

namespace GRINS
{
  class VariablesBase
  {
  public:

    VariablesBase(){};

    virtual ~VariablesBase(){};

    //! Read, not add, variable numbers from the system
    /*! This method assumes that the variable names were setup
        during construction. Then, we'll just grab the variable
        number based on the variable name. */
    virtual void init_vars( libMesh::FEMSystem* system )
    { this->default_var_init(system); }

    //! Return the var names that are active from this class
    /*! This must not be called until init_vars has been called. */
    const std::vector<std::string>& active_var_names() const
    { return _var_names; }

  protected:

    //! Default method for init'ing variables
    /*! This method assumes that the variable has already been
        added to the System elsewhere and we're just grabbing the
        variable number from the system. We attempt to grab
        a variable number for every entry in _var_names. */
    void default_var_init( libMesh::FEMSystem* system );

    //! Method to parse variable names from input
    /*! Names parsed from: [Variables/<subsection>/names] and then
        populated into the supplied var_names vector. It is assumed
        that var_names has been properly sized, that default_names
        and var_names have the same size, and that default_names has
        been populated with unique strings. */
    void parse_names_from_input( const GetPot& input,
                                 const std::string& subsection,
                                 std::vector<std::string>& var_names,
                                 const std::vector<std::string>& default_names );

    //! Check for old name style and new name style. If both present, error.
    /*! Old name style: [Physics/VariableNames]
        New name style: [Variables/<variable type>]
        Here, we just check for the presence of the sections [Physics/VariableNames]
        and [Variables]. */
    void duplicate_name_section_check( const GetPot& input ) const;

    //! Check for deprecated variable name input style
    /*! If found, this returns true and emits a deprecated warning.
        Otherwise, this returns false.
        The string argument is supplied by each variable
        class for the warning message. E.g. if the variable class
        is going to look in "Displacement", i.e.
        [Variables/Displacement/names], then "Displacement" should be
        passed. */
    bool check_dep_name_input( const GetPot& input,
                               const std::string& new_subsection ) const;

    std::vector<VariableIndex> _vars;

    std::vector<std::string> _var_names;

  };

} // end namespace GRINS

#endif // GRINS_VARIABLES_BASE_H