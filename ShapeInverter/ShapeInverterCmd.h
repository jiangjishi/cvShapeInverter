#ifndef _ShapeInverterCmd
#define _ShapeInverterCmd

#include <maya/MSelectionList.h>

#include <maya/MPxCommand.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MItGeometry.h>
#include <maya/MFnMatrixData.h>
#include <maya/MMatrix.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MArgList.h>

//#include "ShapeInverterNode.h"
#include <maya/MSyntax.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MDGModifier.h>
#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MDoubleArray.h>
#include <maya/MStatus.h>
#include <maya/MMessage.h>
#include <maya/MUserEventMessage.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MItGeometry.h>
#include <maya/MFnMatrixData.h>
#include <maya/MMatrix.h>
#include <maya/MFnPointArrayData.h>

#define bmFlag   "-bm"
#define bmLongFlag   "-baseMesh"
#define insFlag   "-ins"
#define insLongFlag   "-invertedShape"
#define sidFlag   "-sid"
#define sidLongFlag   "-ShapeInverterdeformer"
#define omFlag   "-om"
#define omLongFlag   "-origMesh"

class ShapeInverterCmd : public MPxCommand

{
public:
    ShapeInverterCmd();
    virtual ~ShapeInverterCmd();
    static void *creator();
    static MSyntax newSyntax();
    MStatus doIt(const MArgList&);
private:
    void getDagPathByName(const MString & name, MDagPath & dagPath);
    void getObjectByName(const MString & name, MObject & object);
    void getPoints(const MString & name, MPointArray & points);
    void setPoints(const MString & name, MPointArray & points);
};

#endif
