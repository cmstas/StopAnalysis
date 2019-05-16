# Batch mode for Stop Babymaker 
1.  make sure babymaker is compiled (you need the runBabyMaker exe)
2.  make new input directory
mkdir input    
3.  change the METIS_PATH in setup.sh to blank
4.  make sure you define the datasets and babynames in sntSamples.py
5.  run setup script:
. setup.sh
6.  Now you should have a ProjectMetisfolder, merge_scripts.tar.gz, and input.tar.gz
7.  Make your proxy valid for many hours
voms-proxy-init --voms cms -hours 5000`
8.  start a screen session to run metis:
screen
python sntSamples.py
# then hit CtrlA and then hit d
9.  Go drink a nice IPA/Chocolate
10. Check on your babymaking:

11. If need to go back into the screen
screen -ls
screen -r <one of the ids from the ls>

