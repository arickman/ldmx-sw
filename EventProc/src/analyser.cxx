/**
 * @file analyser.cxx
 * @brief Application used to initialize and run the LDMX analysis chain.
 * @author Omar Moreno
 *         SLAC National Accelerator Laboratory
 * @date January 11, 2017
 */

//----------------//
//   C++ StdLib   //
//----------------//
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <list>
#include <string>

//----------//
//   ROOT   //
//----------//
#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

//-------------//
//   LDMX SW   //
//-------------//
//#include "Event/SimEvent.h"
#include "Event/EventFile.h"
#include "Event/EventImpl.h"


using namespace std; 

int main(int argc, char** argv) { 

	// Name of the LCIO file to be processed.
	string file_path; 
    string list_path;

	// Parse all the command line arguments.  If there are no valid command 
	// line arguments passed, print the usage and exit.
	static struct option long_options[] = { 
		{"file",  required_argument, 0, 'f' }, 
        {"list",  required_argument, 0, 'l'},
        {"help",  no_argument,       0, 'h'},
		{ 0, 0, 0, 0 } 
	};

	int option_index = 0; 
	int option_char; 
	while ((option_char = getopt_long(argc, argv, "i:l:n:h", long_options, &option_index)) != -1) { 
		switch (option_char) { 
			case 'f': 
				file_path = optarg;
				break;
            case 'l':
                list_path = optarg;
                break; 
            case 'h':
                return EXIT_SUCCESS; 
			default: 
				return EXIT_FAILURE; 	
		}
	}

	// If a ROOT file was not specified, warn the user and exit the application.
	if (file_path.empty() && list_path.empty()) { 
		cerr << "[ ANALYZER ]: Please specify a file or list of files to process." << endl;
        cerr << "[ ANALYZER ]: Use --help for usage.\n" << endl;
		return EXIT_FAILURE;
	} else if (!file_path.empty() && !list_path.empty()) { 
        cerr << "[ ANALYZER ]: " 
             << "Cannot specify both a file name an a list of files." << endl;
        cerr << "[ ANALYZER ]: Use --help for usage.\n" << endl;
		return EXIT_FAILURE;
    }
    
    // Create a list of files to process
    list<string> files; 
    string file;
    if (!file_path.empty()) { 
        files.push_back(file_path); 
    } else if (!list_path.empty()) { 
        
        ifstream file_list(list_path.c_str(), ifstream::in);
        if (!file_list.is_open()) { 
            cerr << "\n[ ANALYZER ]: Failed to open file " << list_path << endl;
            return EXIT_FAILURE;
        }
        
        while (file_list >> file) { 
            files.push_back(file); 
        }
        file_list.close();
    }
   
    // Loop through the ROOT files and process them
    for (auto file : files) { 
        
        event::EventFile simFile(file, false); 
        string reconFilePath 
            = file.replace(file.begin() + file.find(".root"), file.end(), "_recon.root");
        event::EventFile reconFile(reconFilePath, &simFile);
        event::EventImpl event("recon");
        reconFile.setupEvent(&event); 

        double event_n{0}; 
        while (reconFile.nextEvent()) {
            std::cout << "Event: " << event_n++ << std::endl;
        }

        reconFile.close(); 
        simFile.close(); 
        // Open the ROOT file. If the file can't be opened, exit the application.
        /*
        TFile* simFile = new TFile(file.c_str());
        TTree* simTree = static_cast<TTree*>(simFile->Get("LDMX_Event"));
        
        TFile* reconFile = new TFile(reconFilePath.c_str(), "recreate"); 
        TTree* reconTree = simTree->CloneTree(0); 

        event::SimEvent* event = new event::SimEvent();
        TBranch* bEvent = simTree->GetBranch("LdmxEvent"); 
        bEvent->SetAddress(&event);  

        // Loop over all of the events and process them
        for (int entry = 0; entry < simTree->GetEntries(); ++entry) {
            simTree->GetEntry(entry);
            reconTree->Fill();    
        }
        reconTree->AutoSave();
        
        delete simFile;
        delete reconFile;  */
    } 
}
