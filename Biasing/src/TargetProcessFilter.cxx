/**
 * @file TargetPhotonuclearBiasing.cxx
 * @brief User action plugin that biases Geant4 to only process events which
 *        involve a photonuclear reaction in the target.
 * @author Omar Moreno
 *         SLAC National Accelerator Laboratory
 */

#include "Biasing/TargetProcessFilter.h"

namespace ldmx { 

    extern "C" TargetProcessFilter* createTargetProcessFilter() {
        return new TargetProcessFilter;
    }

    extern "C" void destroyTargetProcessFilter(TargetProcessFilter* object) {
        delete object;
    }


    TargetProcessFilter::TargetProcessFilter() { 
    }

    TargetProcessFilter::~TargetProcessFilter() { 
    }


    /*
    G4ClassificationOfNewTrack TargetProcessFilter::stackingClassifyNewTrack(
            const G4Track* track, 
            const G4ClassificationOfNewTrack& currentTrackClass) {

        std::cout << "********************************" << std::endl; 
        std::cout << "*   Track pushed to the stack  *" << std::endl;
        std::cout << "********************************" << std::endl; 

        // get the PDGID of the track.
        G4int pdgID = track->GetParticleDefinition()->GetPDGEncoding();

        // Get the particle type.
        G4String particleName = track->GetParticleDefinition()->GetParticleName();

        std::cout << "[ TargetProcessFilter ]: " << "\n" 
                    << "\tParticle " << particleName      << " ( PDG ID: " << pdgID << " ) : " << "\n"
                    << "\tTrack ID: " << track->GetTrackID()     << "\n" 
                    << std::endl;


        // Use current classification by default so values from other plugins are not overridden.
        G4ClassificationOfNewTrack classification = currentTrackClass;

        if (track->GetTrackID() == 1 && pdgID == 11) {
            return fWaiting; 
        }

        return classification;
    }*/

    void TargetProcessFilter::stepping(const G4Step* step) { 


        std::cout << "************" << std::endl; 
        std::cout << "*   Step   *" << std::endl;
        std::cout << "************" << std::endl; 


        // Get the track associated with this step.
        G4Track* track = step->GetTrack();

        // get the PDGID of the track.
        G4int pdgID = track->GetParticleDefinition()->GetPDGEncoding();

        // Get the particle type.
        G4String particleName = track->GetParticleDefinition()->GetParticleName();

        // Get the volume the particle is in.
        G4VPhysicalVolume* volume = track->GetVolume();
        G4String volumeName = volume->GetName();


        // Get the kinetic energy of the particle.
        double incidentParticleEnergy = step->GetPreStepPoint()->GetTotalEnergy();

        std::cout << "[ TargetProcessFilter ]: " << "\n" 
                    << "\tTotal energy of " << particleName      << " ( PDG ID: " << pdgID
                    << " ) : " << incidentParticleEnergy       << "\n"
                    << "\tTrack ID: " << track->GetTrackID()     << "\n" 
                    << "\tStep #: " << track->GetCurrentStepNumber() << "\n"
                    << "\tParticle currently in " << volumeName  << std::endl;

        // Get the particles daughters.
        const G4TrackVector* secondaries = step->GetSecondary();

        if (track->GetTrackID() == 2 && pdgID == 22  && volumeName.compareTo(volumeName_) == 0) {

            // If the brem photon doesn't undergo any reaction in the target, stop
            // processing the rest of the event. 
            if (secondaries->size() == 0) {


                std::cout << "[ TargetProcessFilter ]: "
                            << "Brem photon did not interact in the target. --> Killing all tracks!"
                            << std::endl;

                track->SetTrackStatus(fKillTrackAndSecondaries);
                G4RunManager::GetRunManager()->AbortEvent();
                return;
            }  

            G4String processName = secondaries->at(0)->GetCreatorProcess()->GetProcessName(); 


            std::cout << "[ TargetProcessFilter ]: "
                        << "Brem photon produced " << secondaries->size() 
                        << " particle via " << processName << " process." 
                        << std::endl;


            // Only record photonuclear events
            if (!processName.contains(BiasingMessenger::getProcess())) {


                std::cout << "[ TargetProcessFilter ]: "
                  << "Process was not photonuclear. --> Killing all tracks!" 
                  << std::endl; 

                track->SetTrackStatus(fKillTrackAndSecondaries);

                G4RunManager::GetRunManager()->AbortEvent();
                return;
            }

            std::cout << "[ TargetProcessFilter ]: "
                << "Brem photon produced " << secondaries->size() 
                << " particle via " << processName << " process." 
                << std::endl;
        }
    }

}

