from WGraphPy import *
import heapq


def wsearch(fasta_list, experimental, limit, results_max_size=10):
    heap = [WGraphPy(fasta=fasta_list[0][0], name=fasta_list[0][1], node_list=experimental)]
    for fasta, name in fasta_list[1:]:
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
