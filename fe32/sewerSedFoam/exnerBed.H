//initial variable calculations
# include "initFields.H" 

//derived variable calculations
# include "derivedFields.H"

//bedload transport and deposited bed contributions calculated
# include "bedLoad.H"

//suspended  transport and deposited bed contributions calculated
# include "suspendedLoad.H"

//current timestep value of deltaH
Fi = deltaHs + deltaHb;

//previous timestep value of deltaH
Fi1 = Fi.oldTime();

//deltaH value from two timesteps before
Fi2 = Fi1.oldTime();

//final deltaH calculation with scaling
deltaH = morphTimeScale.value() * Fi;
 
scalar Hsum = 0; 

scalar Hsum2 = 0; 

scalar Hsum3 = 0;

forAll(aPatch, faceI)
{
  //filter to stop erosion beyond original mesh specs
  if (deltaH[faceI] < 0)
  {
   if (erosion.value() == 0 || 
       aMeshCentreN[faceI].component(2) <= (aMeshCentre[faceI].component(2) - H0.value()))
   {
    deltaH[faceI] = 0;  	 
   }
  }

  //maximum increase per time step
  if (deltaH[faceI] > 0.001)
  {
   deltaH[faceI] = 0.001;  	
  } 

  //max height filter if required, change 0.1m to the required value
  if (totH[faceI] > 0.1 && deltaH[faceI] > 0)
  {
   deltaH[faceI] = 0;  	
  } 

  //inlet and outlet area filter
  if (xCo[faceI] < (xMin.value() + UxMin.value()) || xCo[faceI] > (xMax.value() - UxMax.value()))
  {
   deltaH[faceI] = 0;
  }

  //filter to block deltaH for the first time step
  if (icount < 2)
  {
   deltaH[faceI] = 0;
  }

  if (deltaH[faceI] != 0)
  {
   //average deltaH magnitude
   Hsum += Foam::sqrt(pow(deltaH[faceI],2));

   //average deltaH
   Hsum2 += deltaH[faceI];

   Hsum3 += 1;
  }
}

Info<< "Overall Cumulative deltaH magnitude = " << Hsum << endl;

if (Hsum3 > 0)
{
 Info<< "Overall Average deltaH = " << (Hsum2/Hsum3) << endl;
}

//mapping key area fields to paraviewable volume fields
vsm.mapToVolume (q_bed, q_b_slide.boundaryField());

vsm.mapToVolume(deltaH,deltaHF.boundaryField());

vsm.mapToVolume (Ce1,testS.boundaryField());

vsm.mapToVolume(Ub,testV.boundaryField());

if (Hsum > 0)
{
 //deltaH interpolation and mesh movement calculations
 # include "meshMove.H"
}
