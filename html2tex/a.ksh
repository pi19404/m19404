cp ./head.html $2 
./delatex $1 >> $2
echo "</html>" >> $2
