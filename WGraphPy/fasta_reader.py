import fastaparser

def fasta_reader(path):
    results = []
    with open(path) as file:
        parser = fastaparser.Reader(file)
        for seq in parser:
            results.append((seq.sequence_as_string(), seq.id))
    return results