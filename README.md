# WGraphPy
WgraphPy is a Python package that implements a graph used to calculate the Wasserstein distance between experimental and theorethical mass spectra.
The package includes two main components: WGraphPy.py, which is a Python interface for the Wgraph class written in C++, and wsearch.py,
which provides a function for searching throughout the list of fasta, to find the one with the theorethical spectrum most similar to the experimental (based on the Wasserstein distance).
