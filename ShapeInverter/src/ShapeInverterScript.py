import maya.cmds as mc

def getShape(node):
    if mc.nodeType(node) == 'transform':
        shapes = mc.listRelatives(node, shapes=True)
        if not shapes:
            raise RuntimeError, '%s has no shape' % node
        # end if
        return shapes[0]
    elif mc.nodeType(node) in ['mesh', 'nurbsCurve', 'nurbsSurface']:
        return node
        
def duplicateMesh(base=None,name="duplicateMesh#"):
    invertedShape = mc.duplicate(base,name =name )[0]
    shapes = mc.listRelatives(invertedShape, children=True, shapes=True)
    for s in shapes:
        if mc.getAttr('%s.intermediateObject' % s):
            mc.delete(s)
    for attr in 'trs':
        for x in 'xyz':
            mc.setAttr('%s.%s%s' % (invertedShape, attr, x), lock=False)
    mc.setAttr('%s.visibility' % invertedShape, 1)
    return invertedShape

def transferMesh(original,targets):
    #transferMesh("duplicateMesh1",["pSphere2_inverted1",'pSphere2_inverted12'])
    outtargets=[]
    for trg in targets:
        bldShp = mc.blendShape(original,trg)[0]
        mc.setAttr('%s.%s' % (bldShp, original), 1)
        mc.delete(trg,ch=1)
        outtargets.append(trg)
    return outtargets
    
def ShapeInverterCmdold(base=None, corrective=None, name=None):
    mc.undoInfo(openChunk=True)
    if not base or not corrective:
        sel = mc.ls(sl=True)
        base, corrective = sel
    shapes = mc.listRelatives(base, children=True, shapes=True)
    for s in shapes:
        if mc.getAttr('%s.intermediateObject' % s) and mc.listConnections('%s.worldMesh' % s,source=False):
            origMesh = s
            break
    deformed = mc.polyPlane(ch=False)[0]
    mc.connectAttr("%s.worldMesh"%origMesh,"%s.inMesh"%deformed)
    mc.setAttr("%s.intermediateObject"%origMesh,0)
    mc.delete(deformed,ch=True)
    mc.setAttr("%s.intermediateObject"%origMesh,1)
    if not name:
        name = '%s_inverted#' % corrective
    invertedShape = duplicateMesh(base, name=name)
    deformer = mc.deformer(invertedShape, type='ShapeInverter')[0]
    mc.ShapeInverterCmd(baseMesh = base,invertedShape = invertedShape,ShapeInverterdeformer = deformer,origMesh = deformed)
    #correctiveShape = duplicateMesh(base,name=corrective+"_corrective#")
    #mc.connectAttr('%s.outMesh' % getShape(correctiveShape), '%s.correctiveMesh' % deformer)
    #transferMesh(corrective,[correctiveShape])
    mc.connectAttr('%s.outMesh' % getShape(corrective), '%s.correctiveMesh' % deformer)
    mc.setAttr('%s.activate' % deformer, True)
    mc.delete(deformed)
    bdingBx = mc.polyEvaluate(corrective,boundingBox=True)
    xDifVal = bdingBx[0][1]-bdingBx[0][0]
    #mc.move(xDifVal*1.10,correctiveShape,r=True,moveX=True)
    mc.move(xDifVal*2.20,invertedShape,r=True,moveX=True)
    mc.undoInfo(closeChunk=True)
    return invertedShape#,correctiveShape


def ShapeInverterCmd(base=None, corrective=None):

    mc.undoInfo(openChunk=True)
    if not base or not corrective:
        sel = mc.ls(sl=True)
        base, corrective = sel
    shapes = mc.listRelatives(base, children=True, shapes=True)
    for s in shapes:
        if mc.getAttr('%s.intermediateObject' % s) and mc.listConnections('%s.worldMesh' % s,source=False):
            origMesh = s
            break
 
    shapes = mc.listRelatives(base, children=True, shapes=True)
    for s in shapes:
        if mc.getAttr('%s.intermediateObject' % s) and mc.listConnections('%s.worldMesh' % s,source=False):
            origMesh = s
            break
    deformed = mc.polyPlane(ch=False)[0]
    mc.connectAttr("%s.worldMesh"%origMesh,"%s.inMesh"%deformed)
    mc.setAttr("%s.intermediateObject"%origMesh,0)
    mc.delete(deformed,ch=True)
    mc.setAttr("%s.intermediateObject"%origMesh,1)
    name = '%s_inverted#' % corrective
    invertedShape = duplicateMesh(base, name=name)
    deformer = mc.deformer(invertedShape, type='ShapeInverter')[0]
    newBlsNode = mc.blendShape(deformed,base,frontOfChain=True,w=[0,1])
    mc.connectAttr('%s.outMesh' % getShape(corrective), '%s.correctiveMesh' % deformer)
    mc.ShapeInverterCmd(baseMesh = base,invertedShape = invertedShape,ShapeInverterdeformer = deformer,origMesh = deformed)
    mc.setAttr('%s.activate' % deformer, True)
    mc.delete(deformed,newBlsNode)
    bdingBx = mc.polyEvaluate(corrective,boundingBox=True)
    xDifVal = bdingBx[0][1]-bdingBx[0][0]
    #mc.move(xDifVal*1.10,correctiveShape,r=True,moveX=True)
    mc.move(xDifVal*2.20,invertedShape,r=True,moveX=True)
    mc.undoInfo(closeChunk=True)
    return invertedShape#,correctiveShape


'''  
import ShapeInverterScript as ShapeInverterScript

# select skinModel add select correctiveModel
ShapeInverterScript.ShapeInverterCmd()
'''    
    