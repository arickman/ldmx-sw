/**
 * @file PrimaryGeneratorAction.h
 * @brief Class implementing the Geant4 primary generator action
 * @author Jeremy McCormick, SLAC National Accelerator Laboratory
 */

#ifndef SIMAPPLICATION_PRIMARYGENERATORACTION_H_
#define SIMAPPLICATION_PRIMARYGENERATORACTION_H_

//------------//
//   Geant4   //
//------------//
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

//----------//
//   ROOT   //
//----------//
#include "TRandom.h"

//-------------//
//   LDMX-SW   //
//-------------//
#include "SimPlugins/PluginManagerAccessor.h"
#include "SimApplication/MultiParticleGunPrimaryGenerator.h"
#include "SimApplication/PrimaryGeneratorMessenger.h"
#include "SimApplication/UserPrimaryParticleInformation.h"

namespace ldmx {

    /**
     * @class PrimaryGeneratorAction
     * @brief Implementation of Geant4 primary generator action
     */
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction, public PluginManagerAccessor {

        public:

            /**
             * Class constructor.
             */
            PrimaryGeneratorAction();

            /**
             * Class destructor.
             */
            virtual ~PrimaryGeneratorAction();

            /**
             * Generate the event.
             * @param anEvent The Geant4 event.
             */
            virtual void GeneratePrimaries(G4Event* anEvent);

            /**
             * Set the primary generator.
             * @param primaryGenerator The primary generator.
             */
            void setPrimaryGenerator(G4VPrimaryGenerator* primaryGenerator);

            /**
             * Enable beamspot smearing.
             * @param bool
             */
            void setUseBeamspot(bool usebs) { useBeamspot_ = usebs; };

            /**
             * Set beamspot size in x.
             * @param beamspot size
             */
            void setBeamspotXSize(double bssize){ beamspotXSize_ = bssize; };
            
            /**
             * Set beamspot size in y.
             * @param beamspot size
             */
            void setBeamspotYSize(double bssize){ beamspotYSize_ = bssize; };

            /** Return the ith generator. */ 
            G4VPrimaryGenerator* getGenerator(int i){ return generator_.at(i); }

            /** 
             * Get the index of the last generator in the list of 
             * generators.
             */
            int getIndexMPG(){ return indexMpg_; }

        private:

            /**
             * Smearing beamspot
             * @param anEvent The Geant4 event.
             */
            void smearingBeamspot(G4Event* anEvent);

            /**
             * The primary generator.
             */
            std::vector< G4VPrimaryGenerator* > generator_;

            /** Random number generator. */
            TRandom* random_;

            /** 
             * Flag denoting whether the vertex position of a particle 
             * should be smeared.
             */ 
            bool useBeamspot_{false};
            
            /** Extent of the beamspot in x. */
            double beamspotXSize_{20.};            
            
            /** Extent of the beamspot in y. */
            double beamspotYSize_{10.};   

            /** The index of the last generator in the list of generators. */ 
            int indexMpg_{-1};          

    };  // PrimaryGeneratorAction
}
#endif // SIMAPPLICATION_PRIMARYGENERATORACTION_H_
