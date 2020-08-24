from WGraphPy import *
import heapq


def wsearch(fastas, eksperymentalne, limit, results_max_size=10):
    heap = [WGraphPy(fasta=fastas[0][0], name=fastas[0][1], node_list=eksperymentalne)]
    for fasta, name in fastas[1:]:
        heap.append(WGraphPy(fasta=fasta, name=name, graph=heap[0].graph))
    heapq.heapify(heap)
    results = []
    while len(results) < results_max_size and heap != []:
        tmp = heapq.heappop(heap)
        if tmp.graph.transported >= 0.99:
            results.append(tmp)
        else:
            tmp.add_inflow()
            if tmp < limit:
                heapq.heappush(heap, tmp)
    return results
