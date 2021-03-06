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
    polyZoneRdf
    
Description
    computes the RDF from a user-specified cell for atomic constituents of molecules

    parallelised

   This looks like the class Konstantinos coded. [MB]
   Not fully tested - and if required to be used needs to be re-visited first.[MB]

SourceFiles
    polyZoneRdfI.H
    polyZoneRdf.C
    polyZoneRdfIO.C

\*---------------------------------------------------------------------------*/

#ifndef polyZoneRdf_H
#define polyZoneRdf_H

#include "polyField.H"
#include "vector.H"
#include "polyMesh.H"
#include "volFields.H"
#include "polyMoleculeCloud.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
    *                           Class polyZoneRdf Declaration
    * \*---------------------------------------------------------------------------*/

class polyZoneRdf
:
    public polyField
{

private:

    // Private data

        //- subDictionary containing the properties
        dictionary propsDict_;

        propertyField fields_;

        word regionName_;

        // 	word RregionName_;

        label regionId_;

        // 	label RregionId_;
        word fieldName_;

        //
        scalar rMax_;
        label nBins_;
        scalar binWidth_;

        label molIdA_;
        label molIdB_;

        scalarField rdf_;
        scalarField sumRdf_;
        scalarField magRadii_;
        scalarField binVolume_;
        scalar totalVolume_;
        scalar mols_;
        scalar nMols_;
        scalarField rdfAveraged_;

        scalar nAvTimeSteps_;
        bool resetAtOutput_;


        // Private Member Functions

        void evaluatePair
        (
            polyMolecule* molI,
            polyMolecule* molJ
        );

        //         void evaluatePair
        //         (
        //             waterMolecule* molReal,
        //             waterReferredMolecule* molRef
        //         );

        void evaluatePairS
        (
            polyMolecule* molReal,
            polyMolecule* molRef
        );

        void setRadii();


public:

    //- Runtime type information
    TypeName("polyZoneRdf");

    // Constructors

        //- Construct from components
        polyZoneRdf
        (
            Time& t,
            const polyMesh& mesh,
            polyMoleculeCloud& molCloud,
            const dictionary& dict
        );

        // Destructor

        ~polyZoneRdf();


        // Member Functions

        //- create initial field
        void createField();

        //- calculate field
        void calculateField();

        //- write field
        void writeField();

        void measureDuringForceComputation
        (
            polyMolecule* molI,
            polyMolecule* molJ
        );

        void measureDuringForceComputationSite
        (
            polyMolecule* molI,
            polyMolecule* molJ,
            label sI,
            label sJ
        );


        const propertyField& fields() const;

//         void updateProperties(const dictionary&);

};


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
