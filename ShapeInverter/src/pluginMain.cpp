//
// Copyright (C) jonn
//
// blog: http://gobrother.net
// email: jonn@gobrother.net
//

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include "ShapeInverterCmd.h"
#include "ShapeInverterNode.h"
#define VERSION		"3.00"

MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "blog: http://gobrother.net  email: jonn@gobrother.net ", "3.0", "Any");
    status = plugin.registerNode("ShapeInverter", ShapeInverter::id, ShapeInverter::creator, ShapeInverter::initialize, MPxNode::kDeformerNode);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = plugin.registerCommand("ShapeInverterCmd", ShapeInverterCmd::creator, ShapeInverterCmd::newSyntax);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MGlobal::displayInfo(MString("ShapeInverter  ") + MString(VERSION) + MString(" - Built On: ") + MString(__DATE__) + MString(" ") + MString(__TIME__));
    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj);
    status = plugin.deregisterNode(ShapeInverter::id);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return status;
}