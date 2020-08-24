import IsoSpecPy
from WGraphCpp import *


class FormulaError(Exception):
    pass


class WGraphPy(object):
    __slots__ = ['name', 'peakGenerator', 'graph']

    def __init__(self, *args, name=None, fasta=None, node_list=None, graph=None):
        try:
            self.peakGenerator = iter(IsoSpecPy.IsoLayeredGenerator(fasta=fasta))
        except Exception:
            raise FormulaError('invalid sequence') from None
        if graph is None:
            self.graph = WGraphCpp()
            for node in node_list:
                self.graph.add_outflow(*node)
        else:
            self.graph = graph.copy()
        self.name = name

    def add_inflow(self):
        try:
            node = next(self.peakGenerator)
            self.graph.add_inflow(*node)
        except Exception as e:
            print(e)

    def __float__(self):
        return self.graph.wdist

    def __str__(self):
        return "WGraph of " + self.name + ", wasserstein distance: " + \
               str(self.graph.wdist) + ", transported probability: " + str(self.graph.transported)

    def __gt__(self, other):
        return self.graph.wdist > float(other)

    def __lt__(self, other):
        return self.graph.wdist < float(other)

    def __ge__(self, other):
        return self.graph.wdist >= float(other)

    def __le__(self, other):
        return self.graph.wdist <= float(other)

    def __eq__(self, other):
        return self.graph.wdist == float(other)

    def __ne__(self, other):
        return self.graph.wdist != float(other)
