# On main branch
git add .
git commit -m "Add new feature with documentation"

# On submission branch
git checkout submission
git merge main
# or: git rebase main
Then clean up comments in the new commits only.
