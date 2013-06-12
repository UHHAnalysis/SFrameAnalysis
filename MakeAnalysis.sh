#!/bin/sh

#usage: source MakeAnalysis.sh <AnalysisName>

export ANALYSISNAME=$1

git clone https://github.com/UHHAnalysis/ExampleAnalysis ${SFRAME_DIR}/${ANALYSISNAME}Analysis

cd ${SFRAME_DIR}/${ANALYSISNAME}Analysis
rm -rf .git
sed -i "s|Example|${ANALYSISNAME}|g" include/*h
sed -i "s|Example|${ANALYSISNAME}|g" src/*cxx
sed -i "s|Example|${ANALYSISNAME}|g" proof/SETUP.C
sed -i "s|Example|${ANALYSISNAME}|g" Makefile
sed -i "s|Example|${ANALYSISNAME}|g" config/ExampleCycle_config.xml

mv include/ExampleHists.h include/${ANALYSISNAME}Hists.h
mv include/ExampleCycle.h include/${ANALYSISNAME}Cycle.h
mv src/ExampleHists.cxx src/${ANALYSISNAME}Hists.cxx
mv src/ExampleCycle.cxx src/${ANALYSISNAME}Cycle.cxx
mv config/ExampleCycle_config.xml config/${ANALYSISNAME}_config.xml
mv include/ExampleAnalysis_LinkDef.h include/${ANALYSISNAME}Analysis_LinkDef.h
#make -j 2