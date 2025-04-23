# Add or update the remote with SSH URL
git remote add origin git@github.com:git@github.com:cschladetsch/CppMandlebrotAnimated.git

# Then force push to replace everything
git add .
git commit -m "Complete code replacement"
git push -f origin main

