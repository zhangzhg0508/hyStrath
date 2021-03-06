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

Description

\*---------------------------------------------------------------------------*/

#include "polyPlaceMoleculesFromFile.H"
#include "addToRunTimeSelectionTable.H"
#include "IFstream.H"
#include "graph.H"
#include "IFstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTypeNameAndDebug(polyPlaceMoleculesFromFile, 0);

addToRunTimeSelectionTable(polyConfiguration, polyPlaceMoleculesFromFile, dictionary);



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
polyPlaceMoleculesFromFile::polyPlaceMoleculesFromFile
(
    polyMoleculeCloud& molCloud,
    const dictionary& dict
//     const word& name
)
:
    polyConfiguration(molCloud, dict/*, name*/)
//     propsDict_(dict.subDict(typeName + "Properties"))
{




    option_ = "fixedPropertiesFromDict";

    if (mdInitialiseDict_.found("option"))
    {
        const word option(mdInitialiseDict_.lookup("option"));
        option_ = option;
    }
}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

polyPlaceMoleculesFromFile::~polyPlaceMoleculesFromFile()
{}



// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void polyPlaceMoleculesFromFile::setInitialConfiguration()
{
    label initialSize = molCloud_.size();

    if(option_ == "fixedPropertiesFromDict")
    {
        fixedPropertiesFromDict();
    }

    if(option_ == "fixedPropertiesFromFile")
    {
        fixedPropertiesFromFile();
    }

    if(option_ == "multiIdFixedProperties")
    {
        multiIdFixedProperties();
    }

    label finalSize = molCloud_.size();

    nMolsAdded_ = finalSize - initialSize;

    if (Pstream::parRun())
    {
        reduce(nMolsAdded_, sumOp<label>());
    }

    Info << tab << " molecules added: " << nMolsAdded_ << endl;
}

void polyPlaceMoleculesFromFile::fixedPropertiesFromDict()
{

    List<vector> molPoints = List<vector>(mdInitialiseDict_.lookup("molPoints"));

//     Info << "molPoints" << molPoints << endl;

    label nMols = molPoints.size();

    List<label> molIds(nMols, 0);
    List<bool> tetheredMols(nMols, false);
    List<bool> frozenMols(nMols, false);
//     List<scalar> temperatureMols(nMols, 0.0);
    List<vector> velocityMols(nMols, vector::zero);
    List<scalar> phiMols(nMols, 0.0);
    List<scalar> thetaMols(nMols, 0.0);
    List<scalar> psiMols(nMols, 0.0);


    word molIdName(mdInitialiseDict_.lookup("molId"));

    const List<word>& idList(molCloud_.cP().molIds());

    label molId = findIndex(idList, molIdName);

    if(molId == -1)
    {
        FatalErrorIn("polyPlaceMoleculesFromFile::setInitialConfiguration()")
            << "Cannot find molecule id: " << molIdName
            << nl << "in moleculeProperties/idList."
            << exit(FatalError);
    }

//         const scalar T(readScalar(mdInitialiseDict_.lookup("temperature")));
    const vector U(mdInitialiseDict_.lookup("velocity"));

    bool frozen = false;

    if (mdInitialiseDict_.found("frozen"))
    {
        frozen = Switch(mdInitialiseDict_.lookup("frozen"));
    }

    bool tethered = false;

    if (mdInitialiseDict_.found("tethered"))
    {
        tethered = Switch(mdInitialiseDict_.lookup("tethered"));
    }

    scalar phi = 0.0;

    if (mdInitialiseDict_.found("phi"))
    {
        phi = readScalar(mdInitialiseDict_.lookup("phi"));
    }

    scalar theta = 0.0;

    if (mdInitialiseDict_.found("theta"))
    {
        theta = readScalar(mdInitialiseDict_.lookup("theta"));
    }

    scalar psi = 0.0;

    if (mdInitialiseDict_.found("psi"))
    {
        psi = readScalar(mdInitialiseDict_.lookup("psi"));
    }

    forAll(molIds, i)
    {
        molIds[i] = molId;
        tetheredMols[i] = tethered;
        frozenMols[i] = frozen;
//             temperatureMols[i] = T;
        velocityMols[i] = U;

        phiMols[i] = phi*constant::mathematical::pi/180.0;
        thetaMols[i] = theta*constant::mathematical::pi/180.0;
        psiMols[i] = psi*constant::mathematical::pi/180.0;
    }

    forAll(molPoints, i)
    {
        const vector& globalPosition = molPoints[i];

        label cell = -1;
        label tetFace = -1;
        label tetPt = -1;

        mesh_.findCellFacePt
        (
            globalPosition,
            cell,
            tetFace,
            tetPt
        );

        if(cell != -1)
        {
            insertMolecule
            (
                globalPosition,
                cell,
                tetFace,
                tetPt,
                molIds[i],
                tetheredMols[i],
                frozenMols[i],
                phiMols[i],
                thetaMols[i],
                psiMols[i],
//                 temperatureMols[i],
                velocityMols[i]
            );
        }
        else
        {
            FatalErrorIn("Foam::polyPlaceMoleculesFromFile::setInitialConfiguration()")
                << "Molecule position: " << globalPosition
                << " is not located in the mesh." << nl
                << abort(FatalError);
        }
    }
}


// fixed positions but of a constant molecule id
void polyPlaceMoleculesFromFile::fixedPropertiesFromFile()
{
    word name(mdInitialiseDict_.lookup("positionsFileName"));

    fileName timePath = molCloud_.mesh().time().system()/name;

    IFstream file(timePath);

    List< vector > molPoints;

    if (file.good())
    {
        file >> molPoints;
    }
    else
    {
        FatalErrorIn
        (
            "void polyPlaceMoleculesFromFile::setInitialConfiguration()"
        )
            << "Cannot open file " << file.name()
            << abort(FatalError);
    }

//     Info << "molPoints" << molPoints << endl;

    label nMols = molPoints.size();

    List<label> molIds(nMols, 0);
    List<bool> tetheredMols(nMols, false);
    List<bool> frozenMols(nMols, false);
//     List<scalar> temperatureMols(nMols, 0.0);
    List<vector> velocityMols(nMols, vector::zero);
    List<scalar> phiMols(nMols, 0.0);
    List<scalar> thetaMols(nMols, 0.0);
    List<scalar> psiMols(nMols, 0.0);


    word molIdName(mdInitialiseDict_.lookup("molId"));

    const List<word>& idList(molCloud_.cP().molIds());

    label molId = findIndex(idList, molIdName);

    if(molId == -1)
    {
        FatalErrorIn("polyPlaceMoleculesFromFile::setInitialConfiguration()")
            << "Cannot find molecule id: " << molIdName
            << nl << "in moleculeProperties/idList."
            << exit(FatalError);
    }

    const scalar temperature(readScalar(mdInitialiseDict_.lookup("temperature")));
    const vector velocity(mdInitialiseDict_.lookup("velocity"));

    bool fixedVelocity = false;

    if (mdInitialiseDict_.found("fixedVelocity"))
    {
        fixedVelocity = Switch(mdInitialiseDict_.lookup("fixedVelocity"));
    }

    bool frozen = false;

    if (mdInitialiseDict_.found("frozen"))
    {
        frozen = Switch(mdInitialiseDict_.lookup("frozen"));
    }

    bool tethered = false;

    if (mdInitialiseDict_.found("tethered"))
    {
        tethered = Switch(mdInitialiseDict_.lookup("tethered"));
    }

    scalar phi = 0.0;

    if (mdInitialiseDict_.found("phi"))
    {
        phi = readScalar(mdInitialiseDict_.lookup("phi"));
    }

    scalar theta = 0.0;

    if (mdInitialiseDict_.found("theta"))
    {
        theta = readScalar(mdInitialiseDict_.lookup("theta"));
    }

    scalar psi = 0.0;

    if (mdInitialiseDict_.found("psi"))
    {
        psi = readScalar(mdInitialiseDict_.lookup("psi"));
    }

    forAll(molIds, i)
    {
        molIds[i] = molId;
        tetheredMols[i] = tethered;
        frozenMols[i] = frozen;
//             temperatureMols[i] = T;

        vector U = vector::zero;

        if(fixedVelocity)
        {
            U = velocity;
        }
        else
        {
            U = equipartitionLinearVelocity(temperature, molCloud_.cP().mass(molId));
            U += velocity;
        }

        velocityMols[i] = U;

        phiMols[i] = phi*constant::mathematical::pi/180.0;
        thetaMols[i] = theta*constant::mathematical::pi/180.0;
        psiMols[i] = psi*constant::mathematical::pi/180.0;
    }

    forAll(molPoints, i)
    {
        const vector& globalPosition = molPoints[i];

        label cell = -1;
        label tetFace = -1;
        label tetPt = -1;

        mesh_.findCellFacePt
        (
            globalPosition,
            cell,
            tetFace,
            tetPt
        );



        if(cell != -1)
        {
            insertMolecule
            (
                globalPosition,
                cell,
                tetFace,
                tetPt,
                molIds[i],
                tetheredMols[i],
                frozenMols[i],
                phiMols[i],
                thetaMols[i],
                psiMols[i],
//                 temperatureMols[i],
                velocityMols[i]
            );
        }
        else
        {
            FatalErrorIn("Foam::polyPlaceMoleculesFromFile::setInitialConfiguration()")
                << "Molecule position: " << globalPosition
                << " is not located in the mesh." << nl
                << abort(FatalError);
        }
    }
}

// fixed positions but of a constant molecule id
void polyPlaceMoleculesFromFile::multiIdFixedProperties()
{
    List< vector > molPoints;
    List< word > molIdWords;

    {
        word name(mdInitialiseDict_.lookup("positionsFileName"));

        fileName timePath = molCloud_.mesh().time().system()/name;

        IFstream file(timePath);

        if (file.good())
        {
            file >> molPoints;
        }
        else
        {
            FatalErrorIn
            (
                "void polyPlaceMoleculesFromFile::setInitialConfiguration()"
            )
                << "Cannot open file " << file.name()
                << abort(FatalError);
        }
    }

    {
        word name(mdInitialiseDict_.lookup("molIdsFileName"));

        fileName timePath = molCloud_.mesh().time().system()/name;

        IFstream file(timePath);

        if (file.good())
        {
            file >> molIdWords;
        }
        else
        {
            FatalErrorIn
            (
                "void polyPlaceMoleculesFromFile::setInitialConfiguration()"
            )
                << "Cannot open file " << file.name()
                << abort(FatalError);
        }
    }

    if(molPoints.size() != molIdWords.size())
    {
        FatalErrorIn
        (
            "void polyPlaceMoleculesFromFile::setInitialConfiguration()"
        )
            << "MolPoints not the same size as molIdWords in your submitted files"
            << molPoints.size()
            << " vs " << molIdWords.size()
            << abort(FatalError);
    }


    label nMols = molPoints.size();

    List<label> molIds(nMols, 0);
    List<bool> tetheredMols(nMols, false);
    List<bool> frozenMols(nMols, false);
//     List<scalar> temperatureMols(nMols, 0.0);
    List<vector> velocityMols(nMols, vector::zero);
    List<scalar> phiMols(nMols, 0.0);
    List<scalar> thetaMols(nMols, 0.0);
    List<scalar> psiMols(nMols, 0.0);

//         const scalar T(readScalar(mdInitialiseDict_.lookup("temperature")));
    const vector U(mdInitialiseDict_.lookup("velocity"));

    bool frozen = false;

    if (mdInitialiseDict_.found("frozen"))
    {
        frozen = Switch(mdInitialiseDict_.lookup("frozen"));
    }

    vector shift = vector::zero;

    if (mdInitialiseDict_.found("shift"))
    {
        shift = mdInitialiseDict_.lookup("shift");
    }

    bool tethered = false;

    if (mdInitialiseDict_.found("tethered"))
    {
        tethered = Switch(mdInitialiseDict_.lookup("tethered"));
    }

    scalar phi = 0.0;

    if (mdInitialiseDict_.found("phi"))
    {
        phi = readScalar(mdInitialiseDict_.lookup("phi"));
    }

    scalar theta = 0.0;

    if (mdInitialiseDict_.found("theta"))
    {
        theta = readScalar(mdInitialiseDict_.lookup("theta"));
    }

    scalar psi = 0.0;

    if (mdInitialiseDict_.found("psi"))
    {
        psi = readScalar(mdInitialiseDict_.lookup("psi"));
    }

    const List<word>& idList(molCloud_.cP().molIds());

    forAll(molIds, i)
    {
        label molId = findIndex(idList, molIdWords[i]);

        if(molId == -1)
        {
            FatalErrorIn("polyPlaceMoleculesFromFile::setInitialConfiguration()")
                << "Cannot find molecule id: " << molIdWords[i]
                << nl << "in moleculeProperties/idList."
                << exit(FatalError);
        }

        molIds[i]=molId;

        tetheredMols[i] = tethered;
        frozenMols[i] = frozen;
//             temperatureMols[i] = T;
        velocityMols[i] = U;

        phiMols[i] = phi*constant::mathematical::pi/180.0;
        thetaMols[i] = theta*constant::mathematical::pi/180.0;
        psiMols[i] = psi*constant::mathematical::pi/180.0;
    }


    forAll(molPoints, i)
    {
        const vector& globalPosition = molPoints[i];

        label cell = -1;
        label tetFace = -1;
        label tetPt = -1;

        mesh_.findCellFacePt
        (
            globalPosition + shift,
            cell,
            tetFace,
            tetPt
        );


        if(cell != -1)
        {
            insertMolecule
            (
                globalPosition + shift,
                cell,
                tetFace,
                tetPt,
                molIds[i],
                tetheredMols[i],
                frozenMols[i],
                phiMols[i],
                thetaMols[i],
                psiMols[i],
//                 temperatureMols[i],
                velocityMols[i]
            );
        }
//         else
//         {
//             FatalErrorIn("Foam::polyPlaceMoleculesFromFile::setInitialConfiguration()")
//                 << "Molecule position: " << globalPosition
//                 << " is not located in the mesh." << nl
//                 << abort(FatalError);
//         }
    }
}


void polyPlaceMoleculesFromFile::insertMolecule
(
    const point& position,
    const label& cell,
    const label& tetFace,
    const label& tetPt,
    const label& id,
    const bool& tethered,
    const bool& frozen,
    const scalar& phi,
    const scalar& theta,
    const scalar& psi,
//     const scalar& temperature,
    const vector& velocity
)
{
    point specialPosition(vector::zero);

    label special = 0;

    if (tethered)
    {
        specialPosition = position;

        special = polyMolecule::SPECIAL_TETHERED;
    }

    if (frozen)//****
    {
        specialPosition = position;

        special = polyMolecule::SPECIAL_FROZEN;
    }

//     const polyMolecule::constantProperties& cP = molCloud_.constProps(id);

//     vector v = equipartitionLinearVelocity(temperature, cP.mass());
//
//     v += bulkVelocity;

    vector pi = vector::zero;

    tensor Q = I;

//     scalar phi = 0.5*constant::mathematical::pi;
//     scalar theta = 0.0;
//     scalar psi = 0.0;

/*
    scalar phi(rndGen_.sample01<scalar>()*constant::mathematical::twoPi);

    scalar theta(rndGen_.sample01<scalar>()*constant::mathematical::twoPi);

    scalar psi(rndGen_.sample01<scalar>()*constant::mathematical::twoPi);*/

//     Info << "phi:" << phi << endl;

    Q = tensor
    (
        cos(psi)*cos(phi) - cos(theta)*sin(phi)*sin(psi),
        cos(psi)*sin(phi) + cos(theta)*cos(phi)*sin(psi),
        sin(psi)*sin(theta),
        - sin(psi)*cos(phi) - cos(theta)*sin(phi)*cos(psi),
        - sin(psi)*sin(phi) + cos(theta)*cos(phi)*cos(psi),
        cos(psi)*sin(theta),
        sin(theta)*sin(phi),
        - sin(theta)*cos(phi),
        cos(theta)
    );

    molCloud_.createMolecule
    (
        position,
        cell,
        tetFace,
        tetPt,
        Q,
        velocity,
        vector::zero,
        pi,
        vector::zero,
        specialPosition,
        special,
        id,
        1.0,
        molCloud_.getTrackingNumber()
    );
}

} // End namespace Foam

// ************************************************************************* //
