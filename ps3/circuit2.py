#!/usr/bin/env python

import json   # Used when TRACE=jsonp
import os     # Used to get the TRACE environment variable
import re     # Used when TRACE=jsonp
import sys    # Used to smooth over the range / xrange issue.

# Python 3 doesn't have xrange, and range behaves like xrange.
if sys.version_info >= (3,):
    xrange = range

# Circuit verification library.

class Wire(object):
  """A wire in an on-chip circuit.
  
  Wires are immutable, and are either horizontal or vertical.
  """
  
  def __init__(self, name, x1, y1, x2, y2):
    """Creates a wire.
    
    Raises an ValueError if the coordinates don't make up a horizontal wire
    or a vertical wire.
    
    Args:
      name: the wire's user-visible name
      x1: the X coordinate of the wire's first endpoint
      y1: the Y coordinate of the wire's first endpoint
      x2: the X coordinate of the wire's last endpoint
      y2: the Y coordinate of the wire's last endpoint
    """
    # Normalize the coordinates.
    if x1 > x2:
      x1, x2 = x2, x1
    if y1 > y2:
      y1, y2 = y2, y1
    
    self.name = name
    self.x1, self.y1 = x1, y1
    self.x2, self.y2 = x2, y2
    self.object_id = Wire.next_object_id()
    
    if not (self.is_horizontal() or self.is_vertical()):
      raise ValueError(str(self) + ' is neither horizontal nor vertical')
  
  def is_horizontal(self):
    """True if the wire's endpoints have the same Y coordinates."""
    return self.y1 == self.y2
  
  def is_vertical(self):
    """True if the wire's endpoints have the same X coordinates."""
    return self.x1 == self.x2
  
  def intersects(self, other_wire):
    """True if this wire intersects another wire."""
    # NOTE: we assume that wires can only cross, but not overlap.
    if self.is_horizontal() == other_wire.is_horizontal():
      return False 
    
    if self.is_horizontal():
      h = self
      v = other_wire
    else:
      h = other_wire
      v = self
    return v.y1 <= h.y1 and h.y1 <= v.y2 and h.x1 <= v.x1 and v.x1 <= h.x2
  
  def __repr__(self):
    # :nodoc: nicer formatting to help with debugging
    return('<wire ' + self.name + ' (' + str(self.x1) + ',' + str(self.y1) + 
           ')-(' + str(self.x2) + ',' + str(self.y2) + ')>')
  
  def as_json(self):
    """Dict that obeys the JSON format restrictions, representing the wire."""
    return {'id': self.name, 'x': [self.x1, self.x2], 'y': [self.y1, self.y2]}

  # Next number handed out by Wire.next_object_id()
  _next_id = 0
  
  @staticmethod
  def next_object_id():
    """Returns a unique numerical ID to be used as a Wire's object_id."""
    id = Wire._next_id
    Wire._next_id += 1
    return id

class WireLayer(object):
  """The layout of one layer of wires in a chip."""
  
  def __init__(self):
    """Creates a layer layout with no wires."""
    self.wires = {}
  
  def wires(self):
    """The wires in the layout."""
    self.wires.values()
  
  def add_wire(self, name, x1, y1, x2, y2):
    """Adds a wire to a layer layout.
    
    Args:
      name: the wire's unique name
      x1: the X coordinate of the wire's first endpoint
      y1: the Y coordinate of the wire's first endpoint
      x2: the X coordinate of the wire's last endpoint
      y2: the Y coordinate of the wire's last endpoint
    
    Raises an exception if the wire isn't perfectly horizontal (y1 = y2) or
    perfectly vertical (x1 = x2)."""
    if name in self.wires:
        raise ValueError('Wire name ' + name + ' not unique')
    self.wires[name] = Wire(name, x1, y1, x2, y2)
  
  def as_json(self):
    """Dict that obeys the JSON format restrictions, representing the layout."""
    return { 'wires': [wire.as_json() for wire in self.wires.values()] }
  
  @staticmethod
  def from_file(file):
    """Builds a wire layer layout by reading a textual description from a file.
    
    Args:
      file: a File object supplying the input
    
    Returns a new Simulation instance."""

    layer = WireLayer()
    
    while True:
      command = file.readline().split()
      if command[0] == 'wire':
        coordinates = [float(token) for token in command[2:6]]
        layer.add_wire(command[1], *coordinates)
      elif command[0] == 'done':
        break
      
    return layer

class RangeIndex(object):
  """Array-based range index implementation."""
  
  def __init__(self):
    """Initially empty range index."""
    self.data = []
  
  def add(self, key):
    """Inserts a key in the range index."""
    if key is None:
        raise ValueError('Cannot insert nil in the index')
    self.data.append(key)
  
  def remove(self, key):
    """Removes a key from the range index."""
    self.data.remove(key)
  
  def list(self, first_key, last_key):
    """List of values for the keys that fall within [first_key, last_key]."""
    return [key for key in self.data if first_key <= key <= last_key]
  
  def count(self, first_key, last_key):
    """Number of keys that fall within [first_key, last_key]."""
    result = 0
    for key in self.data:
      if first_key <= key <= last_key:
        result += 1
    return result

class BlitRangeIndex(object):
  """Sorted array-based range index implementation"""

  def __init__(self):
    self.data = []

  def add(self, key):
    if key is None:
        raise ValueError('Cannot insert nil in the index')
    self.data.insert(self._binary_search(key), key)

  def remove(self, key):
    index = self._binary_search(key)
    if index < len(self.data) and self.data[index] == key:
      self.data.pop(index)

  def list(self, low_key, high_key):
    low_index = self._binary_search(low_key)
    high_index = self._binary_search(high_key)
    return self.data[low_index:high_index]

  def count(self, low_key, high_key):
    low_index = self._binary_search(low_key)
    high_key = self._binary_search(high_key)
    return high_key - low_key

  def _binary_search(self, key):
    """binary search for the given key in the sorted array"""
    low, high = 0, len(self.data) - 1
    while low < high:
      mid = (low + high) // 2
      mid_key = self.data[mid]
      if key < mid_key:
        high = mid - 1
      elif key > mid_key:
        low = mid + 1
      else:
        return mid
    return high + 1 

class BST_node(object):
  """BST data structure"""

  def __init__(self, key):
    self.key = key
    self.left = None
    self.right = None
    self.parent = None

  def find(self, key):
    if key == self.key:
      return self
    elif key < self.key:
      if self.left is None:
        return None
      else:
        return self.left.find(key)
    else:
      if self.right is None:
        return None
      else:
        return self.right.find(key)

  def find_min(self):
    current = self
    while current.left is not None:
      current = current.left
    return current

  def successor(self):
    # Case 1: right child is not None
    if self.right is not None:
      return self.right.find_min()
    current = self
    while current.parent is not None and current is current.parent.right:
      current = current.parent
    return current.parent

  def insert(self, node):
    if node.key < self.key:
      if self.left is None:
        self.left = node
        node.parent = self
        return node
      else:
        return self.left.insert(node)
    elif node.key > self.key:
      if self.right is None:
        self.right = node
        node.parent = self
        return node
      else:
        return self.right.insert(node)
    return self

  def delete(self):
    # Case 1 & 2: node has zero or one children
    if self.left is None or self.right is None:
      if self is self.parent.left:
        self.parent.left = self.left or self.right
        if self.parent.left is not None:
          self.parent.left.parent = self.parent
      else:
        self.parent.right = self.left or self.right
        if self.parent.right is not None:
          self.parent.right.parent = self.parent
      return self
    # Case 3: node has two children
    else:
      s = self.successor()
      deleted_node = s.delete()
      self.key, s.key = s.key, self.key
      return deleted_node

  def check_ri(self):
    """Check BST representation ivarianant around this node"""

    if self.left is not None:
      if self.left.key >= self.key:
        raise RuntimeError("BST RI violated by a left node key")
      if self.left.parent is not self:
        raise RuntimeError("BST RI violated by a left node's parent pointer")
      self.left.check_ri()
    if self.right is not None:
      if self.right.key <= self.key:
        raise RuntimeError("BST RI violated by a right node key")
      if self.right.parent is not self:
        raise RuntimeError("BST RI violated by a right node's parent pointer")
      self.right.check_ri()

class BST_tree(object):
  """BST based range index implementation"""

  def __init__(self, node_class = BST_node):
    self.root = None
    self.node_class = node_class

  def find(self, key):
    return self.root and self.root.find(key)

  def min(self):
    if self.root is None:
      return None
    else:
      return self.root.find_min()

  def insert(self, key):
    node = self.node_class(key)
    if self.root is None:
      self.root = node
      return node
    return self.root.insert(node)

  def delete(self, key):
    node = self.find(key)
    if node is None:
      return None
    if node is self.root:
      pseudoroot = self.node_class(0)
      pseudoroot.left = self.root
      self.root.parent = pseudoroot
      deleted_node = self.root.delete()
      self.root = pseudoroot.left
      if self.root is not None:
        self.root.parent = None
      return deleted_node
    else:
      return node.delete()

  def successor(self, key):
    node = self.find(key)
    return node and node.successor()

  def check_ri(self):
    if self.root is not None:
      if self.root.parent is not None:
        raise RuntimeError("BST RI violated by root's parent")
      self.root.check_ri()


class AVL_node(BST_node):

  def __init__(self, key):
    BST_node.__init__(self, key)
    self.height = 0

  def update_subtree_info(self):
    self.height = self._uncached_height()

  def _uncached_height(self):
    return 1 + max((self.left and self.left.height) or -1, 
                   (self.right and self.right.height) or -1)

  def check_ri(self):
    if self.height != self._uncached_height():
      raise RuntimeError("AVL RI violated by node's height")
    if abs(AVL_tree._height(self.left) - AVL_tree._height(self.right)) > 1:
      raise RuntimeError("AVL RI violated by unbalanced children heights")
    BST_node.check_ri(self)

class AVL_tree(BST_tree):

  def __init__(self, node_class = AVL_node):
    BST_tree.__init__(self, node_class)

  def insert(self, key):
    inserted_node = BST_tree.insert(self, key)
    self._rebalance(inserted_node)
    return inserted_node

  def delete(self, key):
    deleted_node = BST_tree.delete(self, key)
    self._rebalance(deleted_node.parent)
    return deleted_node

  def _rebalance(self, node):
    while node is not None:
      node.update_subtree_info()

      if AVL_tree._height(node.left) >= 2 + AVL_tree._height(node.right):
        if AVL_tree._height(node.left.left) < AVL_tree._height(node.left.right):
          self._left_rotate(node.left)
        self._right_rotate(node)
      elif AVL_tree._height(node.right) >= 2 + AVL_tree._height(node.left):
        if AVL_tree._height(node.right.right) < AVL_tree._height(node.right.left):
          self._right_rotate(node.right)
        self._left_rotate(node)
      node = node.parent

  @staticmethod
  def _height(node):
    if node is not None:
      return node.height
    else:
      return -1

  def _left_rotate(self, x):
    y = x.right
    y.parent = x.parent
    if y.parent is None:
      self.root = y
    else:
      if y.parent.left is x:
        y.parent.left = y
      else:
        y.parent.right = y
    x.right = y.left
    if x.right is not None:
      x.right.parent = x
    y.left = x
    x.parent = y
    x.update_subtree_info()
    y.update_subtree_info()

  def _right_rotate(self, x):
    y = x.left
    y.parent = x.parent
    if y.parent is None:
      self.root = y
    else:
      if y.parent.right is x:
        y.parent.right = y
      else:
        y.parent.left = y
    x.left = y.right
    if x.left is not None:
      x.left.parent = x
    y.right = x
    x.parent = y
    x.update_subtree_info()
    y.update_subtree_info()

class RangeNode(AVL_node):

  def __init__(self, key):
    AVL_node.__init__(self, key)
    self.tree_size = 1

  def update_subtree_info(self):
    AVL_node.update_subtree_info(self)
    self.tree_size = self._uncached_tree_size()

  def _uncached_tree_size(self):
    return 1 + (((self.left and self.left.tree_size) or 0) + 
                ((self.right and self.right.tree_size) or 0))

  def check_ri(self):
    if self.tree_size != self._uncached_tree_size():
      raise RuntimeError("RangeTree RI violated by node subtree size")
    AVL_node.check_ri(self)

  def rank(self, key):
    if key < self.key:
      if self.left is not None:
        return self.left.rank(key)
      else:
        return 0
    if self.left:
      lrank = 1 + self.left.tree_size
    else:
      lrank = 1
    if key > self.key and self.right is not None:
      return lrank + self.right.rank(key)
    return lrank

  def lca(self, low_key, high_key):
    """Lowest-common ancestor node of nodes with low_key and high_key.
    
    If low_key and/or high_key are not in the tree, this returns the LCA of the
    nodes that would be created by inserting the keys in the tree.
    
    Returns a RangeNode instance, or None if low_key and high_key are not in the
    node's subtree, and there is no key in the tree such that
    low_key < key < high_key.
    """
    if low_key <= self.key <= high_key:
      return self
    if low_key < self.key:
      return self.left and self.left.lca(low_key, high_key)
    else:
      return self.right and self.right.lca(low_key, high_key)

  def list(self, low_key, high_key, result):
    """Lists nodes with keys between low_key and high_key in this node's subtree.
    
    Extends result with a list of RangeNode instances for the nodes in the
    subtree rooted at this node, such that each node's key is between low_key
    and high_key."""
    if low_key <= self.key <= high_key:
      result.append(self)
    if self.left is not None and low_key <= self.key:
      self.left.list(low_key, high_key, result)
    if self.right is not None and self.key <= high_key:
      self.right.list(low_key, high_key, result)

class AVLRangeTree(AVL_tree):

  def __init__(self, node_class = RangeNode):
    AVL_tree.__init__(self, node_class)

  def rank(self, key):
    if self.root is not None:
      return self.root.rank(key)
    return 0

  def lca(self, low_key, high_key):
    return self.root and self.root.lca(low_key, high_key)

  def list(self, low_key, high_key):
    result = []
    lca = self.lca(low_key, high_key)
    if lca is not None:
      lca.list(low_key, high_key, result)
    return result

# Uncomment lines below for RI checking
'''
  def insert(self, key):
    result = AVL_tree.insert(self, key)
    self.check_ri()
    return result

  def delete(self, key):
    result = AVL_tree.delete(self, key)
    self.check_ri()
    return result
'''

class AvlRangeIndex(object):
  """Sorted array-based range index implementation."""
  
  def __init__(self):
    """Initially empty range index."""
    self.tree = AVLRangeTree()
  
  def add(self, key):
    """Inserts a key in the range index."""
    if key is None:
        raise ValueError('Cannot insert None in the index')
    self.tree.insert(key)
  
  def remove(self, key):
    """Removes a key from the range index."""
    self.tree.delete(key)
  
  def list(self, low_key, high_key):
    """List of values for the keys that fall within [low_key, high_key]."""
    return [node.key for node in self.tree.list(low_key, high_key)]
  
  def count(self, low_key, high_key):
    """Number of keys that fall within [low_key, high_key]."""
    return self.tree.rank(high_key) - self.tree.rank(low_key)

RangeIndex = AvlRangeIndex

class TracedRangeIndex(RangeIndex):
  """Augments RangeIndex to build a trace for the visualizer."""
  
  def __init__(self, trace):
    """Sets the object receiving tracing info."""
    RangeIndex.__init__(self)
    self.trace = trace
  
  def add(self, key):
    self.trace.append({'type': 'add', 'id': key.wire.name})
    RangeIndex.add(self, key)
  
  def remove(self, key):
    self.trace.append({'type': 'delete', 'id': key.wire.name})
    RangeIndex.remove(self, key)
  
  def list(self, first_key, last_key):
    result = RangeIndex.list(self, first_key, last_key)
    self.trace.append({'type': 'list', 'from': first_key.key,
                       'to': last_key.key,
                       'ids': [key.wire.name for key in result]}) 
    return result
  
  def count(self, first_key, last_key):
    result = RangeIndex.count(self, first_key, last_key)
    self.trace.append({'type': 'list', 'from': first_key.key,
                       'to': last_key.key, 'count': result})
    return result

class ResultSet(object):
  """Records the result of the circuit verifier (pairs of crossing wires)."""
  
  def __init__(self):
    """Creates an empty result set."""
    self.crossings = []
  
  def add_crossing(self, wire1, wire2):
    """Records the fact that two wires are crossing."""
    self.crossings.append(sorted([wire1.name, wire2.name]))
  
  def write_to_file(self, file):
    """Write the result to a file."""
    for crossing in self.crossings:
      file.write(' '.join(crossing))
      file.write('\n')

class TracedResultSet(ResultSet):
  """Augments ResultSet to build a trace for the visualizer."""
  
  def __init__(self, trace):
    """Sets the object receiving tracing info."""
    ResultSet.__init__(self)
    self.trace = trace
    
  def add_crossing(self, wire1, wire2):
    self.trace.append({'type': 'crossing', 'id1': wire1.name,
                       'id2': wire2.name})
    ResultSet.add_crossing(self, wire1, wire2)

class KeyWirePair(object):
  """Wraps a wire and the key representing it in the range index.
  
  Once created, a key-wire pair is immutable."""
  
  def __init__(self, key, wire):
    """Creates a new key for insertion in the range index."""
    self.key = key
    if wire is None:
      raise ValueError('Use KeyWirePairL or KeyWirePairH for queries')
    self.wire = wire
    self.wire_id = wire.object_id

  def __lt__(self, other):
    # :nodoc: Delegate comparison to keys.
    return (self.key < other.key or
            (self.key == other.key and self.wire_id < other.wire_id))
  
  def __le__(self, other):
    # :nodoc: Delegate comparison to keys.
    return (self.key < other.key or
            (self.key == other.key and self.wire_id <= other.wire_id))  

  def __gt__(self, other):
    # :nodoc: Delegate comparison to keys.
    return (self.key > other.key or
            (self.key == other.key and self.wire_id > other.wire_id))
  
  def __ge__(self, other):
    # :nodoc: Delegate comparison to keys.
    return (self.key > other.key or
            (self.key == other.key and self.wire_id >= other.wire_id))

  def __eq__(self, other):
    # :nodoc: Delegate comparison to keys.
    return self.key == other.key and self.wire_id == other.wire_id
  
  def __ne__(self, other):
    # :nodoc: Delegate comparison to keys.
    return self.key == other.key and self.wire_id == other.wire_id

  def __hash__(self):
    # :nodoc: Delegate comparison to keys.
    return hash([self.key, self.wire_id])

  def __repr__(self):
    # :nodoc: nicer formatting to help with debugging
    return '<key: ' + str(self.key) + ' wire: ' + str(self.wire) + '>'

class KeyWirePairL(KeyWirePair):
  """A KeyWirePair that is used as the low end of a range query.
  
  This KeyWirePair is smaller than all other KeyWirePairs with the same key."""
  def __init__(self, key):
    self.key = key
    self.wire = None
    self.wire_id = -1000000000

class KeyWirePairH(KeyWirePair):
  """A KeyWirePair that is used as the high end of a range query.
  
  This KeyWirePair is larger than all other KeyWirePairs with the same key."""
  def __init__(self, key):
    self.key = key
    self.wire = None
    # HACK(pwnall): assuming 1 billion objects won't fit into RAM.
    self.wire_id = 1000000000

class CrossVerifier(object):
  """Checks whether a wire network has any crossing wires."""
  
  def __init__(self, layer):
    """Verifier for a layer of wires.
    
    Once created, the verifier can list the crossings between wires (the 
    wire_crossings method) or count the crossings (count_crossings)."""

    self.events = []
    self._events_from_layer(layer)
    self.events.sort()
  
    self.index = RangeIndex()
    self.result_set = ResultSet()
    self.performed = False
  
  def count_crossings(self):
    """Returns the number of pairs of wires that cross each other."""
    if self.performed:
      raise 
    self.performed = True
    return self._compute_crossings(True)

  def wire_crossings(self):
    """An array of pairs of wires that cross each other."""
    if self.performed:
      raise 
    self.performed = True
    return self._compute_crossings(False)

  def _events_from_layer(self, layer):
    """Populates the sweep line events from the wire layer."""
    for wire in layer.wires.values():
      if wire.is_horizontal():
        self.events.append([wire.x1, 0, wire.object_id, 'add', wire])
        self.events.append([wire.x2, 2, wire.object_id, 'remove', wire])
      else:
        self.events.append([wire.x1, 1, wire.object_id, 'query', wire])

  def _compute_crossings(self, count_only):
    """Implements count_crossings and wire_crossings."""
    if count_only:
      result = 0
    else:
      result = self.result_set

    for event in self.events:
      event_x, event_type, wire = event[0], event[3], event[4]
      self.trace_sweep_line(event_x)
      
      if event_type == 'add':
        self.index.add(KeyWirePair(wire.y1, wire))
      if event_type == 'remove':
        self.index.remove(KeyWirePair(wire.y1, wire))
      elif event_type == 'query':
        if count_only:
          result += self.index.count(KeyWirePairL(wire.y1),
                                     KeyWirePairH(wire.y2))
        else:
          for kwp in self.index.list(KeyWirePairL(wire.y1), 
                                     KeyWirePairH(wire.y2)):
            result.add_crossing(wire, kwp.wire)

    return result
  
  def trace_sweep_line(self, x):
    """When tracing is enabled, adds info about where the sweep line is.
    
    Args:
      x: the coordinate of the vertical sweep line
    """
    # NOTE: this is overridden in TracedCrossVerifier
    pass

class TracedCrossVerifier(CrossVerifier):
  """Augments CrossVerifier to build a trace for the visualizer."""
  
  def __init__(self, layer):
    CrossVerifier.__init__(self, layer)
    self.trace = []
    self.index = TracedRangeIndex(self.trace)
    self.result_set = TracedResultSet(self.trace)
    
  def trace_sweep_line(self, x):
    self.trace.append({'type': 'sweep', 'x': x})
    
  def trace_as_json(self):
    """List that obeys the JSON format restrictions with the verifier trace."""
    return self.trace

# Command-line controller.
if __name__ == '__main__':
    import sys
    layer = WireLayer.from_file(sys.stdin)
    verifier = CrossVerifier(layer)
    
    if os.environ.get('TRACE') == 'jsonp':
      verifier = TracedCrossVerifier(layer)
      result = verifier.wire_crossings()
      json_obj = {'layer': layer.as_json(), 'trace': verifier.trace_as_json()}
      sys.stdout.write('onJsonp(')
      json.dump(json_obj, sys.stdout)
      sys.stdout.write(');\n')
    elif os.environ.get('TRACE') == 'list':
      verifier.wire_crossings().write_to_file(sys.stdout)
    else:
      sys.stdout.write(str(verifier.count_crossings()) + "\n")
