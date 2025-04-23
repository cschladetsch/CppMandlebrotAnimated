# Update the existing remote with the correct SSH URL
git remote set-url origin git@github.com:cschladetsch/CppMandlebrotAnimated.git
# Then force push to replace everything
git add .
git commit -m "Complete code replacement"
git push -f origin master
