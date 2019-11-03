newMeshPoints.internalField() = mesh.points(); //new mesh coordinates

newPoints.internalField() = aMesh.points(); //new FA mesh coordinates

icount = icount + 1; //iteration counter

int Psize= newPoints.size(); //number of FA mesh points

Info<< "Count = " << icount << endl;

//aMesh normal vectorField
PointNormalVector.internalField() = -1 * mesh.Sf().boundaryField()[aPatchID]; 

//sets up mapping between FA and FV meshes
volSurfaceMapping vsm(aMesh); 

vector posZ = vector(0, 0, -1); //Unit gravitation vector

vector posZ2 = vector(0, 0, 1); //Unit positive z vector

meshCentreN = mesh.C(); //mesh centres
 
aMeshCentreN.internalField() = vsm.mapToSurface(meshCentreN.boundaryField()); //new aMesh centres

//Original aMesh centres set up
if (runTime.value() <= (runTime.startTime().value() + mesh.time().deltaT().value()) 
    && runTime.value() < 21) 
{
  aMeshCentre.internalField() = vsm.mapToSurface(meshCentreN.boundaryField());
}

//Original mesh centres set up
if (runTime.value() <= (runTime.startTime().value() + mesh.time().deltaT().value())
    && runTime.value() < 21) 
{
  meshCentre = mesh.C();
}

//total height change for each cell up to previous timestep
totH = aMeshCentreN.component(2) - aMeshCentre.component(2);

//total height change for each cell up to two timesteps prior
totH1 = totH.oldTime();

forAll(aPatch, faceI)
{
  if (mag(PointNormalVector[faceI]) > 0)
  { 
    //aMesh unit normal vectorField
    uNormal[faceI] = PointNormalVector[faceI]/mag(PointNormalVector[faceI]); 
  }
  //initial bed thickness and proportion set up
  if (icount < 2 && runTime.value() <= 21 )
  {
    dd[faceI] = dd0.value(); //Bed thickness

    //initial sediment proportions by mass
    Pa[faceI] = Pa0.value();
    Pc[faceI] = Pc0.value();
    Pe[faceI] = Pe0.value();

    //initial sediment proportions by volume
    Pva[faceI] = (Pa[faceI]/rhoB.value()) / 
            ((Pa[faceI]/rhoB.value()) + (Pc[faceI]/rhoSS1.value()) + (Pe[faceI]/rhoSS2.value()));
    Pvc[faceI] = (Pc[faceI]/rhoSS1.value()) / 
            ((Pa[faceI]/rhoB.value()) + (Pc[faceI]/rhoSS1.value()) + (Pe[faceI]/rhoSS2.value()));
    Pve[faceI] = (Pe[faceI]/rhoSS2.value()) / 
            ((Pa[faceI]/rhoB.value()) + (Pc[faceI]/rhoSS1.value()) + (Pe[faceI]/rhoSS2.value()));
 
    PvaInit =  Pva[faceI]; //initial Sediment type A proportion
  }
   
  //Total height and time of deposition
  if (deltaH[faceI] > 0 && totH[faceI] > 0)
  {
    Hdep[faceI] += (totH[faceI] - totH1[faceI]);
    Tdep[faceI] += mesh.time().deltaT().value();
  }

  if (totH[faceI] < 0)
  {
    Hdep[faceI] = 0;
    Tdep[faceI] = 0;
  }

  //Total height and time of erosion
  if (totH[faceI] < totH1[faceI])
  {
    Te[faceI] += mesh.time().deltaT().value();
    He[faceI] += (totH1[faceI] - totH[faceI]);
  }

  else
  {
    Te[faceI] = 0;
    He[faceI] = 0;
  }

  //Sediment bed thickness
  dd[faceI] += totH[faceI] - totH1[faceI];

  //filter to prevent negative thickness
  if (dd[faceI] < 0)
  {
    dd[faceI] = 0;
  }

  //filter to stop deposition beyond a threshold
  if (dd[faceI] > ddmax[faceI])
  {
    ddmax[faceI] = dd[faceI];
  }

  //total volume of the bed in each cell of the FA mesh
  tVol[faceI] = mag(PointNormalVector[faceI]) * dd[faceI];

  //volume of each sediment class in the bed 
  volA[faceI] = Pva[faceI] * tVol[faceI];
  volC[faceI] = Pvc[faceI] * tVol[faceI];
  volE[faceI] = Pve[faceI] * tVol[faceI];

  //update sediment proportions
  if (deltaH[faceI] != 0 && DyP.value() == 1 )
  {
    //new sediment volumes per cell
    if (mag(deltaH[faceI]) > 0) 
    {
      tVolA[faceI] = volA[faceI] + (deltaHb[faceI] * mag(PointNormalVector[faceI]));
      tVolC[faceI] = volC[faceI] + (deltaHs1[faceI] * mag(PointNormalVector[faceI]));
      tVolE[faceI] = volE[faceI] + (deltaHs2[faceI] * mag(PointNormalVector[faceI]));
    } 

    //new sediment volumetric proportions
    if (tVolA[faceI] > 0) 
    {
      Pva[faceI] = tVolA[faceI] / (tVolA[faceI] + tVolC[faceI] + tVolE[faceI]);
    }

    if (tVolC[faceI] > 0) 
    {
      Pvc[faceI] = tVolC[faceI] / (tVolA[faceI] + tVolC[faceI] + tVolE[faceI]);
    }

    if (tVolE[faceI] > 0) 
    {
      Pve[faceI] = tVolE[faceI] / (tVolA[faceI] + tVolC[faceI] + tVolE[faceI]);
    }
  }

  //filters to prevent instabilities from bedload changes
  if (Pva[faceI] < 0.2) 
  {
    Pva[faceI] = 0.2;
    Pvc[faceI] = 0.8;
  }  

  if (Pva[faceI] > 1) 
  {
    Pva[faceI] = 1;
  }

  if (Pvc[faceI] < 0) 
  { 
    Pvc[faceI] = 0;
  }

  //updated sediment mass proportions
  Pa[faceI] = Pva[faceI] * rhoB.value() / 
   ((Pva[faceI] * rhoB.value()) + (Pvc[faceI] * rhoSS1.value()) + (Pve[faceI] * rhoSS2.value()));
  Pc[faceI] = Pvc[faceI] * rhoSS1.value() / 
   ((Pva[faceI] * rhoB.value()) + (Pvc[faceI] * rhoSS1.value()) + (Pve[faceI] * rhoSS2.value()));
  Pe[faceI] = Pve[faceI] * rhoSS2.value() / 
   ((Pva[faceI] * rhoB.value()) + (Pvc[faceI] * rhoSS1.value()) + (Pve[faceI] * rhoSS2.value()));

  //mesh centres for erosion limit check
  yCo[faceI] = aMeshCentreN[faceI].component(vector::Y);
  zCo[faceI] = aMeshCentreN[faceI].component(vector::Z);
  xCo[faceI] = aMeshCentreN[faceI].component(vector::X);
}

//first iteration set up
if (runTime.value() <= (runTime.startTime().value() + mesh.time().deltaT().value()) 
    && runTime.value() < 21) 
{
  ddmax = dd;
}

scalar SrB = (rhoB.value()/rhof.value()) - 1; //relative bedload density

scalar SrSS1 = (rhoSS1.value()/rhof.value()) - 1; //relative SS1 density

scalar SrSS2 = (rhoSS2.value()/rhof.value()) - 1; //relative SS2 density

//bed dimensionless diameter
scalar dbStar = Db.value() * 
                Foam::pow((SrB*mag(g.value())/(nu1.value()*nu1.value())),0.33333);
//SS1 dimensionless diameter
scalar dss1Star = Dss1.value() * 
                 Foam::pow((SrSS1*mag(g.value())/(nu1.value()*nu1.value())),0.33333); 

//SS2 dimensionless diameter
scalar dss2Star = Dss2.value() * 
                 Foam::pow((SrSS2*mag(g.value())/(nu1.value()*nu1.value())),0.33333); 

//Overall bed dimensionless diameter and relative density
Dbed = (Pa * Db.value()) + (Pc * Dss1.value()) + (Pe * Dss2.value());

scalar SrBed = (Pa0.value() * SrB) + (Pc0.value() * SrSS1) + (Pe0.value() * SrSS2);

//turbulence mapping
Ktemp = turbulence->k()();


//Stokes settling velocity of suspended sediments
dimensionedVector WsSS01 = (SrSS1 * 9.81 * Foam::pow(Dss1.value(),2)) / ((18 * nu1.value()) + 
                          Foam::sqrt(0.75*SrSS1*9.81*Foam::pow(Dss1.value(),3))) 
	                  * posZ;

dimensionedVector WsSS02 = (SrSS2 * 9.81 * Foam::pow(Dss2.value(),2)) / ((18 * nu1.value()) + 
                          Foam::sqrt(0.75*SrSS1*9.81*Foam::pow(Dss2.value(),3))) 
	                  * posZ;

//Gravity force along steepest slope
W = M_PI * SrB * rhof * pow(Db,3) * g / 6; 

//cell height calculations for SSe
const faceList & ff = mesh.faces();
 
const pointField & pp = mesh.points(); 

forAll ( mesh.C(), celli) 
{ 
  const cell & cc = mesh.cells()[celli]; 

  labelList pLabels(cc.labels(ff)); 

  pointField pLocal(pLabels.size(), vector::zero); 

  forAll (pLabels, pointi) 
  {
    pLocal[pointi] = pp[pLabels[pointi]]; 
  }

  zDim[celli] = Foam::max(pLocal & vector(0,0,1)) - Foam::min(pLocal & vector(0,0,1)); 
}

Info<< "Initial Fields Calculated" << endl;
