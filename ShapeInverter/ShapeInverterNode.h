#ifndef _ShapeInverterNode
#define _ShapeInverterNode

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxDeformerNode.h>
#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MFnStringData.h>
#include <maya/MString.h>

#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MFloatMatrix.h>

#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MFnMesh.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MArgDatabase.h>

#include "ShapeInverterCmd.h"
#include "CArray.h"

class ShapeInverter : public MPxDeformerNode
{
public:
    ShapeInverter();
    virtual             ~ShapeInverter();
    static  void*       creator();
    static  MStatus     initialize();
    virtual MStatus     deform(MDataBlock&data, MItGeometry&itGeo, const MMatrix&localToWorldMatrix, unsigned int geomIndex);
    virtual MStatus     setDependentsDirty(const MPlug& plug, MPlugArray& plugArray);
    typedef CArray<MMatrix> MMatrixArray;

public:
    static  MObject     activate;
    static  MObject     correctiveMesh;
    static  MObject     deformedPoints;
    static  MObject     inversionMatrix;
    static  MTypeId     id;
    bool    m_deformedPoints;
    bool __initialized;
    MPointArray oDeformedPoints;
    MPointArray correctivePoints;
    MMatrixArray homeMMatrixarrya;

private:
};
#endif