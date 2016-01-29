import numpy
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-m", "--mode", action="store", choices=["variances", "m_distribution"],
                    help="Analysis that should be performed.")
clvalues = parser.parse_args()
mode = clvalues.mode

data = numpy.genfromtxt(sys.stdin)

if mode == "m_distribution":
    print "Not yet implemented"
elif mode == "variances":
    mean_energy = numpy.mean(data[:,0])
    order_parameter = numpy.mean(numpy.abs(data[:,1]))
    variance_energy = numpy.var(data[:,0])
    variance_magnetization = numpy.var(data[:,1])
    print str(mean_energy) + "\t" + str(order_parameter) + "\t" + str(variance_energy) + "\t" + str(variance_magnetization)

