from regex import FA_from_regex, FA, FANode
from pprint import pprint
from string import ascii_letters, digits


def FA_to_dot(fa):
    print("digraph 1 {")

    for i in fa.nodes:
        print(f"{i.id} [label=\"{i.value}\"]")
        print(f"{i.id} -> {'{'}{",".join(map(str, i.next))}{'}'}")

    print("}")


regexes = {
    "IF": (r"if", True),
    "INA": (r"ina", True),
    "INB": (r"inb", True),

    # "INHERIT": (r"inherit", True),
    # "IMPORT":  (r"import",  True),
    # "IF":      (r"if",      True),
    # "ELSE":    (r"else",    True),
    # "THEN":    (r"then",    True),
    # "LET":     (r"let",     True),
    # "IN":      (r"in",      True),
    # "NUMBER": r"\d+",
}


fas = {}


for i in regexes:
    fas[i] = FA_from_regex(regexes[i][0])

    for o in fas[i].outputs:
        fas[i].nodes[o].output = i

    for node in fas[i].nodes:
        node.can_undefined = regexes[i][1]


fa = FA(set(), set(), [])


offset = 2

for f in fas:
    fas[f].inputs = set(i + offset for i in fas[f].inputs)
    fas[f].outputs = set(i + offset for i in fas[f].outputs)

    for n in fas[f].nodes:
        n.next = set(i + offset for i in n.next)
        # n.next.update([-2])

    # for i in fas[f].outputs:
    #     n.next.remove(-2)
    #     n.next.update([-1])
    #     n.output = f

    offset += len(fas[f].nodes)


fa = FA({0}, set(), [FANode("", set(), can_undefined=True), FANode("", set(), using=True)])


for f in fas:
    fa.nodes[0].next.update(fas[f].inputs)
    fa.nodes += fas[f].nodes
    fa.outputs.update(fas[f].outputs)


def collapse(rid):
    global fa

    chars = {}

    for i in fa.nodes[rid].next:
        c = fa.nodes[i].value

        if c not in chars:
            chars[c] = set()

        chars[c].update([i])

    found = False

    for c in chars:
        if len(chars[c]) < 2:
            continue

        found = True

        fa.nodes.append(FANode(c, set()))

        for i in chars[c]:
            fa.nodes[-1].next.update(fa.nodes[i].next)

            if i in fa.outputs:
                fa.outputs.remove(i)
                fa.outputs.update([len(fa.nodes)-1])

                fa.nodes[-1].output = fa.nodes[i].output

            for n in fa.nodes:
                if i not in n.next:
                    continue
                n.next.remove(i)
                n.next.update([len(fa.nodes)-1])

    if found:
        return 1

    return 0


queue = [0]
checked = set()

while len(queue) > 0:
    id = queue.pop(0)

    while collapse(id):
        pass

    checked.update([id])
    queue += list(fa.nodes[id].next - checked)


for i in fa.nodes:
    for j in i.next:
        fa.nodes[j].using = True

fa.nodes[0].using = True


id = 0

for i in fa.nodes:
    if not i.using:
        continue

    i.id = id
    id += 1


new_nodes = []

for node in fa.nodes:
    if not node.using:
        continue

    new_next = set()

    for j in node.next:
        new_next.update([fa.nodes[j].id])

    node.next = new_next

    new_nodes.append(node)


new_outputs = set()

for i in fa.outputs:
    new_outputs.update([fa.nodes[i].id])

fa.outputs = new_outputs


fa.nodes = new_nodes


# FA_to_dot(fa)
# pprint(fa)


config = [[-1] * 257 for _ in fa.nodes]


for node in fa.nodes:
    if node.can_undefined:
        for i in ascii_letters:
            config[node.id][ord(i)] = 1

    if node.output != '':
        for i in range(256):  # why vanilla python can't arr.fill(val)?
            config[node.id][i] = 0

        config[node.id][256] = node.output

    for child in node.next:
        config[node.id][ord(fa.nodes[child].value)] = child

for i in range(256):
    config[1][i] = 0
config[1][256] = "UNDEFINED"

for i in ascii_letters + digits:
    config[1][ord(i)] = 1


print(f"static int rules[{len(fa.nodes)}][257] = {'{'}")
for i in config:
    print(f"{'{'}{",".join(map(str, i))}{'}'},")
print("};")
