#include "ShapeInverterNode.h"
#include <maya/MThreadUtils.h>
MTypeId     ShapeInverter::id(0x207428);
MObject     ShapeInverter::activate;
MObject     ShapeInverter::correctiveMesh;
MObject     ShapeInverter::deformedPoints;
MObject     ShapeInverter::inversionMatrix;

ShapeInverter::ShapeInverter()
{
    m_deformedPoints = false;
    __initialized = false;
}
ShapeInverter::~ShapeInverter(){}

void* ShapeInverter::creator()
{
    return new ShapeInverter();
}
MStatus ShapeInverter::initialize()
{
    MFnNumericAttribute nAttr;
    MFnMatrixAttribute  mAttr;
    MFnTypedAttribute   tAttr;

    activate = nAttr.create("activate", "activate", MFnNumericData::kBoolean, 0);
    nAttr.setStorable(true);
    addAttribute(activate);

    correctiveMesh = tAttr.create("correctiveMesh", "cm", MFnData::kMesh);
    tAttr.setStorable(true);
    addAttribute(correctiveMesh);

    deformedPoints = tAttr.create("deformedPoints", "dp", MFnData::kPointArray);
    tAttr.setStorable(true);
    addAttribute(deformedPoints);

    inversionMatrix = mAttr.create("inversionMatrix", "im", MFnMatrixAttribute::kDouble);
    mAttr.setDefault(MMatrix::identity);
    mAttr.setArray(true);
    mAttr.setHidden(true);
    mAttr.setUsesArrayDataBuilder(true);
    mAttr.setStorable(true);
    addAttribute(inversionMatrix);

    attributeAffects(ShapeInverter::activate, ShapeInverter::outputGeom);
    attributeAffects(ShapeInverter::correctiveMesh, ShapeInverter::outputGeom);
    return MS::kSuccess;
}
MStatus ShapeInverter::deform(MDataBlock& block,
    MItGeometry& iter,
    const MMatrix& localToWorldMatrix,
    unsigned int multiIndex)
{
    MThreadUtils::syncNumOpenMPThreads();
    MStatus status = MS::kSuccess;
    int env = block.inputValue(activate).asBool();
    if (env == 0)
        return status;
    if (!__initialized)  {
        MFnPointArrayData CoDeformedPoints = block.inputValue(deformedPoints).data();
        CoDeformedPoints.copyTo(oDeformedPoints);
        if (oDeformedPoints.length() == 0)
        {
            return MS::kSuccess;
        }
        MArrayDataHandle hMatrix = block.inputArrayValue(inversionMatrix);
        for (int i = 0; i < oDeformedPoints.length(); ++i)
        {
            hMatrix.jumpToElement(i);
            MMatrix mat = hMatrix.inputValue().asMatrix();
            homeMMatrixarrya.append(mat);
        }
    }
    __initialized = true;
    MPointArray verts;
    iter.allPositions(verts);
    int nPoints = verts.length();
    if (correctivePoints.length() == 0)
    {
        return MS::kSuccess;
    }
#ifdef _OPENMP
#pragma omp parallel for if(nPoints>3000 )
#endif
    for (int index = 0; index < nPoints; index++)
    {
        MPoint delta = correctivePoints[index] - oDeformedPoints[index];
        MMatrix value;
        homeMMatrixarrya.get(value, index);
        MPoint offset = delta *value;
        if (offset.distanceTo(verts[index]) != 0.000f)
            verts[index] = verts[index] + offset;
    }

    iter.setAllPositions(verts);
    return status;
}

MStatus ShapeInverter::setDependentsDirty(const MPlug& plug, MPlugArray& plugArray)
{
    MStatus status;
    if (plug == correctiveMesh)
    {
        MObject meshObj = plug.asMObject();
        if (meshObj.isNull())
        {
            return MS::kSuccess;
        }
        MFnMesh fnMesh(meshObj, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        MPointArray AcorrectivePoints;
        fnMesh.getPoints(AcorrectivePoints);
        correctivePoints.copy(AcorrectivePoints);
    }
    return MS::kSuccess;
}