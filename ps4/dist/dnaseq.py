#!/usr/bin/env python2.7

import unittest
from dnaseqlib import *

### Utility classes ###

# Maps integer keys to a set of arbitrary values.
class Multidict:
    # Initializes a new multi-value dictionary, and adds any key-value
    # 2-tuples in the iterable sequence pairs to the data structure.
    def __init__(self, pairs=[]):
        self.table = dict()
        for pair in pairs:
            self.put(pair[0], pair[1])
    # Associates the value v with the key k.
    def put(self, k, v):
        if k in self.table:
            self.table[k].append(v)
        else:
            self.table[k] = [v]
    # Gets any values that have been associated with the key k; or, if
    # none have been, returns an empty sequence.
    def get(self, k):
        if k in self.table:
            return self.table[k]
        else:
            return []

# Given a sequence of nucleotides, return all k-length subsequences
# and their hashes.  (What else do you need to know about each
# subsequence?)
# Start position of each k-length subsequence
def subsequenceHashes(seq, k):
    seq_k = ''
    for i in range(0, k):
        seq_k += seq.next()
    rh = RollingHash(seq_k)
    pos = 0
    while True:
        yield [rh.current_hash(), (pos, seq_k)]
        previtm = seq_k[0]
        seq_k = seq_k[1:] + seq.next()
        rh.slide(previtm, seq_k[-1])
        pos += 1

# Similar to subsequenceHashes(), but returns one k-length subsequence
# every m nucleotides.  (This will be useful when you try to use two
# whole data files.)
def intervalSubsequenceHashes(seq, k, m):
    pos = 0
    while True:
        seq_k = ''
        for i in range(0, k):
            seq_k += seq.next()
        rh = RollingHash(seq_k)
        yield [rh.current_hash(), (pos, seq_k)]
        for i in range(0, m-k):
            seq.next()
        pos += m

# Searches for commonalities between sequences a and b by comparing
# subsequences of length k.  The sequences a and b should be iterators
# that return nucleotides.  The table is built by computing one hash
# every m nucleotides (for m >= k).
def getExactSubmatches(a, b, k, m):
    print "Building table from sequence A ..."
    sub_a = Multidict(intervalSubsequenceHashes(a, k, m))
    print "... done building table"
    for a_hash_val, (b_pos, b_seq) in subsequenceHashes(b, k):
        for a_pos, a_seq in sub_a.get(a_hash_val):
            if a_seq != b_seq:
                continue
            yield (a_pos, b_pos)
    return

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'Usage: {0} [file_a.fa] [file_b.fa] [output.png]'.format(sys.argv[0])
        sys.exit(1)

    # The arguments are, in order: 1) Your getExactSubmatches
    # function, 2) the filename to which the image should be written,
    # 3) a tuple giving the width and height of the image, 4) the
    # filename of sequence A, 5) the filename of sequence B, 6) k, the
    # subsequence size, and 7) m, the sampling interval for sequence
    # A.
    compareSequences(getExactSubmatches, sys.argv[3], (500,500), sys.argv[1], sys.argv[2], 8, 100)
