rm -rf .git
git init
git add .
git commit -m "first commit"
git remote add origin git@github.com:jonntd/cvShapeInverter.git

#git push -f -u origin master
#git checkout master

git tag v0.2.0
git push --porcelain --progress --recurse-submodules=check origin refs/tags/v0.2.0:refs/tags/v0.2.0
