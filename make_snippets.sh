for f in ./include/*.hpp; do
   snip_file=./.vscode/`basename $f .hpp`.code-snippets
   python3 make_snippets.py $f > $snip_file
done