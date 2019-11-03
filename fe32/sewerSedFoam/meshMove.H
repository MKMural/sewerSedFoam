//Setup and interpolation of deltaH from faces to points
primitivePatchInterpolation patchInterpolator (mesh.boundaryMesh()[aPatchID]);
scalarField faceValues = patchInterpolator.faceToPointInterpolate(deltaH);

//convert interpolated deltaH to a vectorField
vectorField dispVals (Psize);
forAll(dispVals, index)
{
  dispVals[index].x() = 0;
  dispVals[index].y() = 0;
  dispVals[index].z() = faceValues[index];
}

//Set up motionU field for the FA mesh
tetPointVectorField& motionU = 
const_cast<tetPointVectorField&>
(
  mesh.lookupObject<tetPointVectorField>
  (
    "motionU"
  )
);

fixedValueTetPolyPatchVectorField& motionUaPatch =
refCast<fixedValueTetPolyPatchVectorField>
(
  motionU.boundaryField()[aPatchID]
);

//Setup and interpolation of vector and point deltaH values into motionU values for mesh movement
tetPolyPatchInterpolation tppiAPatch
(
  refCast<const faceTetPolyPatch>
  (
    motionUaPatch.patch()
  )
);
motionUaPatch == tppiAPatch.pointToPointInterpolate (dispVals/mesh.time().deltaT().value());

Info<< "deltaH interpolated and motionU values assigned" << endl;
