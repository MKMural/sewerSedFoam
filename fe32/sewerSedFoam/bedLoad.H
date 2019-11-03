//scalars for Ub iterative loop, uncomment if iterative loop is used
/*scalar count = 0;

scalar Ubcount = 0;

scalar iter = Ubiterations.value();*/

forAll(aPatch, faceI)
{
  q_bed[faceI] = vector(0,0,0); //re-initialiased bedload transport

  if (thetaC2B[faceI] > 0)
  {
    //drag and lift coefficient
    Cs[faceI] = 4 * mud.value() / (3 * pow(BLa.value(),2) * 0.5 * thetaC2B[faceI]);
  }

  if (thetaB[faceI] > thetaCB[faceI]) 
  {
    Pef[faceI] = Foam::pow(1 + 
                 Foam::pow(((M_PI*mud.value()/6)/(thetaB[faceI] - thetaCB[faceI])),4),-0.25);

    //2-D Ub calc based on Engelund and Fredsoe (1976)
    if ((thetaB[faceI] > 0)  && (thetaCB[faceI] > 0) && 
        ((0.7*Foam::pow((thetaCB[faceI]/thetaB[faceI]),0.5)) <= 1) )
    {
      Ub2[faceI] = (1 - (0.7*Foam::pow((thetaCB[faceI]/thetaB[faceI]),0.5))) * Utau[faceI];
	
      if (Ub2[faceI].x() < 0)
      {
        Ub2[faceI] = vector(0,0,0);
      }
    }

//Un-comment the code below and swap Ub2 for Ub in the final q_bed calculation in 3-D cases where there is significant morphology changes along the y-axis, else the Ub2 calculation above is much quicker and reasonably accurate

/*
    //3-D Ub loop variables
    a[faceI] = 0.5 * 0.25 * rhof.value() * M_PI * pow(Db.value(),2) * Cs[faceI];

    b[faceI] = Foam::sqrt(pow((W[faceI]&PointNormalVector[faceI]),2)) * mud.value() 
  	       / mag(PointNormalVector[faceI]);

    //set Ub guess value for first iteration
    if (runTime.value() <= (runTime.startTime().value() + mesh.time().deltaT().value()) 
        && runTime.value() < 21)
    {
      x1[faceI] = vector(0.01,0,0.00001);
    }

    //Ub non-linear iterative loop
    for (count=0; count<iter; count++)  
    {
      f1[faceI] = (a[faceI]*mag(Utau[faceI]-x1[faceI])*(Utau[faceI]-x1[faceI])) + Wt[faceI] 
                - (b[faceI]*x1[faceI]/ mag(x1[faceI]));

      if (Foam::sqrt(pow(f1[faceI].component(0),2)) < 1e-20)  
      {
        break;
      }
      else if(f1[faceI].component(0) > 0)
      {
        x1[faceI].component(0) += xsens.value();
      }
      else if(f1[faceI].component(0) < 0)
      {
         x1[faceI].component(0) -= xsens.value();
      }
      else if (f1[faceI].component(2) < 0)
      {
        x1[faceI].component(2) += yzsens.value();
      }
      else if (f1[faceI].component(2) > 0)
      {
        x1[faceI].component(2) -= yzsens.value();
      }
      else if (f1[faceI].component(1) < 0)
      {
        x1[faceI].component(1) += yzsens.value();
      }
      else if (f1[faceI].component(1) > 0)
      {
        x1[faceI].component(1) -= yzsens.value();
      }

      Ubcount +=1;
    } 
   
    Ub[faceI] = x1[faceI]; //Final Ub assignment

    if (Ub[faceI].component(0) < 0)
    {
      Ub[faceI] = vector(0,0,0);
    }
*/

    //final Qbed rate calc
    q_bed[faceI] = M_PI * Db.value() * Pef[faceI] * Ub2[faceI] / 6;
  }
}

//Ub loop counter, uncomment if required
/*
if (Ubcount > 1)
{
  Info<< "Ub iterative loop complete, iteration count = " << Ubcount <<endl;
}*/

//bedload deltaH contribution
deltaHb = - Pva * mesh.time().deltaT().value() * fac::div(q_bed) / (1 - n);

//multiplier for filter
scalar mult = (floor(runTime.value() - 20) * 0.02) + 1;

forAll(aPatch, faceI)
{ 
  //filter to stop high divergence values around the inlet/outlet where deltaH is suppressed
  if (xCo[faceI] < (0.5 + xMin.value() + UxMin.value()))
  {
    deltaHb[faceI] = 0;
  }
  if (xCo[faceI] > (xMax.value() - UxMax.value() - 0.5))
  {
    deltaHb[faceI] = 0;
  }

  //filter to reduce abnormally high deltaHb values, repeated to ensure sufficient reduction
  if (mag(deltaHb[faceI]) >= 0.1 * dd[faceI])
  {
    deltaHb[faceI] = deltaHb[faceI] / 0.5 ; 
  } 
  if (mag(deltaHb[faceI]) >= 0.1 * dd[faceI])
  {
    deltaHb[faceI] = deltaHb[faceI] / 0.5 ; 
  } 

  //filter to stop excess erosion of bedload sediment
  if (deltaHb[faceI] < 0 && Pa[faceI] < 0.3)
  {
    deltaHb[faceI] = 0; 
  }  

//filter to limit bedload deposition/erosion beyond initial sediment volumes in the bed, the mult scalar aims to gradually increase the amount of additional bedload sediment as time passes, curerently by 2% every second, to incorporate the introduction of additional bedload sediment
  if (deltaHb[faceI] > 0 && 
     (volA[faceI] + (deltaHb[faceI] * mag(PointNormalVector[faceI]))) >= 
      (mult * PvaInit * dd0.value() * mag(PointNormalVector[faceI])))
  {
    deltaHb[faceI] = deltaHb[faceI] / 2;
  }

  if (deltaHb[faceI] > 0 && 
     (volA[faceI] + (deltaHb[faceI] * mag(PointNormalVector[faceI]))) >= 
      (mult * PvaInit * dd0.value() * mag(PointNormalVector[faceI])))
  {
    deltaHb[faceI] = deltaHb[faceI] / 2;
  }

  if (deltaHb[faceI] > 0 && 
     (volA[faceI] + (deltaHb[faceI] * mag(PointNormalVector[faceI]))) >= 
      (mult * PvaInit * dd0.value() * mag(PointNormalVector[faceI])))
  {
    deltaHb[faceI] = 0;
  }
}

Info<< "Bedload transport calculations completed" << endl;
