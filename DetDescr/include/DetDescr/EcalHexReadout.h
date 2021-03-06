/**
 * @file EcalHexReadout.h
 * @brief Class that translates raw positions of ECal hits into cells in a hexagonal readout
 * @author Owen Colegro, UCSB
 */

#ifndef DETDESCR_ECALHEXREADOUT_H_
#define DETDESCR_ECALHEXREADOUT_H_

// STL
#include <map>
#include <iostream>

// ROOT
#include "TH2Poly.h"

namespace ldmx {

    /**
     * @class EcalHexReadout
     * @brief Implementation of ECal hexagonal cell readout
     *
     * @note
     * This class implements a hexagonal cell readout across all modules in the ECal.
     * It can provide a cell ID given a position and can also translate a cell ID back
     * into a position of the cell centroid.
     */
    class EcalHexReadout {

        public:

            /**
             * Class constructor.
             * @param width The width of the grid [mm].
             * @param side The length of the hexagon's side [mm].
             */
            EcalHexReadout(const double width = 1000, const double side = 4.59360);

            /**
             * Class destructor.
             */
            virtual ~EcalHexReadout() {
                delete ecalMap;
            }

            /**
             * Get a cell ID from an XY position.
             * @param x The X position.
             * @param y The Y position.
             */
            inline int getCellId(float x, float y) {
                return ecalMap->FindBin(x,y);
            }

            /**
             * Get an XY position from a cell ID.
             * @param cellID The cell ID.
             * @return The XY position of the cell.
             */
            inline std::pair<float, float> getCellCentroidXYPair(int cellID) {
                std::pair<std::map<int, XYCoords>::iterator, bool> isInserted;
                isInserted = cellIdtoCoords.insert(std::pair<int,XYCoords>(cellID,std::pair<float,float>(0, 0)));
                if (isInserted.second == true) {
                    throw std::runtime_error("Error: cell " + std::to_string(cellID) + " is not valid");
                }
                return isInserted.first->second;
            }

            /**
             * Lattice spacing is periodic in ecal map
             * Offsets to move right,left,up and down are hardcoded
             * @param centroidId is center cell id, cellId is neighbor we want to check
             * @return if cell matching centroidId is within inner ring of cellID
             */
            inline bool isInShowerInnerRing(int centroidId, int cellId) {
                bool matched = false;
                for (auto ringId : getInnerRingCellIds(centroidId)) {
                    if (cellId == ringId)
                        matched = true;
                }
                return matched;
            }

            /**
             * Same as InnerShowerRing, but with more neighbors checked
             */
            inline bool isInShowerOuterRing(int centroidId, int cellId) {
                bool matched = false;
                for (auto ringId : getOuterRingCellIds(centroidId)) {
                    if (cellId == ringId)
                        matched = true;
                }
                return matched;
            }

            /**
             * @todo Document this function.
             */
            std::vector<int> getInnerRingCellIds(int cellID);

            /**
             * @todo Document this function.
             */
            std::vector<int> getOuterRingCellIds(int cellID);

        private:

            /**
             * @todo Document this function.
             */
            void buildMap(Double_t xstart,
                    Double_t ystart, /* Map starting points */
                    Double_t a,  /* side length */
                    Int_t k,     /* # hexagons in a column */
                    Int_t s);

        private:

            static const int SHIFT_UP{1};
            static const int SHIFT_DOWN{-1};
            static const int SHIFT_RIGHT_UP{109};
            static const int SHIFT_RIGHT_DOWN{108};
            static const int SHIFT_LEFT_UP{-SHIFT_RIGHT_DOWN};
            static const int SHIFT_LEFT_DOWN{-SHIFT_RIGHT_UP};

            TH2Poly* ecalMap;
            typedef std::pair<float,float> XYCoords;
            std::map<int, XYCoords> cellIdtoCoords;
    };

}

#endif
