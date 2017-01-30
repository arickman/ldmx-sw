/**
 * @file HcalDigiProducer.h
 * @brief Class that performs digitization of simulated HCal data
 * @author Andrew Whitbeck, FNAL
 */

#ifndef EVENTPROC_HCALDIGIPRODUCER_H_
#define EVENTPROC_HCALDIGIPRODUCER_H_

// ROOT
#include "TString.h"
#include "TRandom.h"

// LDMX
#include "DetDescr/DetectorID.h"
#include "DetDescr/HcalDetId.h"
#include "Event/SimCalorimeterHit.h"
#include "Framework/EventProcessor.h"

namespace ldmx{

/**
 * @class HcalDigiProducer
 * @brief Performs digitization of simulated HCal data
 */
class HcalDigiProducer : public Producer {

    public:

        typedef int layer;

        typedef std::pair<double, double> zboundaries;

        HcalDigiProducer(const std::string& name, const Process& process);

        virtual ~HcalDigiProducer() {
            delete hits_;
            if (random_)
                delete random_;
        }

        virtual void configure(const ParameterSet&);

        virtual void produce(Event& event);

    private:

        TClonesArray* hits_{nullptr};
        TRandom* random_{0};
        std::map<layer, zboundaries> hcalLayers_;
        bool verbose_{false};
        DetectorID* detID_{nullptr};
        
        static const int NUM_HCAL_LAYERS;
        float meanNoise_{0};
        float mev_per_mip_{0};
        float pe_per_mip_{0};
        //int num_back_hcal_layers_{0};
        //int num_wrap_hcal_layers_{0};
        HcalDetId hcalDetIds;
        int nProcessed_{0};
};

}

#endif
