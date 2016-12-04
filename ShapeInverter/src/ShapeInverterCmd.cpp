#include "ShapeInverterCmd.h"
void ShapeInverterCmd::getPoints(const MString & name, MPointArray & points)
{
    MDagPath  dagPath;
    getDagPathByName(name, dagPath);
    dagPath.extendToShape();
    MItGeometry iterIn(dagPath);
    iterIn.allPositions(points, MSpace::kObject);
}

void ShapeInverterCmd::setPoints(const MString & name, MPointArray & points)
{
    MDagPath  dagPath;
    getDagPathByName(name, dagPath);
    dagPath.extendToShape();
    MItGeometry iterIn(dagPath);
    iterIn.setAllPositions(points, MSpace::kObject);
}

void ShapeInverterCmd::getObjectByName(const MString & name, MObject & object)
{
    object = MObject::kNullObj;
    MSelectionList sList;
    sList.add(name);
    sList.getDependNode(0, object);
}

void ShapeInverterCmd::getDagPathByName(const MString & name, MDagPath & dagPath)
{
    MSelectionList sList;
    sList.add(name);
    sList.getDagPath(0, dagPath);
}

ShapeInverterCmd::ShapeInverterCmd(){}
ShapeInverterCmd::~ShapeInverterCmd(){}

MSyntax ShapeInverterCmd::newSyntax()
{
    MSyntax syntax;
    syntax.addFlag(bmFlag, bmLongFlag, MSyntax::kString);
    syntax.addFlag(insFlag, insLongFlag, MSyntax::kString);
    syntax.addFlag(sidFlag, sidLongFlag, MSyntax::kString);
    syntax.addFlag(omFlag, omLongFlag, MSyntax::kString);
    return syntax;
}

MStatus ShapeInverterCmd::doIt(const MArgList& args)

{
    MArgDatabase argData(syntax(), args);

    MString baseMesh;
    argData.getFlagArgument(bmFlag, 0, baseMesh);
    MString invertedShape;
    argData.getFlagArgument(insFlag, 0, invertedShape);
    MString ShapeInverterdeformer;
    argData.getFlagArgument(sidFlag, 0, ShapeInverterdeformer);
    MString origMesh;
    argData.getFlagArgument(omFlag, 0, origMesh);

    MPointArray	basePoints;
    getPoints(baseMesh, basePoints);
    int numPoints = basePoints.length();

    MPointArray	origPoints;
    getPoints(origMesh, origPoints);
    MPointArray	 xPoints(origPoints);
    MPointArray	 yPoints(origPoints);
    MPointArray	 zPoints(origPoints);
    for (int i = 0; i < numPoints; i++)
    {
        xPoints[i].x += 1.0;
        yPoints[i].y += 1.0;
        zPoints[i].z += 1.0;
    }
    setPoints(origMesh, xPoints);
    getPoints(baseMesh, xPoints);

    setPoints(origMesh, yPoints);
    getPoints(baseMesh, yPoints);

    setPoints(origMesh, zPoints);
    getPoints(baseMesh, zPoints);

    setPoints(origMesh, origPoints);
    setPoints(invertedShape, origPoints);

    MObject oDeformer;
    getObjectByName(ShapeInverterdeformer, oDeformer);
    MFnDependencyNode  fnDeformer(oDeformer);
    MPlug plugMatrix = fnDeformer.findPlug("inversionMatrix", false);
    MFnMatrixData fnMatrixData;
    for (int i = 0; i < numPoints; i++)
    {
        double myMatrix[4][4];// = { { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0 }, { 0, 0, 0, 1.0 } };

        MPoint	xPoint = xPoints[i] - basePoints[i];
        MPoint	yPoint = yPoints[i] - basePoints[i];
        MPoint	zPoint = zPoints[i] - basePoints[i];

        myMatrix[0][0] = xPoint.x;
        myMatrix[0][1] = xPoint.y;
        myMatrix[0][2] = xPoint.z;
        myMatrix[0][3] = 0.0;

        myMatrix[1][0] = yPoint.x;
        myMatrix[1][1] = yPoint.y;
        myMatrix[1][2] = yPoint.z;
        myMatrix[1][3] = 0.0;

        myMatrix[2][0] = zPoint.x;
        myMatrix[2][1] = zPoint.y;
        myMatrix[2][2] = zPoint.z;
        myMatrix[2][3] = 0.0;

        myMatrix[3][0] = 0.0;
        myMatrix[3][1] = 0.0;
        myMatrix[3][2] = 0.0;
        myMatrix[3][3] = 1.0;

        //if (i < 20) {
        //MGlobal::displayInfo(MString("m_inversionmyMatrixrix :") + myMatrix[0][0] + "  :  " + myMatrix[0][1] + "  :  " + myMatrix[0][2] + "  :  " + myMatrix[0][3] + "  :  " + myMatrix[1][0] + "  :  " + myMatrix[1][1] + "  :  " + myMatrix[1][2] + "  :  " + myMatrix[1][3] + "  :  " + myMatrix[2][0] + "  :  " + myMatrix[2][1] + "  :  " + myMatrix[2][2] + "  :  " + myMatrix[2][3] + "  :  " + myMatrix[3][0] + "  :  " + myMatrix[3][1] + "  :  " + myMatrix[3][2] + "  :  " + myMatrix[3][3] + "  :  ");
        //}
        MMatrix matrix(myMatrix);

        matrix = matrix.inverse();
        MObject oMatrix = fnMatrixData.create(matrix);
        plugMatrix.elementByLogicalIndex(i).setMObject(oMatrix);
    }
    MFnPointArrayData fnPointData;
    MObject oPointData = fnPointData.create(basePoints);
    MPlug plugDeformedPoints = fnDeformer.findPlug("deformedPoints", false);
    plugDeformedPoints.setMObject(oPointData);
    return MS::kSuccess;
}

void *ShapeInverterCmd::creator()
{
    return new ShapeInverterCmd;
}