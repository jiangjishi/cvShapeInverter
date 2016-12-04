@echo off
cd /d D:\cvShapeInverter\ShapeInverter 
set "name=C:\Program Files (x86)\MSBuild\12.0\Bin\MsBuild.exe"
echo Building Entities.sln, please wait a minute...
"%name%" ShapeInverter.sln /t:rebuild /p:configuration=Release /p:platform=x64 /maxcpucount:5
echo Building Entities.sln Complete!
rd 2011 /s/q
rd 2012 /s/q
rd 2013 /s/q
rd 2014 /s/q
rd 2015 /s/q
rd 2016 /s/q
rd Release /s/q
rd Debug  /s/q
rd ShapeInverter.log /s/q
rd ShapeInverter.sdf /s/q
rd ShapeInverter.sln.cache /s/q
rd ShapeInverter.vcxproj.user /s/q
rd ShapeInverter.v12.suo /s/
rd *.vcxproj.user /s/q
rd *.v12.suo /s/
del  /f /s /q *.log
del  /f /s /q *.sdf
del  /f /s /q *.suo
del  /f /s /q *.cache
del  /f /s /q *.vcxproj.user
del  /f /s /q *.v12.suo