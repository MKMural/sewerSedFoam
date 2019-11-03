scalar thetaC0B = 0;

scalar thetaC0SS1 = 0;

scalar thetaC0SS2 = 0;

//initial sediment critical Shield's parameters based on dstar
if (dbStar > 10)
{
 thetaC0B = (0.3/(1 + (1.2*dbStar))) + (0.055*(1-Foam::exp(-0.02 * dbStar))); 
}

if (dbStar < 10 && dbStar >= 4)
{
 thetaC0B = 0.14 * Foam::pow(dbStar,-0.64);
}

if (dbStar < 4)
{
 thetaC0B = 0.115 * Foam::pow(dbStar,-0.5);
}

if (dss1Star > 10)
{
 thetaC0SS1 = (0.3/(1 + (1.2*dss1Star))) + (0.055*(1-Foam::exp(-0.02 * dss1Star))); 
}

if (dss1Star < 10 && dss1Star >= 4)
{
 thetaC0SS1 = 0.14 * Foam::pow(dss1Star,-0.64);
}

if (dss1Star < 4)
{
 thetaC0SS1 = 0.115 * Foam::pow(dss1Star,-0.5);
}

if (dss2Star > 10)
{
 thetaC0SS2 = (0.3/(1 + (1.2*dss2Star))) + (0.055*(1-Foam::exp(-0.02 * dss2Star))); 
}

if (dss2Star < 10 && dss2Star >= 4)
{
 thetaC0SS2 = 0.14 * Foam::pow(dss2Star,-0.64);
}

if (dss2Star < 4)
{
 thetaC0SS2 = 0.115 * Foam::pow(dss2Star,-0.5);
}

forAll(aPatch, faceI)
{ 
 label faceCellI = aPatch.faceCells()[faceI];

 kTempA[faceI] = Ktemp[faceCellI]; //k values of the FA mesh

 //friction velocity magnitude of the bed mesh
 Uf[faceI] = Foam::pow(kTempA[faceI],0.5) * Foam::pow(0.09,0.25);

 //near bed velocity
 aUf[faceI] = BLa.value()*Uf[faceI]*(U[faceCellI]/mag(U[faceCellI])); 

 if (pow(mag(PointNormalVector[faceI]),2) > 0)
 {
  //velocity projected onto the bed
  Utau[faceI] = (PointNormalVector[faceI]^(aUf[faceI]^PointNormalVector[faceI])) / 
                 pow(mag(PointNormalVector[faceI]),2);

  //friction force along steepest slope mapped to bed
  Wt[faceI] = (PointNormalVector[faceI]^(W[faceI]^PointNormalVector[faceI]))  / 
                pow(mag(PointNormalVector[faceI]),2);
 }

 //initial critical Shield's stress assignment
 thetaC1B[faceI] = thetaC0B;

 thetaC1SS1[faceI] = thetaC0SS1;

 thetaC1SS2[faceI] = thetaC0SS2;

 //applied shield's parameter from flow
 thetaB[faceI] = pow(Uf[faceI],2)/(SrB*mag(g.value())*Db.value());

 thetaSS1[faceI] = pow(Uf[faceI],2)/(SrSS1*mag(g.value())*Dss1.value());

 thetaSS2[faceI] = pow(Uf[faceI],2)/(SrSS2*mag(g.value())*Dss2.value());

 //applied shear stress for ref
 tau[faceI] = pow(Uf[faceI],2) * rhof.value(); 

 //Default assignment before variation
 if (dd[faceI] > 0)
 {
  thetaC2B[faceI] = thetaC1B[faceI];
  thetaC2SS1[faceI] = thetaC1SS1[faceI];
  thetaC2SS2[faceI] = thetaC1SS2[faceI];
  n[faceI] = n0.value();
 }

 //Erosion scenario with weak layer for the first third of the bed
 if (dd[faceI] > 0 && dd[faceI] > (ddmax[faceI] / 3) && Te[faceI] > 0)
 {
  thetaC2B[faceI] = ((Femp.value()*thetaC1B[faceI]/(ddmax[faceI]/3)) * 
                    (ddmax[faceI] - dd[faceI])) + thetaC1B[faceI];

  thetaC2SS1[faceI] = ((Femp.value()*thetaC1SS1[faceI]/(ddmax[faceI]/3)) * 
                       (ddmax[faceI] - dd[faceI])) + thetaC1SS1[faceI];

  thetaC2SS2[faceI] = ((Femp.value()*thetaC1SS2[faceI]/(ddmax[faceI]/3)) * 
                       (ddmax[faceI] - dd[faceI])) + thetaC1SS2[faceI];
 }

 //bed stabilized height calculation
 if (bedSS.value() == 1 && icount < 2 && totH[faceI] <= Href1.value())
 {
  Href[faceI] = totH[faceI] + Hmax.value();
 }
 if (bedSS.value() == 1 && icount < 2 && totH[faceI] > Href1.value() &&
    (Href[faceI] == 0 || Href[faceI] == Hmax.value()))
 {
  Href[faceI] = Href1.value() + (totH[faceI] - Href1.value()) * (Href1.value()/totH[faceI]);
 }
 if (bedSS.value() == 0)
 {
  Href[faceI] = Hmax.value();
 }

 //continuous deposition corrector for critical shields parameter
 if (totH[faceI] > 0 && totH[faceI] < Href[faceI] && Tdep[faceI] > TdepSP.value() 
     && thetaSS1[faceI] > thetaSS1init[faceI] && He[faceI] < 0.001 && thetaBinit[faceI] > 0)
 {
  //thetaC2B[faceI] = thetaC1B[faceI] * (thetaB[faceI]/thetaBinit[faceI]);
  thetaC2SS1[faceI] = thetaC1SS1[faceI] * (thetaSS1[faceI]/thetaSS1init[faceI]);
  thetaC2SS2[faceI] = thetaC1SS2[faceI] * (thetaSS2[faceI]/thetaSS2init[faceI]);
 }

 //continuous deposition max height initialization
 if ((totH[faceI]/Href[faceI]) > 0.9 && (totH[faceI]/Href[faceI]) <= 0.99999999)
 {
  //thetaHMaxB[faceI] = thetaB[faceI];
  thetaHMaxSS1[faceI] = thetaSS1[faceI];
  thetaHMaxSS2[faceI] = thetaSS2[faceI];
 }

 //continuous deposition corrector for critical shields parameter above setpoint 
 if (totH[faceI] >= Href[faceI] && Tdep[faceI] > TdepSP.value()  && thetaBinit[faceI] > 0 && 
    thetaSS1[faceI] > thetaSS1init[faceI] && He[faceI] < 0.001 && thetaHMaxSS1[faceI] > 0)
 {
  //thetaC2B[faceI] = thetaC1B[faceI] * (thetaHMaxB[faceI]/thetaBinit[faceI]);
  thetaC2SS1[faceI] = thetaC1SS1[faceI] * (thetaHMaxSS1[faceI]/thetaSS1init[faceI]);
  thetaC2SS2[faceI] = thetaC1SS2[faceI] * (thetaHMaxSS2[faceI]/thetaSS2init[faceI]);
 }

 //Deposited bed below 1/3rd depth scenario
 if (dd[faceI] <= (ddmax[faceI] / 3))
 {
  thetaC2B[faceI] = (Femp.value() + 1) * thetaC1B[faceI];
  thetaC2SS1[faceI] = (Femp.value() + 1) * thetaC1SS1[faceI];
  thetaC2SS2[faceI] = (Femp.value() + 1) * thetaC1SS2[faceI];
 }
}

forAll(aPatch, faceI)
{
 //slope of the cell to the horizontal
 angBeta[faceI] = Foam::acos(uNormal[faceI]&posZ2); 

 if ((mag(Utau[faceI]) != 0) && (mag(Wt[faceI]) != 0) && 
    ((Wt[faceI]&Utau[faceI]) / (mag(Wt[faceI])*mag(Utau[faceI]))) <= 1 &&
    ((Wt[faceI]&Utau[faceI]) / (mag(Wt[faceI])*mag(Utau[faceI]))) >= -1 )
 {
  //angle between Utau and steepest slope
  angAlpha[faceI] = Foam::acos((Wt[faceI]&Utau[faceI]) / (mag(Wt[faceI])*mag(Utau[faceI])));
 }

 //final critical shields parameter with angle effects from morphology
 if (angBeta[faceI] == 0 && angAlpha[faceI] == 0)
 {
  thetaCB[faceI] = thetaC2B[faceI];
  thetaCSS1[faceI] = thetaC2SS1[faceI];
  thetaCSS2[faceI] = thetaC2SS2[faceI];
 }
 else
 {
  thetaCB[faceI] = thetaC2B[faceI] * 
                   ((Foam::cos(angBeta[faceI]) * Foam::sqrt(1 - pow((Foam::sin(angAlpha[faceI]) 
                   * Foam::tan(angBeta[faceI]) / mus.value()),2))) 
                   - (Foam::cos(angAlpha[faceI]) * Foam::sin(angBeta[faceI]) / mus.value()));
 
  thetaCSS1[faceI] = thetaC2SS1[faceI] * 
                     ((Foam::cos(angBeta[faceI]) * Foam::sqrt(1 - pow((Foam::sin(angAlpha[faceI])
                     *  Foam::tan(angBeta[faceI]) / mus.value()),2))) 
                     - (Foam::cos(angAlpha[faceI]) * Foam::sin(angBeta[faceI]) / mus.value())); 

  thetaCSS2[faceI] = thetaC2SS2[faceI] * 
                     ((Foam::cos(angBeta[faceI]) * Foam::sqrt(1 - pow((Foam::sin(angAlpha[faceI])
                     * Foam::tan(angBeta[faceI]) / mus.value()),2))) 
                     - (Foam::cos(angAlpha[faceI]) * Foam::sin(angBeta[faceI]) / mus.value())); 
 }

 //Time since bedload movement
 if (thetaB[faceI] < thetaCB[faceI]) 
 {
  Tb[faceI] += mesh.time().deltaT().value(); 
 }

 //Final dimensioned critical shear stresses for reference
 tauCB[faceI] = thetaCB[faceI] * rhof.value() * (SrBed) * mag(g.value()) * Db.value();
 tauCSS1[faceI] = thetaCSS1[faceI] * rhof.value() * (SrSS1) * mag(g.value()) * Dss1.value();
 tauCSS2[faceI] = thetaCSS2[faceI] * rhof.value() * (SrSS2) * mag(g.value()) * Dss2.value();

 //initial applied Shield's stress for deposition scaling
 if (runTime.value() < 20.5 || thetaBinit[faceI] == 0) 
 {
  thetaBinit[faceI] = thetaB[faceI];
  thetaSS1init[faceI] = thetaSS1[faceI];
  thetaSS2init[faceI] = thetaSS2[faceI];
 }
}

Info<< "Derived Fields Calculated" << endl;
