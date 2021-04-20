#!/usr/bin/env bash

WORKING_DIR=$(pwd)

echo "Insert the file that contains the list of hipo files: "
read FILE
TREE=LTCCefficiency_tree_$FILE.root
OUTPDF1=out1D_$FILE.pdf
OUTPDF2=out2D_$FILE.pdf
OUTROOT=out_$FILE.root

# check if the output files of makeHistos are already present
# and ask the user if they wants to delete or not the files.
if [[ -s $WORKING_DIR/$OUTPDF1 ]] && [[ -s $WORKING_DIR/$OUTPDF2 ]] && [[ -s $WORKING_DIR/$OUTROOT ]]; then
	echo "Files containing histograms already exists.";
	echo "The following files were found: ";
		find $WORKING_DIR -iname $OUTPDF1
		find $WORKING_DIR -iname $OUTPDF2
		find $WORKING_DIR -iname $OUTROOT
	echo -e "Do you want to remove them and produce new histograms? (Y/n): \c"
	read REMOVE
	if [[ $REMOVE = 'n' ]]; then
		echo -e "Do you want to continue anyway running the code? (Y/n): \c"
		read CONTINUE
		if [[ $CONTINUE = 'n' ]]; then
			echo "The script will terminate.";
			exit 0
		elif [[ $CONTINUE = 'Y' ]]; then
			echo "The script will be executed...";
		fi
	elif [[ $REMOVE = 'Y' ]]; then
		rm $OUTPDF1
		rm $OUTPDF2
		rm $OUTROOT
		echo "The files were correctly removed.";
		echo "The script will be now executed...";
	fi
fi

# execute LTCCefficiency and/or makeHistos
# depending on the presence of already existing TTree file.
if [[ -e $WORKING_DIR/$TREE ]]; then              
	echo "The TTree relative to this list is already present.";
	echo $TREE "will be used to produce the histograms.";
	root -l -q 'makeHistos.cxx("'$TREE'")' 
elif ! [[ -e $WORKING_DIR/$TREE ]] && [[ -s $WORKING_DIR/$FILE.dat ]]; then
	echo "Creating the TTree from hipo files of list" $FILE".dat.";
	echo "The operation will require some minutes...";
	time clas12root -b -q LTCCefficiency.cxx --in=$FILE.dat
	echo $TREE "will be now used to produce the histograms.";
	root -l -q 'makeHistos.cxx("'$TREE'")'
else
	echo "Please, insert a valid list of hipo file (Format: .dat).";
	exit 0
fi