#!/bin/sh

#usage: source MakeAnalysis.sh <AnalysisName> <Github username> 

export ANALYSISNAME=$1

git clone https://github.com/UHHAnalysis/ExampleAnalysis ${SFRAME_DIR}/${ANALYSISNAME}Analysis

cd ${SFRAME_DIR}/${ANALYSISNAME}Analysis
rm -rf .git
sed -i "s|Example|${ANALYSISNAME}|g" include/Example{Cycle,Hists}.h include/ExampleAnalysis_LinkDef.h
sed -i "s|Example|${ANALYSISNAME}|g" src/Example{Cycle,Hists}.cxx
sed -i "s|Example|${ANALYSISNAME}|g" proof/SETUP.C
sed -i "s|Example|${ANALYSISNAME}|g" Makefile
sed -i "s|Example|${ANALYSISNAME}|g" config/ExampleCycle_config.xml
sed -i "s|Example|${ANALYSISNAME}|g" config/ExampleCycleNew_config.xml

mv include/ExampleHists.h include/${ANALYSISNAME}Hists.h
mv include/ExampleHistsNew.h include/${ANALYSISNAME}HistsNew.h
mv include/ExampleCycle.h include/${ANALYSISNAME}Cycle.h
mv include/ExampleCycleNew.h include/${ANALYSISNAME}CycleNew.h
mv src/ExampleHists.cxx src/${ANALYSISNAME}Hists.cxx
mv src/ExampleCycle.cxx src/${ANALYSISNAME}Cycle.cxx
mv src/ExampleHistsNew.cxx src/${ANALYSISNAME}HistsNew.cxx
mv src/ExampleCycleNew.cxx src/${ANALYSISNAME}CycleNew.cxx
mv config/ExampleCycle_config.xml config/${ANALYSISNAME}_config.xml
mv include/ExampleAnalysis_LinkDef.h include/${ANALYSISNAME}Analysis_LinkDef.h
mv config/ExampleCycleNew_config.xml config/${ANALYSISNAME}New_config.xml

if [ "$#" -eq 2 ] ; then
    
    git init
    #curl -F 'login=$2' -F 'token=$3' https://github.com/api/v3/json/repos/create -F 'name=${ANALYSISNAME}Analysis'
    curl -u $2 https://api.github.com/user/repos -d '{"name":"'${ANALYSISNAME}Analysis'"}'
    git remote add origin https://github.com/$2/${ANALYSISNAME}Analysis.git
    #git update-server-info
    git add *
    git commit -m "first commit"
    git push origin master 
fi

make -j 2
