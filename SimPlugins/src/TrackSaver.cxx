#include "SimPlugins/TrackSaver.h"

namespace ldmx { 

    extern "C" TrackSaver* createTrackSaver() {
        return new TrackSaver;
    }

    extern "C" void destroyTrackSaver(TrackSaver* object) {
        delete object;
    }

    TrackSaver::TrackSaver() { 
    }

    TrackSaver::~TrackSaver() { 
    }

    void TrackSaver::preTracking(const G4Track* aTrack) {
        
        std::cout << "[preTracking] particle = " << aTrack->GetDynamicParticle()->GetPDGcode() << ", energy = " << aTrack->GetKineticEnergy();
        std::cout << ", vol = " << aTrack->GetLogicalVolumeAtVertex()->GetName() << ", next vol = " << aTrack->GetNextVolume()->GetName(); 
        std::cout << ", id = " << aTrack->GetTrackID() << ", parent id  = " << aTrack->GetParentID() << std::endl;
        UserTrackInformation* userInfo = dynamic_cast<UserTrackInformation*>(aTrack->GetUserInformation());
        if (!userInfo->getSaveFlag()) {
            userInfo->setSaveFlag(true);
        }        

    }    
    
    void TrackSaver::stepping(const G4Step* step) { 

        // Get the track associated with this step.
        G4Track* track = step->GetTrack();
        std::cout << "[stepping] particle = " << track->GetDynamicParticle()->GetPDGcode() << ", energy = " << track->GetKineticEnergy();
        if (track->GetVolume() != NULL) std::cout << ", vol = " << track->GetVolume()->GetName();
        if (track->GetNextVolume() != NULL) std::cout << ", next vol = " << track->GetNextVolume()->GetName(); 
        std::cout << ", id = " << track->GetTrackID() << ", parent id  = " << track->GetParentID() << std::endl;
        
        // this doesn't ever make it to the post user tracking action =(
        UserTrackInformation* userInfo = dynamic_cast<UserTrackInformation*>(track->GetUserInformation());
        if (!userInfo->getSaveFlag()) {
            userInfo->setSaveFlag(true);
        }   

    }
}



