cp ./head.html $2.html 
./comments $1 > $2.tex
./delatex $2 >> $2.html
echo "</html>" >> $2.html
