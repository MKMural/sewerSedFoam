word patchName = "bed";

label aPatchID = mesh.boundaryMesh().findPatchID(patchName); 

const polyPatch aPatch = mesh.boundaryMesh()[aPatchID]; 

pointMesh pMesh(mesh);

scalar icount = 0;

scalar PvaInit = 0;

pointScalarField pointDeltaH
    (
     IOobject
     (
       "pointDeltaH",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
     ),
       pMesh,
       dimensionedScalar("pointDeltaH", dimensionSet(0, 1, 0, 0, 0, 0, 0), 0.0),
       "zeroGradient"
     );

pointVectorField dispField
    (
     IOobject
     (
       "dispField",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
     ),
       pMesh,
       dimensionedVector("dispField", dimensionSet(0, 1, 0, 0, 0, 0, 0), vector::zero),
       "zeroGradient"
     );

pointVectorField newPoints
    (
     IOobject
     (
       "newPoints",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
     ),
       pMesh,
       dimensionedVector("newPoints", dimensionSet(0, 1, 0, 0, 0, 0, 0), vector::zero),
       "zeroGradient"
     );

pointVectorField newMeshPoints
    (
     IOobject
     (
       "newMeshPoints",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
     ),
       pMesh,
       dimensionedVector("newMeshPoints", dimensionSet(0, 1, 0, 0, 0, 0, 0), vector::zero),
       "zeroGradient"
     );

areaScalarField Pa
    (
      IOobject
      (
         "Pa",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pa", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Pc
    (
      IOobject
      (
         "Pc",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pc", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Pe
    (
      IOobject
      (
         "Pe",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pe", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Pva
    (
      IOobject
      (
         "Pva",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pva", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Pvc
    (
      IOobject
      (
         "Pvc",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pvc", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Pve
    (
      IOobject
      (
         "Pve",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Pve", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField tVol
    (
      IOobject
      (
         "tVol",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("tVol", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField volA
    (
      IOobject
      (
         "volA",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("volA", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField volC
    (
      IOobject
      (
         "volC",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("volC", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField volE
    (
      IOobject
      (
         "volE",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("volE", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField tVolA
    (
      IOobject
      (
         "tVolA",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("tVolA", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField tVolC
    (
      IOobject
      (
         "tVolC",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("tVolC", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField tVolE
    (
      IOobject
      (
         "tVolE",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("tVolE", dimensionSet(0, 3, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Dbed
    (
      IOobject
      (
         "Dbed",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Dbed", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );



areaScalarField n
    (
      IOobject
      (
         "n",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("n", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)	
    );


areaScalarField dd
    (
      IOobject
      (
         "dd",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("dd", dimensionSet(0, 1, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField ddmax
    (
      IOobject
      (
         "ddmax",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("ddmax", dimensionSet(0, 1, 0, 0, 0, 0, 0), 0)	
    );

areaScalarField Te
    (
      IOobject
      (
         "Te",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Te", dimensionSet(0, 0, 1, 0, 0, 0, 0), 0)	
    );

areaScalarField Tdep
    (
      IOobject
      (
         "Tdep",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Tdep", dimensionSet(0, 0, 1, 0, 0, 0, 0), 0)	
    );


areaScalarField Tb
    (
      IOobject
      (
         "Tb",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("Tb", dimensionSet(0, 0, 1, 0, 0, 0, 0), 0)	
    );

areaVectorField aMeshCentre
(
   IOobject
   (
     "aMeshCentre",
     runTime.timeName(), 
     aMesh.thisDb(),    
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector ("aMeshCentre", dimensionSet(0, 0, 0, 0, 0, 0, 0), vector::zero)
);

areaVectorField PointNormalVector
(
   IOobject
   (
     "PointNormalVector",
     runTime.timeName(), 
     aMesh.thisDb(),    
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector ("PointNormalVector", dimensionSet(0, 0, 0, 0, 0, 0, 0), vector::zero)
);

areaVectorField uNormal
(
   IOobject
   (
     "uNormal",
     runTime.timeName(), 
     aMesh.thisDb(),    
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector ("uNormal", dimensionSet(0, 0, 0, 0, 0, 0, 0), vector::zero)
);


areaVectorField aMeshCentreN
(
   IOobject
   (
     "aMeshCentreN",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector ("aMeshCentreN", dimensionSet(0, 0, 0, 0, 0, 0, 0), vector::zero)
);

areaVectorField q_bed
(
   IOobject
   (
     "q_bed",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh
);

volVectorField Ug
    (
      IOobject
      (
         "Ug",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("Ug", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

volVectorField Uw
    (
      IOobject
      (
         "Uw",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("Uw", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

volVectorField meshCentre
    (
      IOobject
      (
         "meshCentre",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("meshCentre", dimensionSet(0, 1, 0, 0, 0, 0, 0), vector::zero)	
    );

volVectorField meshCentreN
    (
      IOobject
      (
         "meshCentreN",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("meshCentreN", dimensionSet(0, 1, 0, 0, 0, 0, 0), vector::zero)	
    );

volVectorField Cgrad
    (
      IOobject
      (
         "Cgrad",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("Cgrad", dimensionSet(1, -4, 0, 0, 0, 0, 0), vector::zero)	
    );

volVectorField UtauV
    (
      IOobject
      (
         "UtauV",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("UtauV", dimensionSet(0, -1, -1, 0, 0, 0, 0), vector::zero)		
    );


volScalarField Ktemp
    (
      IOobject
      (
         "Ktemp",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedScalar ("Ktemp", dimensionSet(0, 2, -2, 0, 0, 0, 0), 0)	
    );

areaScalarField kTempA
    (
      IOobject
      (
         "kTempA",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("kTempA", dimensionSet(0, 2, -2, 0, 0, 0, 0), 0)	
    );

areaScalarField kTempC
    (
      IOobject
      (
         "kTempC",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("kTempC", dimensionSet(0, 2, -2, 0, 0, 0, 0), 0)	
    );

areaScalarField nuTempA
    (
      IOobject
      (
         "nuTempA",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedScalar ("nuTempA", dimensionSet(0, 2, -1, 0, 0, 0, 0), 0)	
    );

volScalarField nuTemp
    (
      IOobject
      (
         "nuTemp",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedScalar ("nuTemp", dimensionSet(0, 2, -1, 0, 0, 0, 0), 0)	
    );

areaVectorField Uga
    (
      IOobject
      (
         "Uga",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Uga", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Us
    (
      IOobject
      (
         "Us",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Us", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Utau
    (
      IOobject
      (
         "Utau",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Utau", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField UtauA
    (
      IOobject
      (
         "UtauA",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("UtauA", dimensionSet(0, -1, -1, 0, 0, 0, 0), vector::zero)	
    );

volVectorField Wg
    (
      IOobject
      (
         "Wg",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("Wg", dimensionSet(1, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField W
    (
      IOobject
      (
         "W",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("W", dimensionSet(1, 1, -2, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Wt
   (
      IOobject
      (
         "Wt",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Wt", dimensionSet(1, 1, -2, 0, 0, 0, 0), vector::zero)	
    );


areaVectorField Ub
    (
      IOobject
      (
         "Ub",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Ub", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Ub2
    (
      IOobject
      (
         "Ub2",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Ub2", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Unw
    (
      IOobject
      (
         "Unw",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Unw", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField aUf
    (
      IOobject
      (
         "aUf",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("aUf", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField Ur
    (
      IOobject
      (
         "Ur",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("Ur", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField x1
    (
      IOobject
      (
         "x1",
         runTime.timeName(),
         aMesh.thisDb(),
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	aMesh,
	dimensionedVector ("x1", dimensionSet(0, 1, -1, 0, 0, 0, 0), vector::zero)	
    );

areaVectorField f1
(  
   IOobject
   (
     "f1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector("f1",dimensionSet(0,1,-1,0,0,0,0), vector::zero) 
);

areaScalarField SSe1
(  
   IOobject
   (
     "SSe1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("SSe1",dimensionSet(0,1,-2,-1,0,0,0), 0) 
);

areaScalarField SSe2
(  
   IOobject
   (
     "SSe2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("SSe2",dimensionSet(0,1,-2,-1,0,0,0), 0) 
);

areaVectorField SSetemp
(  
   IOobject
   (
     "SSetemp",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedVector("SSetemp",dimensionSet(0,1,-1,0,0,0,0), vector::zero) 
);

areaScalarField eTemp
(  
   IOobject
   (
     "eTemp",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("eTemp",dimensionSet(0,2,-1,0,0,0,0), 0) 
);

areaScalarField SSd1
(  
   IOobject
   (
     "SSd1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("SSd1",dimensionSet(0,1,-2,-1,0,0,0), 0) 
);

areaScalarField SSd2
(  
   IOobject
   (
     "SSd2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("SSd2",dimensionSet(0,1,-2,-1,0,0,0), 0) 
);

areaScalarField SSdtemp
(  
   IOobject
   (
     "SSdtemp",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("SSd",dimensionSet(0,1,-2,-1,0,0,0), 0) 
);

areaScalarField deltaHs2
(  
   IOobject
   (
     "deltaHs2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh
);

areaScalarField deltaHs1
(  
   IOobject
   (
     "deltaHs1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh
);

areaScalarField deltaHs
(  
   IOobject
   (
     "deltaHs",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
      deltaHs1 + deltaHs2
);

areaScalarField deltaH
(  
   IOobject
   (
     "deltaH",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("deltaH",dimensionSet(0,1,0,0,0,0,0),0) 
);

areaScalarField deltaHInt
(  
   IOobject
   (
     "deltaHInt",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("deltaHInt",dimensionSet(0,1,0,0,0,0,0),0) 
);

areaScalarField deltaHb
(  
   IOobject
   (
     "deltaHb",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh
);

areaScalarField Uf
(  
   IOobject
   (
     "Uf",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Uf", dimensionSet(0, 1, -1, 0, 0, 0, 0), 0)
);

areaScalarField Uf2
(  
   IOobject
   (
     "Uf2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Uf2", dimensionSet(0, 1, -1, 0, 0, 0, 0), 0)
);

areaScalarField thetaB
(  
   IOobject
   (
     "thetaB",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaB", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField tau
(  
   IOobject
   (
     "tau",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("tau", dimensionSet(1, 1, 2, 0, 0, 0, 0), 0)
);

areaScalarField tauCB
(  
   IOobject
   (
     "tauCB",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("tauCB", dimensionSet(1, 1, 2, 0, 0, 0, 0), 0)
);

areaScalarField tauCSS1
(  
   IOobject
   (
     "tauCSS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("tauCSS1", dimensionSet(1, 1, 2, 0, 0, 0, 0), 0)
);

areaScalarField tauCSS2
(  
   IOobject
   (
     "tauCSS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("tauCSS2", dimensionSet(1, 1, 2, 0, 0, 0, 0), 0)
);

areaScalarField angAlpha
(  
   IOobject
   (
     "angAlpha",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("angAlpha", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField angBeta
(  
   IOobject
   (
     "angBeta",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("angBeta", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaCB
(  
   IOobject
   (
     "thetaCB",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaCB", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaCSS1
(  
   IOobject
   (
     "thetaCSS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaCSS1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaCSS2
(  
   IOobject
   (
     "thetaCSS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaCSS2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaBinit
(  
   IOobject
   (
     "thetaBinit",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaBinit", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaSS1init
(  
   IOobject
   (
     "thetaSS1init",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaSS1init", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaSS2init
(  
   IOobject
   (
     "thetaSS2init",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaSS2init", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaC1B
(  
   IOobject
   (
     "thetaC1B",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC1B", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaC1SS1
(  
   IOobject
   (
     "thetaC1SS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC1SS1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaC1SS2
(  
   IOobject
   (
     "thetaC1SS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC1SS2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);


areaScalarField thetaC2B
(  
   IOobject
   (
     "thetaC2B",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC2B", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaC2SS1
(  
   IOobject
   (
     "thetaC2SS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC2SS1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaC2SS2
(  
   IOobject
   (
     "thetaC2SS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaC2SS2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaSS1
(  
   IOobject
   (
     "thetaSS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaSS1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaSS2
(  
   IOobject
   (
     "thetaSS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaSS2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaHMaxB
(  
   IOobject
   (
     "thetaHMaxB",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaHMaxB", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaHMaxSS1
(  
   IOobject
   (
     "thetaHMaxSS1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaHMaxSS1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField thetaHMaxSS2
(  
   IOobject
   (
     "thetaHMaxSS2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("thetaHMaxSS2", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField Pef
(  
   IOobject
   (
     "Pef",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Pef", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField PefSS
(  
   IOobject
   (
     "PefSS",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("PefSS", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField Cs
(  
   IOobject
   (
     "Cs",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Cs", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField a
(  
   IOobject
   (
     "a",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("a", dimensionSet(1, -1, 0, 0, 0, 0, 0), 0)
);

areaScalarField b
(  
   IOobject
   (
     "b",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("b", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

edgeScalarField q_f
(  
   IOobject
   (
     "q_f",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("q_f", dimensionSet(0, 3, -1, 0, 0, 0, 0), 0)
);

areaScalarField xCo
(  
   IOobject
   (
     "xCo",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("xCo", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField yCo
(  
   IOobject
   (
     "yCo",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("yCo", dimensionSet(0, 1, 0, 0, 0, 0, 0), 0)
);

areaScalarField zCo
(  
   IOobject
   (
     "zCo",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("zCo", dimensionSet(0, 1, 0, 0, 0, 0, 0), 0)
);

areaScalarField Fi
(  
   IOobject
   (
     "Fi",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("Fi",dimensionSet(0,1,-1,0,0,0,0),0) 
);

Fi.oldTime();

areaScalarField Fi1
(  
   IOobject
   (
     "Fi1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("Fi1",dimensionSet(0,1,-1,0,0,0,0),0),
   Fi.boundaryField().types() 
);

Fi1.oldTime();

areaScalarField Fi2
(  
   IOobject
   (
     "Fi2",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("Fi2",dimensionSet(0,1,-1,0,0,0,0),0) 
);

areaScalarField hDot
(  
   IOobject
   (
     "hDot",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("hDot",dimensionSet(0,0,0,0,0,0,0),0) 
);

areaScalarField hDot1
(  
   IOobject
   (
     "hDot1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar("hDot1",dimensionSet(0,0,0,0,0,0,0),0) 
);

areaScalarField Ce1
(
    IOobject
    (
       	 "Ce1",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("Ce1", C.dimensions(), 0)
);

areaScalarField Ce2
(
    IOobject
    (
       	 "Ce2",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("Ce2", C.dimensions(), 0)
);


areaScalarField Cd
(
    IOobject
    (
       	 "Cd",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("Cd", C.dimensions(), 0)
);

areaScalarField Cpre
(
    IOobject
    (
       	 "Cpre",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("Cpre", C.dimensions(), 0)
);

areaScalarField Cpost
(
    IOobject
    (
       	 "Cpost",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("Cpost", C.dimensions(), 0)
);

areaScalarField lambdaB
(
    IOobject
    (
       	 "lambdaB",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("lambdaB",dimensionSet(0,0,0,0,0,0,0),0)
);

    volScalarField zDim
    (
      IOobject
      (
         "zDim",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
       mesh,
       dimensionedScalar("zDim",dimensionSet(0,1,0,0,0,0,0),0)
    );

areaScalarField zDimA
(
    IOobject
    (
       	 "zDimA",
       	 runTime.timeName(),
	 aMesh.thisDb(),
       	 IOobject::READ_IF_PRESENT,
       	 IOobject::AUTO_WRITE
     ),
     aMesh,
     dimensionedScalar("zDimA",dimensionSet(0,1,0,0,0,0,0),0)
);

areaScalarField totH
(  
   IOobject
   (
     "totH",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("totH", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

totH.oldTime();

areaScalarField totH1
(  
   IOobject
   (
     "totH1",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("totH1", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0),
   totH.boundaryField().types() 
);

areaScalarField He
(  
   IOobject
   (
     "He",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("He", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField Hdep
(  
   IOobject
   (
     "Hdep",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Hdep", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);

areaScalarField Href
(  
   IOobject
   (
     "Href",
     runTime.timeName(),
     aMesh.thisDb(),
     IOobject::READ_IF_PRESENT,
     IOobject::AUTO_WRITE
   ),
   aMesh,
   dimensionedScalar ("Href", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0)
);
