    //volScalarField rhoUC = ((1 - (C/rhoSS)) *  rho) + C;
    //volScalarField Alpha2 = C/rhoUC;

    surfaceScalarField muEff
    (
        "muEff",
        twoPhaseProperties.muf()
      + fvc::interpolate(rho*turbulence->nut())
    );

    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(rhoPhi, U)
      - fvm::laplacian(muEff, U)
      - (fvc::grad(U) & fvc::grad(muEff))
     /*+ fvc::div
    (
        (Alpha2/(scalar(1.001) - Alpha2))*(sqr(rhoSS)/rhoUC)*WsSS*WsSS,
        "div(phiWsSS,WsSS)"
    )*/

    //- fvc::div(muEff*(fvc::interpolate(dev(fvc::grad(U))) & mesh.Sf()))
    );

    UEqn.relax();

    if (momentumPredictor)
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    fvc::interpolate(interface.sigmaK())*fvc::snGrad(alpha1)
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(pd)
                )*mesh.magSf()
            )
        );
    }



