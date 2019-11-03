//Initial settling velocity assignment, definied this way to incorporate hindered settling if required
forAll (WsSS1, cellI)
{
  WsSS1[cellI] = WsSS01.value();
  WsSS2[cellI] = WsSS02.value();
}

forAll(WsSS1.boundaryField(), patchI)
{
  forAll(WsSS1.boundaryField()[patchI], faceI)
  {
    WsSS1.boundaryField()[patchI][faceI] = WsSS01.value(); 
    WsSS2.boundaryField()[patchI][faceI] = WsSS02.value(); 
  }
}
//update coupled boundaries	         
WsSS1.correctBoundaryConditions();
WsSS2.correctBoundaryConditions();

//Advection terms
AdSS1 = phi + (mesh.Sf() & fvc::interpolate(WsSS1));	
AdSS2 = phi + (mesh.Sf() & fvc::interpolate(WsSS2));

//Diffusion term, same for both fractions
DiSS = (nu1*alpha1) + (nu2*(1-alpha1));
DiSSt = (DiSS + turbulence->nut()()); 

//Filter to limit turbulence effects to water phase
//internal
forAll(DiSS, cellI)
{
  if ((1-alpha1[cellI]) < 1e-2) 
  {
    DiSS[cellI] = DiSSt[cellI];
  }
}
//boundary fields
forAll(DiSS.boundaryField(), patchI)
{
  forAll(DiSS.boundaryField()[patchI], faceI)
  {
    if ((1-alpha1.boundaryField()[patchI][faceI]) < 1e-2)
    {
      DiSS.boundaryField()[patchI][faceI] = DiSSt.boundaryField()[patchI][faceI];
    }
  }
}
//update coupled boundaries
DiSS.correctBoundaryConditions();

//SS1 equation definition and solution
fvScalarMatrix C1Eqn
(
  fvm::ddt(C1)
  + fvm::div(AdSS1,C1)
  - fvm::laplacian(DiSS,C1)
); 

C1Eqn.solve();

//SS2 equation definition and solution
fvScalarMatrix C2Eqn
(
  fvm::ddt(C2)
  + fvm::div(AdSS2,C2)
  - fvm::laplacian(DiSS,C2)
); 

C2Eqn.solve();

forAll(aPatch, faceI)
{
  label faceCellI = aPatch.faceCells()[faceI];

  zDimA[faceI] = zDim[faceCellI]; //height of cells on the bed

  //Entrainment rate calculations 
  if ((thetaSS1[faceI]-thetaCSS1[faceI]) > 0 && thetaCSS1[faceI] > 0 && zDimA[faceI] > 0 )
  {
    Ce1[faceI] = 0.015 * Dss1.value() * 
	         Foam::pow(((thetaSS1[faceI] - thetaCSS1[faceI]) / thetaCSS1[faceI]),1.5) /
                 ((zDimA[faceI] / 2) * dss1Star);
  }

  if ((thetaSS2[faceI]-thetaCSS2[faceI]) > 0 && thetaCSS2[faceI] > 0 && zDimA[faceI] > 0 )
  {
    Ce2[faceI] = 0.015 * Dss2.value() * 
	         Foam::pow(((thetaSS2[faceI] - thetaCSS2[faceI]) / thetaCSS2[faceI]),1.5) /
                 ((zDimA[faceI] / 2) * dss2Star);
  }

  Cpre[faceI] = C1[faceCellI]; //concentration prior to deltaH correction

  if (deltaH[faceI] != 0)
  {
     C1[faceCellI] -= (mesh.time().deltaT().value() * (SSd1[faceI] - SSe1[faceI]) / zDimA[faceI]);

     C2[faceCellI] -= (mesh.time().deltaT().value() * (SSd2[faceI] - SSe2[faceI]) / zDimA[faceI]);
   }

  Cpost[faceI] = C1[faceCellI]; //concentration after deltaH correction

  //Deposition rate
  SSd1[faceI] = C1[faceCellI] * mag(WsSS1[faceCellI]);
  SSd2[faceI] = C2[faceCellI] * mag(WsSS2[faceCellI]);
 
  //Erosion rate
  SSe1[faceI] = Pvc[faceI] * Ce1[faceI] * mag(WsSS1[faceCellI]);
  SSe2[faceI] = Pve[faceI] * Ce2[faceI] * mag(WsSS2[faceCellI]);

  //deltaH contributions from suspended sediments
  deltaHs1[faceI] = (mesh.time().deltaT().value()/(1 - n[faceI])) * 
                    (SSd1[faceI] - SSe1[faceI]);

  deltaHs2[faceI] = (mesh.time().deltaT().value()/(1 - n[faceI])) * 
                    (SSd2[faceI] - SSe2[faceI]);
}

deltaHs = deltaHs1 + deltaHs2; //combined height from SS

C = C1 + C2; //combined concentrations of SS

Info<< "Suspended sediment transport calculations completed" <<endl;
