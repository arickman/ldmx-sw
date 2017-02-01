/**
 * @file GammaMuConversionBiasingMessenger.h
 * @brief Class providing a macro messenger for a GammaMuConversionBiasingPlugin
 * @author Jeremy McCormick, SLAC National Accelerator Laboratory
 */

#ifndef SIMPLUGINS_GAMMAMUCONVERSIONBIASINGPLUGINMESSENGER_H
#define SIMPLUGINS_GAMMAMUCONVERSIONBIASINGPLUGINMESSENGER_H

// LDMX
#include "SimPlugins/UserActionPluginMessenger.h"

namespace ldmx {

// Forward declare to avoid circular dependency in headers.
class GammaMuConversionBiasingPlugin;

/**
 * @class GammaMuConversionBiasingMessenger
 * @brief Messenger for setting parameters on GammaMuConversionBiasingPlugin
 */
class GammaMuConversionBiasingMessenger : UserActionPluginMessenger {

    public:

        /**
         * Class constructor.
         * @param plugin The associated GammaMuConversionBiasingPlugin object.
         */
        GammaMuConversionBiasingMessenger(GammaMuConversionBiasingPlugin* plugin);

        /**
         * Class destructor.
         */
        virtual ~GammaMuConversionBiasingMessenger() {
            delete xsecFactorCmd_;
        }

        /**
         * Process the macro command.
         * @param[in] command The macro command.
         * @param[in] newValues The argument values.
         */
        void SetNewValue(G4UIcommand *command, G4String newValue);

    private:

        /**
         * The associated GammaMuConversionBiasingPlugin object.
         */
        GammaMuConversionBiasingPlugin* biasingPlugin_;

        /**
         * The command for setting the cross-section biasing factor.
         */
        G4UIcommand* xsecFactorCmd_;
};

}

#endif
