#!/usr/bin/env python

import argparse
import random
import time
import os
import getpass
import subprocess

# Geant4 macro that will be created by this script
macro_path = './run.mac'

def main():

    parser = argparse.ArgumentParser(description="Run an ldmx-sim job")
    parser.add_argument('-i', nargs=1, help="input file (ROOT or LHE)", required=False) # input file if applicable
    parser.add_argument('-o', nargs=1, help="output file", required=True) # output file
    parser.add_argument('-m', action='append', dest='m', default=[], help="macros to run", required=False) # macros for generating events
    parser.add_argument('-n', nargs=1, help="number of events", required=True) # number of events to generate
    parser.add_argument('-d', nargs=1, help="detector name", required=True) # detector name
    parser.add_argument('-p', nargs=1, help="output directory", required=True) # output directory
    args = parser.parse_args()

    if not os.environ.get('LDMXSW_DIR'):
        raise Exception("ERROR: LDMXSW_DIR is not set in the environment!")
    ldmxsw = os.environ.get('LDMXSW_DIR')

    output_file = args.o[0]
    macros = args.m
    nevents = int(args.n[0])
    detector_name = args.d[0]
    output_dir = os.path.abspath(args.p[0])
    if args.i is not None:
        input_file = args.i[0]
    else:
        if len(macros) == 0 :
            raise Exception("ERROR: An input file or at least one macro must be supplied.")
        input_file = None

    scratch_dir = '/scratch/'+getpass.getuser()
    if not os.path.exists(scratch_dir):
        os.makedirs(scratch_dir)
    tmp_dir = '%s/%s' % (scratch_dir, os.environ['LSB_JOBID'])
    if not os.path.exists(tmp_dir):
        os.makedirs(tmp_dir)
    os.chdir(tmp_dir)

    detector_data_dir = os.path.join(ldmxsw, 'data', 'detectors', detector_name)
    detector_path = os.path.join(detector_data_dir, 'detector.gdml')

    fieldmap_dir = os.path.join(ldmxsw, 'data', 'fieldmap')
    for item in os.listdir(fieldmap_dir):
        os.symlink(os.path.join(fieldmap_dir, item), os.path.join(tmp_dir, item))

    if os.path.exists(output_file):
        raise Exception("ERROR: The output file '%s' already exists!" % output_file)

    create_macro(detector_path, output_file, input_file, macros, nevents)
    
    subprocess.Popen('echo -n \"started: \"; date', shell=True).wait()

    # Send this to a process so it appears at the top of the job log.
    subprocess.Popen('echo ---- Job macro ----; cat run.mac; echo', shell=True).wait()

    command = 'time ldmx-sim %s' % macro_path
    subprocess.Popen(command, shell=True).wait()

    os.system('cp -r %s/%s %s' % (tmp_dir, output_file, output_dir))
    os.system('ls -lah %s' % os.path.join(output_dir, output_file))
    subprocess.Popen('echo -n \"ended: \"; date', shell=True).wait()
    subprocess.Popen('rm -rf %s' % tmp_dir, shell=True).wait()

def create_macro(detector_path, output_file, input_file, macros, nevents):

    random.seed(time.time())
    seed1 = int(random.random()*10000)
    seed2 = int(random.random()*10000)

    macro_file = open(macro_path, 'w')
    macro_file.write('/persistency/gdml/read '+detector_path+'\n')
    macro_file.write('/run/initialize\n')
    macro_file.write('/random/setSeeds %d %d\n' % (seed1, seed2))
    for macro in macros:
        macro_file.write('/control/execute '+macro+'\n')
    if input_file is not None:
        if input_file.lower().endswith('.root'):
            macro_file.write('/ldmx/generators/root/open '+input_file+'\n')
            macro_file.write('/ldmx/generators/root/useSeed\n')
        elif input_file.lower().endswith('.lhe'):
            macro_file.write('/ldmx/generators/lhe/open '+input_file+'\n')
    macro_file.write('/ldmx/persistency/root/verbose 2\n')
    macro_file.write('/ldmx/persistency/root/file '+output_file+'\n')
    macro_file.write('/ldmx/plugins/load EventPrintPlugin\n')
    macro_file.write('/run/beamOn '+str(nevents)+'\n')

    macro_file.close()

if __name__ == '__main__' :
    main()
