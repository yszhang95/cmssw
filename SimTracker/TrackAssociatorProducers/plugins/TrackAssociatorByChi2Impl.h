#ifndef TrackAssociatorByChi2Impl_h
#define TrackAssociatorByChi2Impl_h

/** \class TrackAssociatorByChi2Impl
 *  Class that performs the association of reco::Tracks and TrackingParticles evaluating the chi2 of reco tracks parameters and sim tracks parameters. The cut can be tuned from the config file: see data/TrackAssociatorByChi2.cfi. Note that the Association Map is filled with -ch2 and not chi2 because it is ordered using std::greater: the track with the lowest association chi2 will be the first in the output map.It is possible to use only diagonal terms (associator by pulls) seeting onlyDiagonal = true in the PSet 
 *
 *  \author cerati, magni
 */

#include "SimDataFormats/Associations/interface/TrackToTrackingParticleAssociatorBaseImpl.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include <map>

//Note that the Association Map is filled with -ch2 and not chi2 because it is ordered using std::greater:
//the track with the lowest association chi2 will be the first in the output map.

namespace edm {
  class EDProductGetter;
}

namespace reco {
  typedef edm::AssociationMap<
      edm::OneToManyWithQualityGeneric<reco::GenParticleCollection, edm::View<reco::Track>, double> >
      GenToRecoCollection;
  typedef edm::AssociationMap<
      edm::OneToManyWithQualityGeneric<edm::View<reco::Track>, reco::GenParticleCollection, double> >
      RecoToGenCollection;
}  // namespace reco

class TrackAssociatorByChi2Impl : public reco::TrackToTrackingParticleAssociatorBaseImpl {
public:
  typedef std::map<double, SimTrack> Chi2SimMap;
  typedef std::pair<reco::Track, Chi2SimMap> RecoToSimPair;
  typedef std::vector<RecoToSimPair> RecoToSimPairAssociation;

  /*
  /// Constructor with PSet
  TrackAssociatorByChi2Impl(const edm::ESHandle<MagneticField> mF, const edm::ParameterSet& conf):
    chi2cut(conf.getParameter<double>("chi2cut")),
    onlyDiagonal(conf.getParameter<bool>("onlyDiagonal")),
    bsSrc(conf.getParameter<edm::InputTag>("beamSpot")) {
    mF_=mF;  
    if (onlyDiagonal)
      edm::LogInfo("TrackAssociator") << " ---- Using Off Diagonal Covariance Terms = 0 ---- " <<  "\n";
    else 
      edm::LogInfo("TrackAssociator") << " ---- Using Off Diagonal Covariance Terms != 0 ---- " <<  "\n";
  }
  */

  /// Constructor
  TrackAssociatorByChi2Impl(edm::EDProductGetter const& productGetter,
                            const MagneticField& mF,
                            const reco::BeamSpot& bs,
                            double chi2Cut,
                            bool onlyDiag)
      : productGetter_(&productGetter), mF_(&mF), beamSpot_(&bs), chi2cut_(chi2Cut), onlyDiagonal_(onlyDiag) {}

  /// Association Reco To Sim with Collections

  reco::RecoToSimCollection associateRecoToSim(const edm::RefToBaseVector<reco::Track>&,
                                               const edm::RefVector<TrackingParticleCollection>&) const override;
  /// Association Sim To Reco with Collections

  reco::SimToRecoCollection associateSimToReco(const edm::RefToBaseVector<reco::Track>&,
                                               const edm::RefVector<TrackingParticleCollection>&) const override;

  /// compare reco to sim the handle of reco::Track and TrackingParticle collections

  reco::RecoToSimCollection associateRecoToSim(const edm::Handle<edm::View<reco::Track> >& tCH,
                                               const edm::Handle<TrackingParticleCollection>& tPCH) const override {
    return TrackToTrackingParticleAssociatorBaseImpl::associateRecoToSim(tCH, tPCH);
  }

  /// compare reco to sim the handle of reco::Track and TrackingParticle collections

  reco::SimToRecoCollection associateSimToReco(const edm::Handle<edm::View<reco::Track> >& tCH,
                                               const edm::Handle<TrackingParticleCollection>& tPCH) const override {
    return TrackToTrackingParticleAssociatorBaseImpl::associateSimToReco(tCH, tPCH);
  }

private:
  edm::EDProductGetter const* productGetter_;
  const MagneticField* mF_;
  const reco::BeamSpot* beamSpot_;
  double chi2cut_;
  bool onlyDiagonal_;
};

#endif
