#!/usr/bin/python

from LDMX.Framework import ldmxcfg

hcalDigis = ldmxcfg.Producer("hcalDigis", "ldmx::HcalDigiProducer")

# set the mean noise in PE units
hcalDigis.parameters["meanNoise"] = 2.0

