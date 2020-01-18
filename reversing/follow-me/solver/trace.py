import json
import os.path

class MemoryMap:
    def __init__(self, path, base, size):
        self.path = path
        self.name = os.path.basename(path)
        self.base = int(base, 16)
        self.size = int(size, 16)

    def __repr__(self):
        return "MemoryMap(path={}, base={:#x}, size={:#x})".format(self.path, self.base, self.size)

class Trace:
    def __init__(self, trace_file):
        with open(trace_file) as f:
            try:
                self.trace = json.load(f)
            except ValueError as e:
                print("[!] JSON parse error in file {}: {}".format(trace_file, e))
                exit(1)

        self.events = {}
        for event in self.trace:
            event_type = event['event']
            if not event_type in self.events:
                self.events[event_type] = []
            self.events[event_type].append(event)
        
        self.mmap = []
        for event in self.events['image_load']:
            self.mmap.append(MemoryMap(event['image_name'], event['base_addr'], event['image_size']))

        self.branch_i = 0

    def get_image_by_addr(self, addr):
        print(addr, self.mmap)
        for m in self.mmap:
            if m.base <= addr and addr < m.base + m.size:
                return m
        raise Exception("image not found")

    def get_image_name_by_addr(self, addr):
        return self.get_image_by_addr(addr).name

    def get_relative_addr_and_image(self, rebased_addr):
        m = self.get_image_by_addr(rebased_addr)
        return (rebased_addr - m.base, m)
    
    def get_next_branch(self):
        self.branch_i += 1
        return self.events['branch'][self.branch_i - 1]

if __name__ == "__main__":
    import sys

    trace = Trace(sys.argv[1])
    print(trace.get_next_branch())
    print(trace.get_next_branch())
    import ipdb; ipdb.set_trace()