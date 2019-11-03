    Info<< "Reading field pd\n" << endl;
    volScalarField pd
    (
        IOobject
        (
            "pd",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field alpha1\n" << endl;
    volScalarField alpha1
    (
        IOobject
        (
            "alpha1",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading transportProperties\n" << endl;
    IOdictionary transportProperties
    (
	IOobject
	(	
	"transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
        )
    );

    IOdictionary sedimentProperties
    (
	IOobject
	(	
	"sedimentProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
        )
    );

    dimensionedScalar nu1
    (	
     sedimentProperties.lookup("nu1")
    );

    dimensionedScalar nu2
    (	
     sedimentProperties.lookup("nu2")
    );

    dimensionedScalar rhof
    (	
     sedimentProperties.lookup("rhof")
    );

    dimensionedScalar mud
    (	
     sedimentProperties.lookup("mud")
    );

    dimensionedScalar mus
    (	
     sedimentProperties.lookup("mus")
    );

    dimensionedScalar BLa
    (	
     sedimentProperties.lookup("BLa")
    );

    dimensionedScalar Ubiterations
    (	
     sedimentProperties.lookup("Ubiterations")
    );

    dimensionedScalar xsens
    (	
     sedimentProperties.lookup("xsens")
    );

    dimensionedScalar yzsens
    (	
     sedimentProperties.lookup("yzsens")
    );

    dimensionedScalar xMin
    (	
     sedimentProperties.lookup("xMin")
    );

    dimensionedScalar xMax
    (	
     sedimentProperties.lookup("xMax")
    );

    dimensionedScalar UxMin
    (	
     sedimentProperties.lookup("UxMin")
    );

    dimensionedScalar UxMax
    (	
     sedimentProperties.lookup("UxMax")
    );

    dimensionedScalar erosion
    (	
     sedimentProperties.lookup("erosion")
    );

    dimensionedScalar Db
    (	
     sedimentProperties.lookup("Db")
    );

    dimensionedScalar Dss1
    (	
     sedimentProperties.lookup("Dss1")
    );

    dimensionedScalar Dss2
    (	
     sedimentProperties.lookup("Dss2")
    );

    dimensionedScalar rhoB
    (	
     sedimentProperties.lookup("rhoB")
    );

    dimensionedScalar rhoSS1
    (	
     sedimentProperties.lookup("rhoSS1")
    );

    dimensionedScalar rhoSS2
    (	
     sedimentProperties.lookup("rhoSS2")
    );

    dimensionedScalar Pa0
    (	
     sedimentProperties.lookup("Pa")
    );

    dimensionedScalar Pc0
    (	
     sedimentProperties.lookup("Pc")
    );

    dimensionedScalar Pe0
    (	
     sedimentProperties.lookup("Pe")
    );

    dimensionedScalar DyP
    (	
     sedimentProperties.lookup("DyP")
    );

    dimensionedScalar n0
    (	
     sedimentProperties.lookup("n")
    );

    dimensionedScalar dd0
    (	
     sedimentProperties.lookup("dd0")
    );

    dimensionedScalar Femp
    (	
     sedimentProperties.lookup("Femp")
    );

    dimensionedScalar H0
    (	
     sedimentProperties.lookup("H0")
    );

    dimensionedScalar Hmax
    (	
     sedimentProperties.lookup("Hmax")
    );

    dimensionedScalar Href1
    (	
     sedimentProperties.lookup("Href1")
    );

    dimensionedScalar TdepSP
    (	
     sedimentProperties.lookup("TdepSP")
    );

    dimensionedScalar morphTimeScale
    (	
     sedimentProperties.lookup("morphTimeScale")
    );

    dimensionedScalar bedSS
    (	
     sedimentProperties.lookup("bedSS")
    );

#   include "createPhi.H"


    Info<< "Reading transportProperties\n" << endl;
    twoPhaseMixture twoPhaseProperties(U, phi, "alpha1");

    const dimensionedScalar& rho1 = twoPhaseProperties.rho1();
    const dimensionedScalar& rho2 = twoPhaseProperties.rho2();


    // Need to store rho for ddt(rho, U)
    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT
        ),
        alpha1*rho1 + (scalar(1) - alpha1)*rho2,
        alpha1.boundaryField().types()
    );
    rho.oldTime();


    // Mass flux
    // Initialisation does not matter because rhoPhi is reset after the
    // alpha1 solution before it is used in the U equation.
    surfaceScalarField rhoPhi
    (
        IOobject
        (
            "rho*phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rho1*phi
    );

    volVectorField WsSS1
    (
      IOobject
      (
         "WsSS1",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
        dimensionedVector("0.0", U.dimensions(), vector::zero)
    );

    volVectorField WsSS2
    (
      IOobject
      (
         "WsSS2",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
        dimensionedVector("0.0", U.dimensions(), vector::zero)
    );

	volScalarField C1
	(
	    IOobject
	    (
        	 "C1",
        	 runTime.timeName(),
        	 mesh,
        	 IOobject::READ_IF_PRESENT,
        	 IOobject::AUTO_WRITE
	     ),
	     mesh,
       dimensionedScalar("C1",dimensionSet(1,3,0,0,0,0,0),0)
	);

	volScalarField C2
	(
	    IOobject
	    (
        	 "C2",
        	 runTime.timeName(),
        	 mesh,
        	 IOobject::READ_IF_PRESENT,
        	 IOobject::AUTO_WRITE
	     ),
	     mesh,
       dimensionedScalar("C2",dimensionSet(1,3,0,0,0,0,0),0)
	);

	volScalarField C
	(
	    IOobject
	    (
        	 "C",
        	 runTime.timeName(),
        	 mesh,
        	 IOobject::READ_IF_PRESENT,
        	 IOobject::AUTO_WRITE
	     ),
       (C1+C2)

	);

    surfaceScalarField AdSS1
    (
      IOobject
      (
         "AdSS1",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
    (phi + (mesh.Sf() & fvc::interpolate(WsSS1)))
    );

    surfaceScalarField AdSS2
    (
      IOobject
      (
         "AdSS2",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
    (phi + (mesh.Sf() & fvc::interpolate(WsSS2)))
    );

    volScalarField DiSS
    (
      IOobject
      (
         "DiSS",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
       mesh,
       dimensionedScalar("DiSS",dimensionSet(0,2,-1,0,0,0,0),0)
    );

    volScalarField DiSSt
    (
      IOobject
      (
         "DiSSt",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
        mesh,
        dimensionedScalar("DiSSt",dimensionSet(0,2,-1,0,0,0,0),0) 
    );

    volScalarField Umag
    (
      IOobject
      (
         "Umag",
         runTime.timeName(),
         mesh,
         IOobject::NO_READ,
         IOobject::AUTO_WRITE
      ),
       mesh,
       dimensionedScalar("Umag",dimensionSet(0,1,-1,0,0,0,0),0)
    );



    volScalarField zMul
    (
      IOobject
      (
         "zMul",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
        mesh,
        dimensionedScalar("zMul",dimensionSet(0,0,0,0,0,0,0),0) 
    );

    volScalarField deltaHF
    (
      IOobject
      (
         "deltaHF",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
       mesh,
       dimensionedScalar("deltaHF",dimensionSet(0,1,0,0,0,0,0),0) 
    );

    volScalarField testS
    (
      IOobject
      (
         "testS",
         runTime.timeName(),
         mesh,
         IOobject::NO_READ,
         IOobject::AUTO_WRITE
      ),
       mesh,
       dimensionedScalar("testS",dimensionSet(0,0,0,0,0,0,0),0) 
    );


    volVectorField q_b_slide
    (
      IOobject
      (
         "q_b_slide",
         runTime.timeName(),
         mesh,
         IOobject::READ_IF_PRESENT,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("q_b_slide", dimensionSet(0, 2, -1, 0, 0, 0, 0), vector::zero)
    );

    volVectorField testV
    (
      IOobject
      (
         "testV",
         runTime.timeName(),
         mesh,
         IOobject::NO_READ,
         IOobject::AUTO_WRITE
      ),
	mesh,
	dimensionedVector ("testV", dimensionSet(0, 0, 0, 0, 0, 0, 0), vector::zero)
    );

    // Construct interface from alpha1 distribution
    interfaceProperties interface(alpha1, U, twoPhaseProperties);

    // Construct incompressible turbulence model
    autoPtr<incompressible::turbulenceModel> turbulence
    (
        incompressible::turbulenceModel::New(U, phi, twoPhaseProperties)
    );

    wordList pcorrTypes
    (
        pd.boundaryField().size(),
        zeroGradientFvPatchScalarField::typeName
    );

    for (label i = 0; i < pd.boundaryField().size(); i++)
    {
        if (pd.boundaryField()[i].fixesValue())
        {
            pcorrTypes[i] = fixedValueFvPatchScalarField::typeName;
        }
    }


    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        pd + rho*(g & mesh.C())
    );


    label pdRefCell = 0;
    scalar pdRefValue = 0.0;
    setRefCell
    (
        pd,
        mesh.solutionDict().subDict("PIMPLE"),
        pdRefCell,
        pdRefValue
    );

    scalar pRefValue = 0.0;

    if (pd.needReference())
    {
        pRefValue = readScalar
        (
            mesh.solutionDict().subDict("PIMPLE").lookup("pRefValue")
        );

        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pdRefCell)
        );
    }
