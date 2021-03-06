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


Description

\*---------------------------------------------------------------------------*/

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// inline void trackingInfo::setUf(const vector& Uf)
// {
//     Uf_ = Uf;
// }



//- Access
// inline const labelField& atomisticFaceTracker::crossedMols() const
// {
//     return crossedMols_;
// }
//
// inline const scalarField& atomisticFaceTracker::netMassFlux() const
// {
//     return netMassFlux_;
// }
//
// inline const vectorField& atomisticFaceTracker::netMomFlux() const
// {
//     return netMomFlux_;
// }
//
// inline const vectorField& atomisticFaceTracker::forceFlux() const
// {
//     return forceFlux_;
// }
//
// inline const scalarField& atomisticFaceTracker::pEFlux() const
// {
//     return pEFlux_;
// }
//
// inline const scalarField& atomisticFaceTracker::kEFlux() const
// {
//     return kEFlux_;
// }
//
// inline const scalarField& atomisticFaceTracker::energyFlux() const
// {
//     return energyFlux_;
// }

inline const List<scalarField>& pdFaceTracker::parcelIdFlux() const
{
    return parcelIdFlux_;
}


inline const List<scalarField>& pdFaceTracker::massIdFlux() const
{
    return massIdFlux_;
}




// inline const surfaceScalarField& atomisticFaceTracker::totMolFlux() const
// {
//     return totMolFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::molFlux() const
// {
//     return molFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::massFlux() const
// {
//     return massFlux_;
// }
//
// inline const surfaceVectorField& atomisticFaceTracker::momFlux() const
// {
//     return momFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::pEFlux() const
// {
//     return pEFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::kEFlux() const
// {
//     return kEFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::energyFlux() const
// {
//     return energyFlux_;
// }
//
// inline const surfaceScalarField& atomisticFaceTracker::volumeFlux() const
// {
//     return volumeFlux_;
// }
//
// inline const surfaceVectorField& atomisticFaceTracker::forceFlux() const
// {
//     return forceFlux_;
// }

// inline const List<scalarField>& atomisticFaceTracker::idFlux() const
// {
//     return idFlux_;
// }


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

// inline bool operator==
// (
//     const referredCell& a,
//     const referredCell& b
// )
// {
//     return const_cast<referredCell&>(a).duplicate
//     (
//         const_cast<const referredCell&>(b)
//     );
// }
//
//
// inline bool operator!=
// (
//     const referredCell& a,
//     const referredCell& b
// )
// {
//     return !(a == b);
// }

// * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
