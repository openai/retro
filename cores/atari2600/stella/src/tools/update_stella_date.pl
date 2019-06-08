#!/usr/bin/perl

die if(@ARGV != 1);

open( FILES, $ARGV[0]);
@files = <FILES>;
close( FILES );

foreach $infile (@files)
{
  chomp $infile;
  print "Processing file $infile\n";
  $outfile = $infile . "___";

  open(IN, "$infile");
  open(OUT, ">$outfile");

  # Update date
  foreach $line (<IN>)
  {
    $line =~ s/1995-2014/1995-2013/;
    print OUT $line;
  }

  close(IN);
  close(OUT);

  # Put back into original file
  system( "mv \"$outfile\" \"$infile\"" );
}
