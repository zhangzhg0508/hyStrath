/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016-2021 hyStrath
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of hyStrath, a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::pairPotentials::electrostatic

Description


SourceFiles
    reactionField.C

\*---------------------------------------------------------------------------*/

#ifndef reactionField_H
#define reactionField_H

#include "pairPotential.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace pairPotentials
{

/*---------------------------------------------------------------------------*\
                         Class reactionField Declaration
\*---------------------------------------------------------------------------*/

class reactionField
:
    public pairPotential
{
    // Private data

        dictionary reactionFieldCoeffs_;

        // dielectric constant is dimensionless
        scalar dielectricConst_;

        scalar oneOverFourPiEps0_;

public:

    //- Runtime type information
    TypeName("reactionField");


    // Static data members

//         static scalar oneOverFourPiEps0;


    // Constructors

        //- Construct from components
        reactionField
        (
            const word& name,
            const reducedUnits& rU,
            const dictionary& pairPotentialProperties
        );


    // Destructor

        ~reactionField()
        {}


    // Member Functions

        scalar unscaledEnergy(const scalar r) const;

        const dictionary& dict() const;

        //- Read dictionary
        bool read
        (
            const dictionary& pairPotentialProperties,
            const reducedUnits& rU
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace pairPotentials
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
