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
    Foam::VTRelaxationModels::MillikanWhitePark

Description
    Millikan-White-Park V-T Relaxation model.

SourceFiles
    MillikanWhitePark.C

\*---------------------------------------------------------------------------*/

#ifndef MillikanWhitePark_H
#define MillikanWhitePark_H

#include "VTRelaxationModel.H"
#include "dimensionedScalar.H"
#include "volFields.H"

#include "constants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace VTRelaxationModels
{

/*---------------------------------------------------------------------------*\
                      Class MillikanWhitePark Declaration
\*---------------------------------------------------------------------------*/

class MillikanWhitePark
:
    public VTRelaxationModel
{
    // Private data

        //- Species indices
        label species1_, species2_;

        //- Millikan and White coefficients
        scalar A12_, B12_;

        //- Molecular weight [g/mol] of the molecular species
        scalar W1_;

        //- Offset term in the exponential
        scalar offset_;

        //- Coefficients related to Park's correction
        scalar sigma1_, sigma2_;


public:

    //- Runtime type information
    TypeName("MillikanWhitePark");


    // Constructors

        //- construct from components
        MillikanWhitePark
        (
            const word& name1,
            const word& name2,
            const label& lname1,
            const label& lname2,
            const dictionary& dict2T,
            const dictionary& dictThermoPhy,
            const volScalarField& p,
            const volScalarField& T,
            const PtrList<volScalarField>& Tv,
            const PtrList<volScalarField>& nD
        );


    // Destructor

        ~MillikanWhitePark()
        {}


    // Member Functions

        //- V-T relaxation time [s]
        virtual tmp<volScalarField> tauVT() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace VTRelaxationModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
