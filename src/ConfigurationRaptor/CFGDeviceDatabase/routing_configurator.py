import argparse
import os
import shlex
import sys
import copy
import re
import json
import threading
import ast
import time

DIR_IN = 0
DIR_OUT = 1
MAX_THREAD = 4

def match_string(keyword, strings):

  match = -1
  if keyword.find("partial:") == 0:
    keyword = keyword[8:]
    assert len(keyword)
    for i, s in enumerate(strings):
      if s.find(keyword) != -1:
        match = i
        break
  elif keyword.find("RE:") == 0:
    keyword = keyword[3:]
    keyword = keyword.replace("[", "\[")
    keyword = keyword.replace("]", "\]")
    expected_match_count = 0
    for replacement in ["(*s*)", "(*d*)"]:
      index = keyword.find(replacement)
      while index != -1:
        expected_match_count += 1
        index = keyword.find(replacement, index + len(replacement))
      if replacement == "(*s*)":
        keyword = keyword.replace(replacement, "([-+]?[a-zA-Z0-9_.\[\]]+)")
      else:
        keyword = keyword.replace(replacement, "([-+]?[0-9]+)")
    assert len(keyword)
    for i, s in enumerate(strings):
      m = re.search(r"%s\n" % keyword, "%s\n" % s)
      if m != None and len(m.groups()) == expected_match_count:
        match = i
        break
  else:
    if keyword in strings:
      match = strings.index(keyword)
    else:
      match = -1
  return match

class PORT:

  def __init__(self, bit):

    self.bit = bit

class CONFIG_BIT:

  def __init__(self, bit):

    self.bit = bit

class CONFIG_RESULT:

  def __init__(self, feature, instance, bit_name, bit_value):

    self.feature = feature
    self.instance = instance
    self.bit_name = bit_name
    self.bit_value = bit_value

class DIAGRAM:

  def __init__(self, block_name, instance_name, type, instance):

    self.block_name = block_name
    self.instance_name = instance_name
    self.type = type
    self.instance = instance
    self.mapped_edge = {}
    assert self.type in ["IN_PORTS", "OUT_PORTS", "BLOCK", "MUX", "NODE"]
    self.iport_names = ""
    self.oport_names = ""
    self.mux_names = ""
    if type == "NODE":
      self.mapped_edge[instance_name] = instance_name

  def add_ports(self, dir, ports, format):

    assert dir in [DIR_IN, DIR_OUT]
    if dir == DIR_IN:
      assert self.iport_names == ""
    else:
      assert self.oport_names == ""

    def update_mapped_edge(edge, bit, name):

      for i in range(bit):
        if self.type == "BLOCK":
          original_name = "%s->%s%s" % (
            self.instance_name,
            name,
            "" if bit == 1 else ("[%d]" % i),
          )
        else:
          original_name = "%s%s" % (name, "" if bit == 1 else ("[%d]" % i))
        assert original_name not in self.mapped_edge
        self.mapped_edge[original_name] = edge

    # Create port name
    names = ["**INPUT Ports**" if dir == DIR_IN else "**OUTPUT Ports**"]
    mapped_names = []
    for name, p in ports.items():
      edge = "edge%d" % len(self.mapped_edge)
      if (format & 2) == 0 and name in self.instance.diagram_map:
        if self.instance.diagram_map[name] not in mapped_names:
          mapped_names.append(self.instance.diagram_map[name])
          names.append("<%s>%s" % (edge, self.instance.diagram_map[name]))
          update_mapped_edge(edge, p.bit, name)
      else:
        if p.bit == 1:
          names.append("<%s>%s" % (edge, name))
        else:
          names.append("<%s>%s[%d:%d]" % (edge, name, p.bit - 1, 0))
        update_mapped_edge(edge, p.bit, name)

    # Serialize
    if dir == DIR_IN:
      self.iport_names = "|".join(names)
    else:
      self.oport_names = "|".join(names)

  def add_mux_input(self, inputs, values, bits):

    assert len(inputs)
    assert len(inputs) == len(values)
    assert self.iport_names == ""
    assert self.mux_names == ""
    total_size = 0
    bit_infos = [self.block_name]
    msize = 0
    for (b, size) in bits:
      total_size += size
      bit_infos.append("%s - %dbit(s)" % (b, size))
    assert total_size
    dsize = len(str((1 << total_size) - 1))
    names = ["**INPUT Ports**"]
    for i, v in zip(inputs, values):
      edge = "edge%d" % len(self.mapped_edge)
      names.append(
        "<%s>%s\\n0b%0*d (%*d)"
        % (edge, i, total_size, int(bin(v)[2:]), dsize, v)
      )
      names[-1] = names[-1].replace("->", "\\n")
      assert i not in self.mapped_edge
      self.mapped_edge[i] = edge
    self.iport_names = "|".join(names)
    self.mux_names = "|".join(bit_infos)

  def add_mux_output(self, output):

    assert len(output)
    assert self.oport_names == ""
    names = ["**OUTPUT Ports**"]
    edge = "edge%d" % len(self.mapped_edge)
    names.append("<%s>%s" % (edge, output))
    names[-1] = names[-1].replace("->", "\\n")
    assert output not in self.mapped_edge
    self.mapped_edge[output] = edge
    self.oport_names = "|".join(names)

  def get_node_label(self):

    if self.type == "IN_PORTS":
      return "{%s}" % (self.iport_names)
    elif self.type == "OUT_PORTS":
      return "{%s}" % (self.oport_names)
    elif self.type == "BLOCK":
      return "{%s} | {%s|%s} | {%s}" % (
        self.iport_names,
        self.block_name,
        self.instance_name,
        self.oport_names,
      )
    elif self.type == "MUX":
      return "{%s} | {%s} | {%s}" % (
        self.iport_names,
        self.mux_names,
        self.oport_names,
      )
    else:
      assert self.type == "NODE"
      return self.instance_name

class BLOCK:

  def __init__(self, name, top):

    self.name = name
    self.top = top
    self.instantiated = False
    self.instance_name = None
    self.parent = None
    self.id = None
    self.in_ports = {}
    self.out_ports = {}
    self.nodes = []
    self.instances = {}
    self.drives = {}
    self.sinks = {}
    self.mux_drives = {}
    self.mux_sinks = {}
    self.mux_values = {}
    self.mux_bits = {}
    self.config_bits = {}
    self.mux_drive_selection = {}
    self.parameter_primitive_name = None
    self.parameter_script = None
    self.diagram_map = {}

  def fullname(self, do_not_include_if_not_exist=False):

    if self.parent == None:
      assert self.top
      if do_not_include_if_not_exist:
        return ""
      else:
        return self.instance_name
    else:
      assert not self.top
      parent_name = self.parent.fullname(do_not_include_if_not_exist)
      if len(parent_name):
        return "%s.%s" % (parent_name, self.instance_name)
      else:
        return self.instance_name

  def update_child(self, instances, ignore=False):

    for name, instance in self.instances.items():
      assert instance.instance_name == None or ignore
      assert instance.parent == None or ignore
      assert instance.id == None or ignore
      instance.instance_name = name
      instance.parent = self
      instance.id = len(instances)
      instances.append(instance)
      instance.update_child(instances, ignore)

  def check_naming(self, name, support_indexing, feature):

    m = re.search(r"([-+]?[a-zA-Z0-9_]+)", name)
    if m != None and len(m.groups()) == 1 and m.group(1) == name:
      pass
    elif support_indexing:
      # Retry
      valid = False
      index = name.find("[")
      if index != -1 and name[-1] == "]":
        regular_name = name[:index]
        if len(regular_name):
          m = re.search(r"([-+]?[a-zA-Z0-9_]+)", regular_name)
          if (
            m != None
            and len(m.groups()) == 1
            and m.group(1) == regular_name
          ):
            sub_name = name[index + 1 : -1]
            if len(sub_name):
              m = re.search(r"([-+]?[0-9]+)", sub_name)
              if (
                m != None
                and len(m.groups()) == 1
                and m.group(1) == sub_name
              ):
                valid = True
                name = regular_name
      if not valid:
        raise Exception(
          "Invalid %s name %s which should consist character in re format {a-zA-Z0-9_} or {a-zA-Z0-9_}[{0-9}]"
          % (feature, name)
        )
    else:
      raise Exception(
        "Invalid %s name %s which should consist character in re format {a-zA-Z0-9_}"
        % (feature, name)
      )
    return name

  def add_port(self, name, dir, bit):

    assert dir in [DIR_IN, DIR_OUT]
    assert bit > 0
    self.check_naming(name, False, "port")
    assert name not in self.in_ports, (
      "Port %s had been defined as input port" % name
    )
    assert name not in self.out_ports, (
      "Port %s had been defined as ouput port" % name
    )
    for n in self.nodes:
      n = self.check_naming(n, True, "node")
      assert name != n, "Port %s had been defined as node" % name
    if dir == DIR_IN:
      self.in_ports[name] = PORT(bit)
    else:
      self.out_ports[name] = PORT(bit)

  def add_node(self, name):

    no_index_name = self.check_naming(name, True, "node")
    assert name not in self.in_ports, (
      "Node %s had been defined as input port" % name
    )
    assert name not in self.out_ports, (
      "Node %s had been defined as output port" % name
    )
    assert name not in self.nodes, "Node %s had been defined as node" % name
    assert no_index_name not in self.in_ports, (
      "Node %s had been defined as input port" % no_index_name
    )
    assert no_index_name not in self.out_ports, (
      "Node %s had been defined as output port" % no_index_name
    )
    assert no_index_name not in self.nodes, (
      "Node %s had been defined as node" % no_index_name
    )
    self.nodes.append(name)

  def add_instance(self, name, block):

    self.check_naming(name, True, "instance")
    assert name not in self.instances, "Block %s already has instance named %s" % (
      self.name,
      name,
    )
    assert isinstance(block, BLOCK)
    self.instances[name] = block

  def check_connection_bit(self, is_input, connection):

    if connection in self.nodes:
      return connection
    else:
      assert is_input in [None, False, True]
      m = re.search(r"([-+]?[a-zA-Z0-9_]+)\[([-+]?[0-9]+)\]", connection)
      if m != None:
        assert len(m.groups()) == 2
        name = m.group(1)
        bit = int(m.group(2))
      else:
        name = connection
        bit = None
      if is_input == None:
        assert (
          name in self.in_ports or name in self.out_ports
        ), "Block %s does not have input/output port named %s" % (
          self.name,
          name,
        )
        if name in self.in_ports:
          bits = self.in_ports[name]
        else:
          bits = self.out_ports[name]
      elif is_input == True:
        assert (
          name in self.in_ports
        ), "Block %s does not have input port named %s" % (self.name, name)
        bits = self.in_ports[name]
      else:
        assert is_input == False
        assert (
          name in self.out_ports
        ), "Block %s does not have output port named %s" % (self.name, name)
        bits = self.out_ports[name]
      assert (bit == None and bits.bit == 1) or (
        bit != None and bit < bits.bit
      ), (
        "Block %s port %s is %d bit(s), connection should be made bit by bit or within valid bit range"
        % (self.name, name, bits.bit)
      )
      if bits.bit == 1:
        # Always return no []
        return name
      else:
        return connection

  def get_connection_info(self, connection):

    instance = None
    port = None
    assert isinstance(connection, str)
    cons = connection.split("->")
    assert len(cons) in [1, 2]
    if len(cons) == 1:
      # This is a pin
      port = cons[0]
      instance = self
    else:
      # This is instance
      if cons[0] == self.fullname():
        instance = self
      elif cons[0] in self.instances:
        instance = self.instances[cons[0]]
      elif self.parent != None and cons[0] == self.parent.fullname():
        instance = self.parent
      else:
        for inst in self.instances:
          if self.instances[inst].fullname() == cons[0]:
            instance = self.instances[inst]
            break
        if instance == None and self.parent != None:
          for inst in self.parent.instances:
            if self.parent.instances[inst].fullname() == cons[0]:
              instance = self.parent.instances[inst]
              break
        assert (
          instance != None
        ), "Block %s (%s) does not have instance named %s" % (
          self.name,
          self.fullname(),
          cons[0],
        )
      port = cons[1]
    assert isinstance(instance, BLOCK)
    port = instance.check_connection_bit(None, port)
    return [instance, port]

  def check_connection(self, connection, is_drive):

    assert isinstance(connection, str)
    cons = connection.split("->")
    assert len(cons) in [1, 2]
    if len(cons) == 1:
      # This is a pin
      return self.check_connection_bit(is_drive, cons[0])
    else:
      # This is instance
      # Node is not accessible by instance
      assert cons[0] in self.instances, (
        "Instance %s does not exist to make connection" % cons[0]
      )
      instance = self.instances[cons[0]]
      assert (
        cons[1] not in instance.nodes
      ), "Block %s (any block) should not access instance %s node %s" % (
        self.name,
        cons[0],
        cons[1],
      )
      return "%s->%s" % (
        cons[0],
        instance.check_connection_bit(not is_drive, cons[1]),
      )

  def add_connection(self, source, destinations):

    # Pin   [input]   -> Instance   [input]
    # Instance  [output]  -> Instance   [input]
    # Instance  [output]  -> Pin  [output]
    # Pin   [input]   -> Pin  [output]
    for i in range(len(destinations)):
      assert (
        destinations[i] not in self.nodes
      ), "Block %s (any block) does not support add connection to a node %s" % (
        self.name,
        destinations[i],
      )
      destinations[i] = self.check_connection(destinations[i], False)
    assert (
      source not in self.nodes
    ), "Block %s (any block) does not support add connection from a node %s" % (
      self.name,
      source,
    )
    source = self.check_connection(source, True)
    # assert source not in self.drives, "Block %s driving source %s connection had been made" % (self.name, source)
    if source in self.drives:
      for dest in destinations:
        assert (
          dest not in self.drives[source]
        ), "Block %s driving source %s already drive %s" % (
          self.name,
          source,
          dest,
        )
        self.drives[source].append(dest)
    else:
      self.drives[source] = destinations
    for dest in destinations:
      dest = self.check_connection(dest, False)
      assert (
        dest not in self.sinks
      ), "Block %s sink destination %s connection had been made" % (
        self.name,
        dest,
      )
      self.sinks[dest] = source

  def add_config_mux(self, out, selection, bits):

    out = self.check_connection(out, False)
    assert (
      out not in self.mux_sinks
    ), "Block %s mux sink destination %s connection had been made" % (
      self.name,
      out,
    )
    assert out not in self.mux_values, "Block %s mux value %s had been defined" % (
      self.name,
      out,
    )
    assert out not in self.mux_bits, "Block %s mux bits %s had been defined" % (
      self.name,
      out,
    )
    self.mux_sinks[out] = []
    self.mux_values[out] = []
    self.mux_bits[out] = []
    total_bits = 0
    bit_names = []
    for bit in bits:
      for b, size in bit.items():
        self.check_naming(b, True, "mux-bit")
        assert (
          b not in bit_names
        ), "Block %s configuration bits %s had been defined" % (self.name, b)
        if b in self.config_bits:
          assert self.config_bits[b].bit == size, (
            "Block %s configuration bits %s has conflict bit size definition (%d defined vs %d defining)"
            % (self.name, b, self.config_bits[b].bit, size)
          )
        else:
          self.config_bits[b] = CONFIG_BIT(size)
        bit_names.append(b)
        total_bits += size
    for value, drive in selection.items():
      drive = self.check_connection(drive, True)
      assert value < (1 << total_bits), (
        "Block %s configuration mux %s selection value 0x%X (%d) (drive: %s) is out of range (bit-size=%d)"
        % (self.name, out, value, value, drive, total_bits)
      )
      if drive not in self.mux_drives:
        self.mux_drives[drive] = []
      assert out not in self.mux_drives[drive]
      self.mux_drives[drive].append(out)
      self.mux_sinks[out].append(drive)
      self.mux_values[out].append(value)
      # bits
      values = {}
      index = 0
      for bit in bits:
        for b, size in bit.items():
          v = (value >> index) & ((1 << size) - 1)
          index += size
          values[b] = str(v)
      drive_path = "%s | %s" % (drive, out)
      assert drive_path not in self.mux_drive_selection
      self.mux_drive_selection[drive_path] = values
    for bit in bits:
      for b, size in bit.items():
        self.mux_bits[out].append([b, size])

  def add_parameter(self, primitive_name, script):

    assert self.parameter_primitive_name == None
    assert self.parameter_script == None
    self.parameter_primitive_name = primitive_name
    self.parameter_script = script

  def add_diagram_map(self, name, mapped_name):

    assert name not in self.diagram_map
    assert name in self.in_ports or name in self.out_ports
    self.diagram_map[name] = mapped_name

  def recursive_query(
    self,
    start_node,
    paths,
    configs,
    end_node=None,
    filters=None,
    path=[],
    config=[None],
  ):

    instance_node = "%s->%s" % (self.instance_name, start_node)
    full_instance_node = "%s->%s" % (self.fullname(), start_node)
    path = list(path)
    path.append(full_instance_node)
    if filters != None:
      assert isinstance(filters, list)
      for f in filters:
        assert isinstance(f, str) and len(f) > 0
        if match_string(f, [full_instance_node]) != -1:
          return
    if end_node != None and match_string(end_node, [full_instance_node]) != -1:
      paths.append(path)
      configs.append(config)
      return
    found = False
    if start_node in self.drives:
      for dest in self.drives[start_node]:
        (instance, port) = self.get_connection_info(dest)
        instance.recursive_query(
          port, paths, configs, end_node, filters, path, config + [None]
        )
      found = True
    if self.parent != None and instance_node in self.parent.drives:
      for dest in self.parent.drives[instance_node]:
        (instance, port) = self.parent.get_connection_info(dest)
        instance.recursive_query(
          port, paths, configs, end_node, filters, path, config + [None]
        )
      found = True
    if start_node in self.mux_drives:
      for dest in self.mux_drives[start_node]:
        (instance, port) = self.get_connection_info(dest)
        c = "%s | %s" % (start_node, dest)
        assert c in self.mux_drive_selection
        c = self.mux_drive_selection[c]
        instance.recursive_query(
          port,
          paths,
          configs,
          end_node,
          filters,
          path,
          config + [{self.fullname(True): c}],
        )
      found = True
    if self.parent != None and instance_node in self.parent.mux_drives:
      for dest in self.parent.mux_drives[instance_node]:
        (instance, port) = self.parent.get_connection_info(dest)
        c = "%s | %s" % (instance_node, dest)
        assert c in self.parent.mux_drive_selection
        c = self.parent.mux_drive_selection[c]
        instance.recursive_query(
          port,
          paths,
          configs,
          end_node,
          filters,
          path,
          config + [{self.parent.fullname(True): c}],
        )
      found = True
    if not found:
      paths.append(path)
      configs.append(config)

  def query(self, start_node, end_node=None, filters=None):

    assert end_node == None or (isinstance(end_node, str) and len(end_node) > 0)
    assert filters == None or (isinstance(filters, list))
    potential_paths = []
    potential_configs = []
    paths = []
    configs = []
    start_node = self.check_connection(start_node, True)
    self.recursive_query(
      start_node, potential_paths, potential_configs, end_node, filters
    )
    assert isinstance(potential_paths, list)
    assert isinstance(potential_configs, list)
    assert len(potential_paths) == len(potential_configs)
    for p, c in zip(potential_paths, potential_configs):
      assert isinstance(p, list)
      assert isinstance(c, list)
      assert len(p)
      assert len(p) == len(c)
      if end_node != None:
        if match_string(end_node, [p[-1]]) != -1:
          paths.append(p)
          configs.append(c)
      else:
        paths.append(p)
        configs.append(c)
    return [paths, configs]

  def graph_diagram(self, graph, format):
    def get_edge(diagrams, is_self_connection, instance_name, connection, is_mux):

      if is_self_connection:
        name = connection
      else:
        name = "%s->%s" % (instance_name, connection)
      edge = ""
      for diagram in diagrams:
        if (
          is_mux
          and diagram.type == "MUX"
          and diagram.instance_name == instance_name
        ) or (not is_mux and diagram.type != "MUX"):
          if name in diagram.mapped_edge:
            if diagram.type == "NODE":
              edge = "%s" % (diagram.instance_name)
            else:
              edge = "%s:%s" % (
                diagram.instance_name,
                diagram.mapped_edge[name],
              )
            break
      return edge

    def make_mux_connection(
      connections, connection_pairs, diagrams, mux_name, connection, m2b
    ):

      mux_edge = get_edge(diagrams, True, mux_name, connection, True)
      assert len(mux_edge)
      inst, connection = self.get_connection_info(connection)
      edge = get_edge(
        diagrams, inst == self, inst.instance_name, connection, False
      )
      if len(edge):
        if m2b:
          connection_name = "%s + %s" % (mux_edge, edge)
        else:
          connection_name = "%s + %s" % (edge, mux_edge)
        if connection_name not in connections:
          connection_pairs.append(
            [mux_edge, edge] if m2b else [edge, mux_edge]
          )
          connections.append(connection_name)

    graph.rankdir = "LR"
    diagrams = []
    # We must have input ports
    assert len(self.in_ports)
    diagrams.append(DIAGRAM(self.name, "@in", "IN_PORTS", self))
    diagrams[-1].add_ports(DIR_IN, self.in_ports, format)

    # Add instance as "BLOCK"
    for inst_name, instance in self.instances.items():
      diagrams.append(DIAGRAM(instance.name, inst_name, "BLOCK", instance))
      diagrams[-1].add_ports(DIR_IN, instance.in_ports, format)
      diagrams[-1].add_ports(DIR_OUT, instance.out_ports, format)

    mux_names = {}
    for out, selections in self.mux_sinks.items():
      assert len(selections) == len(self.mux_values[out])
      name = "@mux%d" % len(mux_names)
      mux_names[out] = name
      diagrams.append(DIAGRAM("MUX", name, "MUX", self))
      diagrams[-1].add_mux_input(
        selections, self.mux_values[out], self.mux_bits[out]
      )
      diagrams[-1].add_mux_output(out)

    for node in self.nodes:
      diagrams.append(DIAGRAM("NODE", node, "NODE", self))

    # Not neccessary for output ports
    diagrams.append(DIAGRAM(self.name, "@out", "OUT_PORTS", self))
    diagrams[-1].add_ports(DIR_OUT, self.out_ports, format)

    for diagram in diagrams:
      graph.node(
        diagram.instance_name,
        label=diagram.get_node_label(),
        shape=("circle" if diagram.type == "NODE" else "record"),
      )

    connections = []
    connection_pairs = []
    for src, destinations in self.drives.items():
      inst, connection = self.get_connection_info(src)
      src_edge = get_edge(
        diagrams, inst == self, inst.instance_name, connection, False
      )
      if len(src_edge):
        for destination in destinations:
          inst, connection = self.get_connection_info(destination)
          dest_edge = get_edge(
            diagrams, inst == self, inst.instance_name, connection, False
          )
          if len(dest_edge):
            connection_name = "%s + %s" % (src_edge, dest_edge)
            if connection_name not in connections:
              connection_pairs.append([src_edge, dest_edge])
              connections.append(connection_name)

    for out, selections in self.mux_sinks.items():
      make_mux_connection(
        connections, connection_pairs, diagrams, mux_names[out], out, True
      )
      if len(selections) <= 10 or (format & 4) != 0:
        for s in selections:
          make_mux_connection(
            connections,
            connection_pairs,
            diagrams,
            mux_names[out],
            s,
            False,
          )

    # RGB
    if len(connection_pairs):
      color_groups = (len(connection_pairs) + 7) // 8
      color_step = int(((255 + color_groups - 1) / color_groups) * 0.8)
      lcolor = 0
      ccolor = color_step
      for i, pair in enumerate(connection_pairs):
        rcolor = ccolor if (i & 1) else lcolor
        gcolor = ccolor if (i & 2) else lcolor
        bcolor = ccolor if (i & 4) else lcolor
        color = "#%02X%02X%02X" % (rcolor, gcolor, bcolor)
        graph.edge(pair[0], pair[1], color=color)
        if (i % 8) == 7:
          lcolor += color_step // 2
          ccolor += color_step

class PathFinderThread(threading.Thread):

  def __init__(self, configurator, routing):

    threading.Thread.__init__(self)
    self.configurator = configurator
    self.routing = routing
    self.paths = None
    self.configs = None
    self.status = False
    self.error_msg = ""
    self.parameters = {}
    self.defined_parameters = {}

  def run(self):

    assert "feature" in self.routing
    assert "source" in self.routing
    assert "destinations" in self.routing
    assert "flags" in self.routing
    assert "filters" in self.routing
    assert "parameters" in self.routing
    feature = self.routing["feature"]
    src = self.routing["source"]
    dests = self.routing["destinations"]
    flags = self.routing["flags"]
    filters = self.routing["filters"]
    parameters = self.routing["parameters"]
    assert self.configurator.top_block != None
    assert isinstance(src, str) and len(src)
    assert isinstance(dests, list)
    assert isinstance(flags, list)
    assert isinstance(filters, list)
    (src, self.error_msg) = self.configurator.validate_node(src, True)
    if len(self.error_msg) == 0:
      self.routing["source"] = src
      for i, dest in enumerate(dests):
        assert isinstance(dest, str) and len(dest)
        (dests[i], self.error_msg) = self.configurator.validate_node(
          dests[i], False
        )
        if len(self.error_msg):
          break
      if len(self.error_msg) == 0:
        self.routing["destinations"] = dests
        for flag in flags:
          assert isinstance(flag, str) and len(flag)
        for filter in filters:
          assert isinstance(filter, str) and len(filter)
        assert src not in dests
        (self.paths, self.configs) = self.configurator.query(
          src, None if len(dests) == 0 else dests[-1], filters
        )
        self.apply_parameters(parameters)
        self.configurator.validate_paths_and_configs(self.paths, self.configs)
        (
          self.paths,
          self.configs,
        ) = self.configurator.confirm_intermediate_path(
          self.paths, self.configs, dests[:-1]
        )
        (self.paths, self.configs) = self.configurator.apply_flag(
          self.paths, self.configs, flags
        )
        self.status = True

  def filter(self, update_routing):

    assert self.paths != None and self.configs != None
    (self.paths, self.configs) = self.configurator.filter_used_path(
      self.routing["feature"], self.paths, self.configs, self.routing["msgs"]
    )
    if update_routing:
      self.routing["potential paths"] = self.paths

  def apply_parameters(self, parameters):

    assert isinstance(parameters, dict)
    if len(parameters):
      for path, config in zip(self.paths, self.configs):
        applied_block = []
        for i in range(len(path)):
          ps = path[i].split("->")
          assert len(ps) == 2
          for inst in self.configurator.instances:
            if (
              inst.fullname() == ps[0]
              and inst.parameter_primitive_name != None
              and inst.parameter_script != None
              and inst.parameter_primitive_name in parameters
              and inst.name not in applied_block
            ):
              self.parameters = parameters[inst.parameter_primitive_name]
              self.defined_parameters = {}
              exec(inst.parameter_script)
              if config[i] == None:
                config[i] = {}
              inst_name = inst.fullname(True)
              if inst_name not in config[i]:
                config[i][inst_name] = {}
              for p, v in self.defined_parameters.items():
                assert isinstance(p, (str, int)) and len(p)
                assert isinstance(v, (str, int))
                if isinstance(v, int):
                  v = str(v)
                assert len(v)
                if p not in config[i][inst_name]:
                  config[i][inst_name][p] = v
                else:
                  assert config[i][inst_name][p] == v
              applied_block.append(inst.name)
              break

class CONFIGURATOR:

  def __init__(self, top_model_file, enable_tcl_map, debug_level, is_external):

    self.top_model_file = top_model_file
    assert os.path.exists(self.top_model_file), (
      "Model file %s must exist" % self.top_model_file
    )
    self.enable_tcl_map = enable_tcl_map
    self.debug_level = debug_level
    self.is_external = is_external
    self.top_directory = os.path.dirname(os.path.abspath(self.top_model_file))
    if self.top_directory not in sys.path:
      sys.path.insert(0, self.top_directory)
    self.sub_directories = []
    self.top_model_file_md5 = None
    self.models_md5 = []
    self.current_model_md5 = None
    self.current_block = None
    self.top_block = None
    self.blocks = []
    self.instances = []
    self.config_results = {}
    self.tcl_map = {}
    self.print_info(1, 0, "Top model file: %s" % self.top_model_file)
    self.load_model_file(self.top_model_file)

  def reset_config_results(self):

    self.config_results = {}

  def print_info(self, level, space, msg):

    if level <= self.debug_level:

      assert space >= 0
      spacing = ""
      while space != 0:
        spacing += "  "
        space -= 1
      print("INFO   : %s%s" % (spacing, msg))

  def print_warning(self, msg):

    if self.is_external:
      print("WARNING: %s" % msg)

  def load_model_file(self, file):

    def load_model(file):
      self.load_model_file(file)

    def create_block(name, top=False):
      self.create_block(name, top)

    def add_port(name, dir, bit=1, parent=None):
      self.add_port(name, dir, bit, parent)

    def add_node(name, parent=None):
      self.add_node(name, parent)

    def add_instance(name, block, parent=None):
      self.add_instance(name, block, parent)

    def add_connection(source, destinations, parent=None):
      self.add_connection(source, destinations, parent)

    def add_config_mux(out, selection, bits, parent=None):
      self.add_config_mux(out, selection, bits, parent)

    def add_parameter(primitive_name, script, parent=None):
      self.add_parameter(primitive_name, script, parent)

    def add_tcl_map(name, tcl_name):
      self.add_tcl_map(name, tcl_name)

    def add_diagram_map(name, mapped_name, parent=None):
      self.add_diagram_map(name, mapped_name, parent)

    assert isinstance(file, str) and len(file)
    if os.path.exists(file):
      pass
    else:
      found = False
      tested_file = [file]
      search_directories = [self.top_directory] + self.sub_directories
      for directory in search_directories:
        relative_file = "%s/%s" % (directory, file)
        relative_file = relative_file.replace("\\", "/")
        while relative_file.find("//") != -1:
          relative_file = relative_file.replace("//", "/")
        if os.path.exists(relative_file):
          file = relative_file
          found = True
          break
        elif relative_file not in tested_file:
          tested_file.append(relative_file)
      if not found:
        raise Exception("Model file(s) (%s) does not exist" % (tested_file))
    current_dir = os.path.dirname(os.path.abspath(file))
    if current_dir != self.top_directory and current_dir not in self.sub_directories:
      self.sub_directories.append(current_dir)
    self.print_info(1, 1, "Load model file: %s" % file)
    # Original intention is to use md5 but it does not work for msys2
    # Fullpath will work too
    # hashlib.sha256(open(file, "rb").read()).hexdigest()
    md5 = os.path.abspath(file)
    if self.top_model_file_md5 == None:
      assert len(self.models_md5) == 0
      assert self.current_model_md5 == None
      self.top_model_file_md5 = md5
      self.current_model_md5 = md5
      exec(open(file).read())
      self.set_top()
    elif self.current_model_md5 == md5:
      raise Exception("Illegal to self-load model file %s" % file)
    elif md5 in self.models_md5:
      self.print_warning("Model file %s had been loaded. Skip" % file)
    else:
      assert md5 != self.top_model_file_md5, (
        "Illegal to load top model file %s more than once" % file
      )
      self.models_md5.append(md5)
      backup_md5 = self.current_model_md5
      self.current_model_md5 = md5
      exec(open(file).read())
      self.current_model_md5 = backup_md5

  def create_block(self, name, top):

    assert isinstance(name, str) and len(name)
    assert isinstance(top, bool)
    self.print_info(1, 2, "Create block %s (Top:%r)" % (name, top))
    assert len(name)
    self.curret_block = None
    for block in self.blocks:
      if block.name == name:
        raise Exception("Block %s had already been created" % name)
      else:
        assert not top or not block.top, (
          "Block %s already has been set as top module, cannot set block %s as top module"
          % (block.name, name)
        )
    if self.curret_block == None:
      self.current_block = BLOCK(name, top)
      self.blocks.append(self.current_block)

  def get_current_block(self, parent):

    assert self.current_block != None, "No block is created"
    if parent != None:
      self.current_block = None
      for block in self.blocks:
        if block.name == parent:
          self.current_block = block
          break
      assert self.current_block == None, (
        "Specified parent %s does not exist" % parent
      )
      self.print_info(
        2, 1, "Switch the current block to %s" % self.current_block.name
      )

  def set_top(self):

    assert self.top_block == None
    assert len(self.instances) == 0
    for b in self.blocks:
      if b.top:
        assert self.top_block == None
        assert not b.instantiated
        self.top_block = b
    assert self.top_block != None, "Not top block is set"
    assert self.top_block.instance_name == None
    assert self.top_block.id == None
    self.top_block.instance_name = self.top_block.name
    self.top_block.id = len(self.instances)
    self.instances.append(self.top_block)
    self.top_block.update_child(self.instances)
    assert self.top_block.parent == None
    assert self.top_block.fullname(True) == ""
    assert self.top_block.fullname() == self.top_block.instance_name

  def copy_block(self, name):

    block = None
    for b in self.blocks:
      if b.name == name:
        b.instantiated = True
        block = copy.deepcopy(b)
        break
    assert block != None, "Could not find block %s" % (name)
    return block

  def add_port(self, name, dir, bit, parent):

    assert isinstance(name, str) and len(name)
    assert isinstance(bit, int) and bit > 0
    assert dir in [DIR_IN, DIR_OUT]
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(2, 3, "Add port %s" % name)
    assert not self.current_block.instantiated, (
      "Block %s had instantiated, it is illegal to add more port"
      % self.current_block.name
    )
    self.current_block.add_port(name, dir, bit)

  def add_node(self, name, parent):

    assert isinstance(name, str) and len(name)
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(2, 3, "Add node %s" % name)
    assert not self.current_block.instantiated, (
      "Block %s had instantiated, it is illegal to add more node"
      % self.current_block.name
    )
    self.current_block.add_node(name)

  def add_instance(self, name, block, parent):

    assert isinstance(name, str) and len(name)
    assert isinstance(block, str) and len(block)
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(2, 3, "Add block %s as instance %s" % (block, name))
    assert not self.current_block.instantiated, (
      "Block %s had instantiated, it is illegal to add more instance"
      % self.current_block.name
    )
    block = self.copy_block(block)
    assert isinstance(block, BLOCK)
    self.current_block.add_instance(name, block)

  def add_connection(self, source, destinations, parent):

    assert isinstance(source, str)
    assert isinstance(destinations, list) and len(destinations) > 0
    for dest in destinations:
      assert isinstance(dest, str)
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(
      3, 2, "Add connection: source=%s, destinations=%s" % (source, destinations)
    )
    self.current_block.add_connection(source, destinations)

  def add_config_mux(self, out, selection, bits, parent):

    assert isinstance(out, str) and len(out)
    assert isinstance(selection, dict) and len(selection)
    for value, drive in selection.items():
      assert isinstance(value, int)
      assert isinstance(drive, str)
    assert isinstance(bits, list) and len(bits)
    for bit in bits:
      assert isinstance(bit, dict) and len(bit) == 1
      for b, size in bit.items():
        assert isinstance(b, str)
        assert isinstance(size, int) and size > 0
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(2, 3, "Add configuration mux for %s" % (out))
    self.current_block.add_config_mux(out, selection, bits)

  def add_parameter(self, primitive_name, script, parent):

    assert isinstance(primitive_name, str) and len(primitive_name)
    assert isinstance(script, str) and len(script)
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.print_info(2, 3, "Add parameter")
    self.current_block.add_parameter(primitive_name, script)

  def add_tcl_map(self, name, tcl_name):

    assert isinstance(name, str) and len(name)
    assert isinstance(tcl_name, str) and len(tcl_name)
    assert name not in self.tcl_map, "Duplicated tcl map: %s" % name
    self.tcl_map[name] = tcl_name

  def add_diagram_map(self, name, mapped_name, parent):

    assert isinstance(name, str) and len(name)
    assert isinstance(mapped_name, str) and len(mapped_name)
    assert parent == None or (isinstance(parent, str) and len(parent))
    self.get_current_block(parent)
    self.current_block.add_diagram_map(name, mapped_name)

  def get_instance_and_node(self, node):

    block = None
    cons = node.split("->")
    if len(cons) == 1:
      # No instance name, so it is top
      block = self.top_block
    else:
      assert len(cons) == 2
      # There is instance
      for instance in self.instances:
        if cons[0] == instance.fullname() or cons[0] == instance.fullname(True):
          block = instance
          break
      assert block != None, "Instance name %s does not exist" % cons[0]
      node = cons[1]
    return [block, node]

  def validate_node(self, node, is_source):

    assert not is_source or (node.find("partial:") != 0 and node.find("RE:") != 0)
    if node.find("partial:") == 0 or node.find("RE:") == 0:
      pass
    else:
      try:
        cons = node.split("->")
        assert len(cons) in [1, 2]
        if len(cons) == 1:
          node = "%s->%s" % (
            self.top_block.name,
            self.top_block.check_connection_bit(
              True if is_source else None, cons[0]
            ),
          )
        elif len(cons) == 2:
          status = False
          for instance in self.instances:
            if cons[0] == instance.fullname() or cons[
              0
            ] == instance.fullname(True):
              status = True
              node = "%s->%s" % (instance.fullname(), cons[1])
              break
          if not status:
            count = 0
            fullname = ""
            for instance in self.instances:
              if cons[0] == instance.instance_name:
                count += 1
                fullname = instance.fullname()
            if count == 1:
              node = "%s->%s" % (fullname, cons[1])
            elif count == 0:
              raise Exception("Instance name %s is invalid" % cons[0])
            else:
              raise Exception(
                "There are more than one instance with name %s, please use more detail name"
                % cons[0]
              )
      except AssertionError as msg:
        msg = str(msg)
        assert len(msg)
        return [node, msg]
      except Exception as error:
        return [node, "Exception: %s: %s" % (type(error).__name__, str(error))]
    return [node, ""]

  def query(self, node, dest, filters):

    assert self.top_block != None
    assert isinstance(node, str)
    assert dest == None or isinstance(dest, str)
    (block, node) = self.get_instance_and_node(node)
    (paths, configs) = block.query(node, dest, filters)
    return [paths, configs]

  def validate_paths_and_configs(self, paths, configs):

    assert isinstance(paths, list)
    assert isinstance(configs, list)
    for p, c in zip(paths, configs):
      assert isinstance(p, list)
      assert isinstance(c, list)
      assert len(p) == len(c)

  def finalize_path(self, paths, configs, routing):

    assert isinstance(paths, list)
    assert isinstance(configs, list)
    assert len(paths)
    assert len(paths) == len(configs)
    for config in configs:
      if config != None:
        assert isinstance(config, dict), config
        assert len(config) == 1
        for instance, muxes in config.items():
          for mux, value in muxes.items():
            imux = "%s->%s" % (instance, mux)
            if imux in self.config_results:
              assert self.config_results[imux].bit_value == value
            else:
              self.config_results[imux] = CONFIG_RESULT(
                routing["feature"], instance, mux, value
              )

  def confirm_intermediate_path(
    self, potential_paths, potential_configs, intermediate_paths
  ):

    self.validate_paths_and_configs(potential_paths, potential_configs)
    assert isinstance(intermediate_paths, list)
    paths = []
    configs = []
    for path, config in zip(potential_paths, potential_configs):
      count = 0
      for ip in intermediate_paths:
        if match_string(ip, path) != -1:
          count += 1
      if count == len(intermediate_paths):
        paths.append(path)
        configs.append(config)
    return [paths, configs]

  def apply_flag(self, potential_paths, potential_configs, flags):

    self.validate_paths_and_configs(potential_paths, potential_configs)
    if len(potential_paths) > 0 and "shortest path" in flags:
      for flag in flags:
        if flag == "shortest path":
          shortest_path = len(potential_paths[0])
          paths = []
          configs = []
          for path, config in zip(potential_paths, potential_configs):
            if len(path) < shortest_path:
              shortest_path = len(path)
          for path, config in zip(potential_paths, potential_configs):
            if len(path) == shortest_path:
              paths.append(path)
              configs.append(config)
          # Update for next flag if we need to support more
          potential_paths = paths
          patontial_configs = configs
    else:
      paths = potential_paths
      configs = potential_configs
    return [paths, configs]

  def filter_used_path(self, feature, potential_paths, potential_configs, msgs):

    self.validate_paths_and_configs(potential_paths, potential_configs)
    paths = []
    configs = []
    for path, config in zip(potential_paths, potential_configs):
      conflict = False
      for c in config:
        if c != None:
          assert isinstance(c, dict)
          assert len(c) == 1
          for instance, muxes in c.items():
            for mux, value in muxes.items():
              imux = "%s->%s" % (instance, mux)
              if imux in self.config_results:
                if self.config_results[imux].bit_value != value:
                  msgs.append(
                    "'%s' had conflict to set config mux %s to value %s, had been set with value %s by '%s'"
                    % (
                      feature,
                      imux,
                      value,
                      self.config_results[imux].bit_value,
                      self.config_results[imux].feature,
                    )
                  )
                  conflict = True
                  break
            if conflict:
              break
        if conflict:
          break
      if not conflict:
        paths.append(path)
        configs.append(config)
    return [paths, configs]

  def find_path(self, routing):

    thread = PathFinderThread(self, routing)
    thread.start()
    thread.join()
    # thread.filter(True)
    return [thread.status, thread.error_msg, thread.paths, thread.configs]

  def finalize_config_mux(self, config_mux):

    assert isinstance(config_mux, list)
    assert len(config_mux)
    if self.enable_tcl_map:
      tcl_config_mux = []
      muxed = False
      for config in config_mux:
        if config != None:
          tcl_config = {}
          assert isinstance(config, dict)
          assert len(config)
          for module, bits in config.items():
            assert isinstance(bits, dict)
            for bit, value in bits.items():
              assert module.find("->") == -1
              assert bit.find("->") == -1
              name = "%s->%s" % (module, bit)
              for p, t in self.tcl_map.items():
                name = name.replace(p, t)
              names = name.split("->")
              assert len(names) == 2
              if names[0] not in tcl_config:
                tcl_config[names[0]] = {}
              tcl_config[names[0]][names[1]] = value
              muxed = True
          tcl_config_mux.append(tcl_config)
        else:
          tcl_config_mux.append(None)
      assert muxed
      return tcl_config_mux
    else:
      return config_mux

  def solve_routings(self, path_file, clean):

    assert os.path.exists(path_file), "Routing JSON %s does not exist" % path_file
    file = open(path_file)
    routings = json.load(file)
    file.close()
    assert isinstance(routings, list) and len(routings)
    # Reset
    restart_routings = []
    for routing in routings:
      assert isinstance(routing, dict)
      temp = {}
      for key in [
        "feature",
        "comments",
        "source",
        "destinations",
        "filters",
        "flags",
        "parameters",
      ]:
        if key in routing:
          temp[key] = routing[key]
        elif key in ["feature"]:
          temp[key] = "NOT-provided"
        elif key in ["comments", "filters", "flags"]:
          temp[key] = []
        elif key in ["parameters"]:
          temp[key] = {}
      restart_routings.append(temp)
    routings = restart_routings
    if clean == None or clean == False:
      threads = []
      running_threads = []
      for routing in routings:
        assert isinstance(routing, dict)
        assert "feature" in routing
        assert "source" in routing
        assert "destinations" in routing
        assert "filters" in routing
        assert "flags" in routing
        routing["msgs"] = []
        thread = PathFinderThread(self, routing)
        thread.start()
        running_threads.append(thread)
        if len(running_threads) == MAX_THREAD:
          running_threads[0].join()
          threads.append(running_threads[0])
          running_threads.pop(0)
      while len(running_threads):
        running_threads[0].join()
        threads.append(running_threads[0])
        running_threads.pop(0)
      assert len(routings) == len(threads)
      for force in [False, True]:
        for thread in threads:
          if "status" not in thread.routing:
            if thread.status:
              thread.filter(not force)
              if len(thread.paths) == 0:
                thread.routing["msgs"].append(
                  "Fail to find any paths %s round"
                  % ("first" if force == False else "second")
                )
                thread.routing["config mux"] = []
                thread.routing["status"] = False
              elif len(thread.paths) == 1:
                self.validate_paths_and_configs(
                  thread.paths, thread.configs
                )
                self.finalize_path(
                  thread.paths[0], thread.configs[0], thread.routing
                )
                thread.routing["config mux"] = self.finalize_config_mux(
                  thread.configs[0]
                )
                thread.routing["status"] = True
              elif force:
                assert "potential paths" in thread.routing
                assert len(thread.routing["potential paths"])
                index = -1
                for i, paths in enumerate(
                  thread.routing["potential paths"]
                ):
                  if len(paths) == len(thread.paths[0]):
                    found = True
                    for p0, p1 in zip(paths, thread.paths[0]):
                      if p0 != p1:
                        found = False
                        break
                    if found:
                      index = i
                      break
                assert index != -1
                thread.routing["msgs"].append(
                  "Force to use first valid path at index #%d" % index
                )
                self.finalize_path(
                  thread.paths[0], thread.configs[0], thread.routing
                )
                thread.routing["config mux"] = self.finalize_config_mux(
                  thread.configs[0]
                )
                thread.routing["status"] = True
            else:
              # There is an error
              if len(thread.error_msg):
                thread.routing["msgs"].append(thread.error_msg)
              else:
                thread.routing["msgs"].append("Unknown error in finding path")
              thread.routing["potential paths"] = []
              thread.routing["config mux"] = []
              thread.routing["status"] = False
    file = open(path_file, "w")
    json.dump(routings, file, indent=2)
    file.close()

  def solve_routing(self, commands):

    assert isinstance(commands, list) and len(commands)
    query_parser = argparse.ArgumentParser(
      prog="Configuration Modeler Query",
      description="Query Configuration Modeler",
    )
    query_parser.add_argument(
      "--source", type=str, required=True, help="Source node"
    )
    query_parser.add_argument(
      "--destinations", type=str, help="Destination nodes seperated by comma"
    )
    query_parser.add_argument(
      "--filters", type=str, help="Filters seperated by comma"
    )
    query_parser.add_argument(
      "--parameters", type=str, help="Parameters dictionary text"
    )
    query_parser.add_argument("--flags", type=str, help="Flags seperated by comma")
    args = query_parser.parse_args(commands)
    self.reset_config_results()
    routing = {"feature": "Query", "source": args.source}
    if args.destinations != None:
      routing["destinations"] = args.destinations.split(",")
    else:
      routing["destinations"] = []
    if args.filters != None:
      routing["filters"] = args.filters.split(",")
    else:
      routing["filters"] = []
    if args.flags != None:
      routing["flags"] = args.flags.split(",")
    else:
      routing["flags"] = []
    if args.parameters != None:
      routing["parameters"] = ast.literal_eval(args.parameters)
      assert isinstance(routing["parameters"], dict)
    else:
      routing["parameters"] = {}
    routing["msgs"] = []
    (status, error_msg, paths, configs) = self.find_path(routing)
    if status:
      self.validate_paths_and_configs(paths, configs)
      self.print_info(0, 0, "Messages:")
      for i, msg in enumerate(routing["msgs"]):
        self.print_info(0, 1, "%d) %s" % (i, msg))
      self.print_info(0, 0, "Potential Paths:")
      for i, (path, config) in enumerate(zip(paths, configs)):
        max = 0
        for p in path:
          if len(p) > max:
            max = len(p)
        for j, (p, c) in enumerate(zip(path, config)):
          i_str = str(i)
          c_str = str(c)
          if c != None:
            assert isinstance(c, dict)
            assert len(c) == 1
            for k, v in c.items():
              c_str = "%s: %s" % (k, str(v))
          if j == 0:
            self.print_info(0, 1, "%s) %-*s (%s)" % (i_str, max, p, c_str))
          else:
            self.print_info(
              0, 1, "%s  %-*s (%s)" % (" " * len(i_str), max, p, c_str)
            )
    elif len(error_msg):
      self.print_info(0, 0, "ErrorMessages: %s" % error_msg)
    else:
      self.print_info(0, 0, "ErrorMessages: Unknown error in finding path")

  def diagram(self, format):

    import graphviz

    for block in self.blocks:
      block.top = False
    for block in self.blocks:
      graph = graphviz.Digraph("%s" % block.name)
      graph.attr("node", shape="record")
      graph.attr(layout="dot", splines="spline")
      graph.attr(nodesep="1", ranksep="2")
      block.top = True
      block.update_child([], True)
      block.graph_diagram(graph, format)
      block.top = False
      graph.render()

def main(external):

  start = time.time()
  parser = argparse.ArgumentParser(
    prog="Configuration Modeler", description="Generate configuration via modeling"
  )
  parser.add_argument(
    "-m", "--model", type=str, required=True, help="Top model input file"
  )
  parser.add_argument("-s", "--solve", type=str, help="Routing JSON to solve")
  parser.add_argument(
    "-q",
    "--query",
    type=str,
    help='Query. Format "--source=??? --destinations=???,??? --filters=???,??? --flags=???,???"',
  )
  parser.add_argument("-t", "--tcl_map", action="store_true", help=argparse.SUPPRESS)
  parser.add_argument("-v", "--debug", type=int, default=0, help="Debug level")
  parser.add_argument("-c", "--clean", action="store_true", help=argparse.SUPPRESS)
  parser.add_argument("-d", "--diagram", type=int, default=0, help=argparse.SUPPRESS)
  args = parser.parse_args()
  configurator = CONFIGURATOR(args.model, args.tcl_map == True, args.debug, external)
  if args.solve != None:
    configurator.solve_routings(args.solve, args.clean)
  if args.query != None:
    configurator.solve_routing(shlex.split(args.query))
  if args.diagram > 0:
    configurator.diagram(args.diagram)
  end = time.time()
  if external: print("Elapsed time: %f" % (end - start))
  
def cpp_entry(top_file, json_file):
  
  assert isinstance(top_file, str)
  assert isinstance(json_file, str)
  sys.argv = ["config_model.py", "--model", top_file, "--solve", json_file, "--tcl_map"]
  main(False)
  return [True]

if __name__ == "__main__":
  main(True)
