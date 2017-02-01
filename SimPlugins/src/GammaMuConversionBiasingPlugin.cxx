/**
 * @file GammaMuConversionBiasingPlugin.h
 * @brief Run action plugin that biases the Geant4 photonuclear xsec by a user
 *        specified value. 
 * @author Omar Moreno
 *         SLAC National Accelerator Laboratory
 */

#include "SimPlugins/GammaMuConversionBiasingPlugin.h"

extern "C" ldmx::GammaMuConversionBiasingPlugin* createGammaMuConversionBiasingPlugin() {
    return new ldmx::GammaMuConversionBiasingPlugin;
}

extern "C" void destroyGammaMuConversionBiasingPlugin(ldmx::GammaMuConversionBiasingPlugin* object) {
    delete object;
}


ldmx::GammaMuConversionBiasingPlugin::GammaMuConversionBiasingPlugin() {
}

ldmx::GammaMuConversionBiasingPlugin::~GammaMuConversionBiasingPlugin() { 
    delete messenger_;
}

void ldmx::GammaMuConversionBiasingPlugin::beginRun(const G4Run*) { 
    
    // Get the process manager associated with reactions involving photons.
    G4ProcessManager* pm = G4Gamma::GammaDefinition()->GetProcessManager();

    // Get the list of available processes.
    G4ProcessVector* processes = pm->GetProcessList();

    // Bias the photonuclear process by the user specified factor.
    std::cout << "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL" << std::endl;
    std::cout << "In andrew and nhan's super cool new plugin!!!!" << std::endl;
    for (int processIndex = 0; processIndex < processes->entries(); processIndex++) {
        
        G4VProcess* process = (*processes)[processIndex];
        
        std::cout << "list of processes = " << process->GetProcessName() << std::endl;

        // if (process->GetProcessName().compareTo("photonNuclear") == 0) {
        
        //     ((G4HadronicProcess*) process)->BiasCrossSectionByFactor(xsecBiasingFactor_);
            
        //     std::cout << "[ GammaMuConversionBiasingPlugin ]: "
        //               << "Photonuclear xsec has increased by a factor of " 
        //               << xsecBiasingFactor_ << std::endl;
        // }

        if (process->GetProcessName().compareTo("GammaToMuPair") == 0) {
        
            ((G4GammaConversionToMuons*) process)->SetCrossSecFactor(xsecBiasingFactor_);
            
            std::cout << "[ GammaMuConversionBiasingPlugin ]: "
                      << "Photonuclear xsec has increased by a factor of " 
                      << xsecBiasingFactor_ << std::endl;
        }        
    }
}
