# WGraphPy
WgraphPy is a Python package that implements a graph used to calculate the Wasserstein distance between experimental and theorethical mass spectra and wsearch algorith for identification of protein sequence based on its spectra.

## wsearch Algorithm

The **wsearch** algorithm, developed by us, is designed to find a protein whose spectrum is closest (in terms of Wasserstein distance) to the spectrum provided by the user.

### Input Data
The algorithm takes the following inputs:
- Experimental spectrum: A mass spectrometry spectrum represented as a list of tuples [(peak mass, normalized frequency)].
- Protein database: A set of protein sequences among which the search will be conducted.
- Maximum lower bound: The maximum lower bound for the Wasserstein distance.
- Maximum number of closest sequences: The maximum number of sequences closest to the query that the user wants to see.
- The fraction of theoretical spectrum: A fraction determining the portion of the theoretical spectrum to be generated.

### Output
The output of the algorithm is a list of graphs, each associated with an amino acid sequence. Each graph includes the following information:
- Amino acid sequence: The sequence of amino acids corresponding to the graph.
- Sequence key: The key identifying the sequence in the searched database.
- Wasserstein distance: The Wasserstein distance between the experimental spectrum and the theoretical spectrum generated based on the sequence.

### Approach
Existing algorithms for calculating the Wasserstein metric directly require both histograms as input and perform full transport, which can be computationally expensive. However, the **wsearch** algorithm overcomes this challenge by leveraging network flow theory.

The algorithm assigns a graph to each protein sequence in the database, allowing for the calculation of increasingly tighter lower estimates of the Wasserstein distance. It generates the theoretical spectrum based on the given sequence and computes subsequent estimates only for proteins that are closest to the query at each step. This approach significantly reduces computation time by avoiding the generation of additional peaks for distant proteins and eliminating the need for full optimal transport calculations.
