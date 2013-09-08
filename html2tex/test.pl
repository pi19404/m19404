#!/usr/bin/perl -w

`./delatex test/in > /tmp/testDelatex.txt`;
my $diffResult = `diff test/correct.txt /tmp/testDelatex.txt`;

if ($diffResult eq '') {
	print "Test ok\n";
	exit(0);
} else {
	print "Test failed:\n";
	#print $diffResult;
	my $compared = "test/correct.txt /tmp/testDelatex.txt";
	if (`which kdiff3`) {
		system("kdiff3 $compared");
	} elsif (`which vimdiff`) {
		system("vimdiff $compared");
	} else {
		system("diff -u $compared");
	}
	exit(11);
}
