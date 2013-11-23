<Normal>"\\definecolor"				{ KILLARGS(3); }
<Normal>"\\color"				{ KILLARGS(1); }
<Normal>"\\textcolor"				{ KILLARGS(2); }
<Normal>"\\colorbox"				{ KILLARGS(2); }
<Normal>"\\fcolorbox"				{ KILLARGS(3); }
<Normal>"\\pagecolor"				{ KILLARGS(1); }
<Normal>"\\foilhead"				{ STRIPARGS(1); }
<Normal>"\\addfontfeature"			{ KILLARGS(1); }
<Normal>"\\thispagestyle"			{ KILLARGS(1); }
<Normal>"\\addcontentsline"			{ KILLARGS(3); }
<Normal>"\\automark" {KILLARGS(1); IGNORE;}
<Normal>"\\counterwithout" {KILLARGS(2); IGNORE;}
<Normal>"\\ECFAugie\\color" {KILLARGS(1); IGNORE;}

<Normal>"\\lstset"				{LATEX;KILLARGS(1); IGNORE;}
<Normal>"\\usepackage"				{KILLARGS(1); IGNORE;}
<Normal>"\\setcapwidth" 			{KILLARGS(1); IGNORE;}
<Normal>"\\DeclareFixedFont" 			{KILLARGS(6); IGNORE;}
<Normal>"\\setheadsepline" 			{KILLARGS(1); IGNORE;}
<Normal>"\\deffootnote"				{KILLARGS(2); IGNORE;}
<Normal>"\\end"					{KILLARGS(1); IGNORE;}
<Normal>"\\hypersetup"			{KILLARGS(1);}
<Normal>"\\index"				{KILLARGS(1);}
	/*<Normal>"\\footnote"				{KILLARGS(1); SPACE;}*/
<Normal>"\\label"				{KILLARGS(1); IGNORE;}
<Normal>"\\nameref"					{CITE(1); IGNORE;}
<Normal>"\\pageref"				{CITE(1); IGNORE;}
<Normal>"\\pagestyle"				{KILLARGS(1); IGNORE;}
<Normal>"\\ref"					{CITE(1); IGNORE;}
<Normal>"\\setcounter"				{KILLARGS(2); IGNORE;}

<Normal>"\\fontspec"			{KILLARGS(1);}

<Normal>"\\footnote"(\[([^\]])+\])?"{"		{
							putchar('(');
							footnoteLevel = currBracesLevel;
							++currBracesLevel;
							}
<Normal>"\\verb" /* ignore \verb<ch>...<ch> */	{   if (fLatex) {
						 	char verbchar, c;
						 	verbchar = input();
						 	while ((c = input()) != verbchar)
							    /*if (c == '\n')
								NEWLINE;*/
								putchar(c);
						    }
						    IGNORE;
						}

<Normal>"\\newcommand"				{ LATEX; KILLARGS(2); }
<Normal>"\\renewcommand"			{ LATEX; KILLARGS(2); }
<Normal>"\\newenvironment"			{ LATEX; KILLARGS(3); }

<Normal>"\\def"		/* ignore def begin */	{BEGIN Define; IGNORE;}
<MAIN>"\\addtokomafont" 			{LATEX;KILLARGS(2); IGNORE;}
<MAIN>"\\setkomafont" {LATEX;KILLARGS(2); IGNORE;}
<Normal>"\\DeclareGraphicsExtensions" 		{KILLARGS(1); IGNORE;}
