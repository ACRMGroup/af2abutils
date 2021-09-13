#!/usr/bin/perl -s
#*************************************************************************
#
#   Program:    makeaf2abpdbs
#   File:       makeaf2abpdbs.pl
#   
#   Version:    V1.0
#   Date:       10.09.21
#   Function:   Create cleaned PDB files of antibodies from AF2
#   
#   Copyright:  (c) Prof. Andrew C. R. Martin, UCL, 2021
#   Author:     Prof. Andrew C. R. Martin
#   Address:    Institute of Structural and Molecular Biology
#               Division of Biosciences
#               University College
#               Gower Street
#               London
#               WC1E 6BT
#   EMail:      andrew@bioinf.org.uk
#               
#*************************************************************************
#
#   This program is not in the public domain, but it may be copied
#   according to the conditions laid out in the accompanying file
#   COPYING.DOC
#
#   The code may be modified as required, but any modifications must be
#   documented so that the person responsible can be identified. If 
#   someone else breaks this code, I don't want to be blamed for code 
#   that does not work! 
#
#   The code may not be sold commercially or included as part of a 
#   commercial product except as described in the file COPYING.DOC.
#
#*************************************************************************
#
#   Description:
#   ============
#
#*************************************************************************
#
#   Usage:
#   ======
#
#*************************************************************************
#
#   Revision History:
#   =================
#
#*************************************************************************
# Add the path of the executable to the library path
use FindBin;
use lib $FindBin::Bin;
# Or if we have a bin directory and a lib directory
#use Cwd qw(abs_path);
#use FindBin;
#use lib abs_path("$FindBin::Bin/../lib");

use strict;

my @inDirs       = ("/home/lilian/af2_models_reduced");
my @outDirsRel   = ("/var/tmp/relaxed");
my @outDirsUnrel = ("/var/tmp/unrelaxed");

CheckDirs(\@inDirs, \@outDirsRel, \@outDirsUnrel);
BuildAllFiles(\@inDirs, \@outDirsRel, \@outDirsUnrel);

#*************************************************************************
sub CheckDirs
{
    my ($aInDirs, $aOutDirsRel, $aOutDirsUnrel) = @_;

    foreach my $inDir (@$aInDirs)
    {
        if(! -d $inDir)
        {
            print STDERR "Error: Input directory does not exist ($inDir)\n";
            exit 1;
        }
    }
    
    foreach my $outDir (@$aOutDirsRel)
    {
        MakeDir($outDir);
    }

    foreach my $outDir (@$aOutDirsUnrel)
    {
        MakeDir($outDir);
    }
}


#*************************************************************************
sub MakeDir
{
    my($dirName) = @_;
    `mkdir -p $dirName`;
    if(! -d $dirName)
    {
        print STDERR "Error: Unable to create output directory ($dirName)\n";
        exit 1;
    }
}


#*************************************************************************
sub BuildAllFiles
{
    my ($aInDirs, $aOutDirsRel, $aOutDirsUnrel) = @_;
    
    my $outDirIndex = 0;
    foreach my $inDir (@$aInDirs)
    {
        my $outDirRel   = $$aOutDirsRel[$outDirIndex];
        my $outDirUnrel = $$aOutDirsUnrel[$outDirIndex];

        my @subdirs = ReadSubDirs($inDir);
        foreach my $subdir (@subdirs)
        {
            my $fullDirName = "$inDir/$subdir";
            BuildFileIfNeeded($outDirRel,   $subdir, $fullDirName, 1);
            BuildFileIfNeeded($outDirUnrel, $subdir, $fullDirName, 0);
        }
        $outDirIndex++;
    }
}


#*************************************************************************
sub ReadSubDirs
{
    my($dirName) = @_;
    my @dirs = ();
    
    if(opendir(my $dh, $dirName))
    {
        @dirs = grep(-D, readdir($dh));
        @dirs = grep(!/^\.\.?$/, @dirs);
        closedir($dh);
    }
    return(@dirs);
}


#*************************************************************************
sub BuildFileIfNeeded
{
    my ($dest, $linkname, $src, $relaxed) = @_;
    my $inFile = $relaxed?"$src/relaxed_model_1.pdb":"$src/unrelaxed_model_1.pdb";
    my $outFile = "$dest/$linkname.pdb";
    
    if(FileNewer($inFile, $outFile) || defined($::f))
    {
        ProcessFile($inFile, $outFile);
    }
}


#*************************************************************************
#> BOOL FileNewer($testFile, $refFile)
#  -----------------------------------
#  Input:   string   $testFile   File of which to check date
#           string   $refFile    Reference file against which we compare
#  Returns: BOOL                 True if either file does not exist or
#                                if $testFile is newer than $refFile
#
#  Tests whether $testFile is newer than $refFile
#
#  19.09.13  Original  By: ACRM
sub FileNewer
{
    my($testFile, $refFile) = @_;

    return(1) if((! -e $testFile) || (! -e $refFile));

    my @stats;
    @stats = stat($testFile);
    my $testDate = $stats[9];
    @stats = stat($refFile);
    my $refDate = $stats[9];

    return((($testDate > $refDate)?1:0));
}


#*************************************************************************
sub ProcessFile
{
    my($inFile, $outFile) = @_;
    my $tFile = "/var/tmp/mad2abp.pdb.$$" . time();
    my $exe = "pdbhstrip $inFile | ./pdbrmseq GGGGSGGGGSGGGGSGGGGS | pdbchain -c L,H > $tFile";
    print "$exe\n" if(defined($::v));
    `$exe`;
    $exe = "pdbabnum $tFile > $outFile";
    print "$exe\n" if(defined($::v));
    `$exe`;
    unlink($tFile);
}

