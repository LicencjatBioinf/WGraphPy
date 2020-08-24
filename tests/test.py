from WGraphPy import *

path = "uniprot-mass_[900+TO+1100].fasta"

f = fasta_reader(path)
print("zaladowano ", len(f), " bialek")

eksperymentalne = IsoSpecPy.IsoTotalProb(
    fasta="TDRNFLRL",
    prob_to_cover=0.99)
moja_lista = []
for mass, prob in eksperymentalne:
    moja_lista.append((mass, prob))
print('utworzono widmo eksperymentalne z', len(moja_lista), " pikow")

print("wsearch ...")

results = wsearch(fastas=f, eksperymentalne=moja_lista, limit=100.,results_max_size=25)
for i in results:
    print(i)

print("OK" if len(results) == 25 else "FAILED ")
