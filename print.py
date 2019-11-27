import numpy as np
from pylab import *

# matplotlib.use("pgf")
# matplotlib.rcParams.update({
#     "pgf.texsystem": "pdflatex",
#     'font.family': 'serif',
#     'text.usetex': True,
#     'pgf.rcfonts': False,
# })

f = open("LargeNetworkComercial.txt")
Y1 = f.read().splitlines()
Y1[:] = [float(x) for x in Y1]
X = list(range(1, len(Y1) + 1))
plot(X,Y1, label="Comercial Route")

f = open("LargeNetworkNonComercial.txt")
Y2 = f.read().splitlines()
Y2[:] = [float(x) for x in Y2]
X = list(range(1, len(Y2) + 1))
plot(X,Y2, label="Non Comercial Route")

plt.ylabel('ccdf')
plt.xlabel('Número de saltos')
plt.legend(loc="upper right")

xlim([1,35])
ylim([0,100])
show()