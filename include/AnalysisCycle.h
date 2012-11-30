// Dear emacs, this is -*- c++ -*-
// $Id: AnalysisCycle.h,v 1.7 2012/11/02 10:02:15 peiffer Exp $
#ifndef AnalysisCycle_H
#define AnalysisCycle_H

// SFrame include(s):
#include "core/include/SCycleBase.h"
#include "Selection.h"
#include "PUWeightProducer.h"
#include "LuminosityHandler.h"
#include "BaseHists.h"
#include "MCDataScaleFactors.h"

/**
 *   @short Base of every analysis cycle 
 *          This is the mother of every analysis cycle.
 *          It handles the read out of all variables from
 *          the Ntuple root files, keeps track of all 
 *          selection and histogram collections, 
 *          handles the luminosity and performs the 
 *          good-run selection.
 *          User cycles specific to a certain analysis
 *          should inherit from this class.
 *
 *  @author Roman Kogler
 *  @version $Revision: 1.7 $
 */

class AnalysisCycle : public SCycleBase {

public:
  /// Default constructor
  AnalysisCycle();
  /// Default destructor
  ~AnalysisCycle();

  /// add a new selection to the analysis
  void RegisterSelection(Selection* sel);

  /// add a new histogram collection to the analysis
  void RegisterHistCollection(BaseHists* hists);

  /// returns booked selection
  Selection* GetSelection(const std::string name);

  /// returns booked histogram collection
  BaseHists* GetHistCollection(const std::string name);

  /// print out information of the selections
  void PrintSelectionSummary();

  /// reset the cut-flows of all selections
  void ResetSelectionStats();

  /// calls init for each histogram collection
  void InitHistos();
  
  /// calls finalise for each histogram collection
  void FinaliseHistos();
  
  /// Function to set the integrated luminosity per bin
  void SetIntLumiPerBin(double int_lumi){m_int_lumi_per_bin = int_lumi;}

  /// Function called at the beginning of the cycle
  void BeginCycle() throw( SError );
  /// Function called at the end of the cycle
  void EndCycle() throw( SError );

  /// Function called at the beginning of a new input data
  void BeginInputData( const SInputData& ) throw( SError );
  /// Function called after finishing to process an input data
  void EndInputData  ( const SInputData& ) throw( SError );

  /// Function called after opening each new input file
  void BeginInputFile( const SInputData& ) throw( SError );

  /// Function called for every event
  void ExecuteEvent( const SInputData&, Double_t ) throw( SError );

  /// Function called at the end of every event to store selected data
  void WriteOutputTree() throw( SError );

  /// returns whether generator info is added or not (automatically determined if data type is 'data')
  bool addGenInfo() {return m_addGenInfo;}

protected:

  // JEC information
  std::string m_JECFileLocation;
  std::string m_JECDataGlobalTag;
  std::string m_JECMCGlobalTag;
  std::string m_JECJetCollection;

private:

  // wrapper to LuminosityHandler
  LuminosityHandler *LumiHandler() { return (LuminosityHandler*)GetConfigObject("LuminosityHandler"); }
  double m_int_lumi_per_bin; // the integrated luminosity per bin for the lumi handler

  // lumi file properties
  std::string m_lumifile_path;
  std::string m_lumifile_name;
  std::string m_lumi_trigger;

  // PU-reweighting properties
  std::string m_PUFilenameMC;
  std::string m_PUFilenameData;
  std::string m_PUHistnameMC;
  std::string m_PUHistnameData;
  PUWeightProducer* m_puwp;

  // properties of the NTuples
  std::string m_JetCollection;
  std::string m_ElectronCollection;
  std::string m_MuonCollection;
  std::string m_TauCollection;
  std::string m_PhotonCollection;
  std::string m_PrimaryVertexCollection;
  std::string m_METName;
  std::string m_TopJetCollection;
  std::string m_TopJetCollectionGen;
  std::string m_PrunedJetCollection;
  std::string m_GenParticleCollection;
  bool m_addGenInfo;
  bool m_newrun;
  bool m_writeTTbarReco;
  bool m_readTTbarReco;

  //MC-data correction weights
  std::vector<std::string> m_leptonweights;
  LeptonScaleFactors* m_lsf;

  //output variables
  std::vector< Electron > m_output_electrons;  
  std::vector< Muon > m_output_muons;
  std::vector< Tau > m_output_taus;
  std::vector< Photon > m_output_photons;
  std::vector< PrimaryVertex > m_output_pvs;
  std::vector< Jet > m_output_jets;
  std::vector< TopJet > m_output_topjets;
  std::vector< TopJet > m_output_topjetsgen;
  std::vector< TopJet > m_output_prunedjets;
  MET m_output_met;
  std::vector<std::string> m_output_triggerNames;
  std::vector<bool> m_output_triggerResults;
  std::vector< GenParticle > m_output_genparticles;
  GenInfo m_output_genInfo;
  std::vector< ReconstructionHypothesis > m_output_recoHyps;

  // list of the event selections 
  std::vector<Selection*> m_selections;

  // list of the histogram collections
  std::vector<BaseHists*> m_histcollections;

  // important: the container with all variables
  BaseCycleContainer m_bcc;
 
  // Macro adding the functions for dictionary generation
  ClassDef( AnalysisCycle, 0 );

}; // class AnalysisCycle

#endif // AnalysisCycle_H

