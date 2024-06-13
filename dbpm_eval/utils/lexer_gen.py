from regex import FA_from_regex, FA, FANode
from pprint import pprint


regexes = {
    "INHERIT": r"inherit",
    "IMPORT": r"import",
    "IF": r"if",
    "ELSE": r"else",
    "THEN": r"then",
    "LET": r"let",
    "IN": r"in",
    # "NUMBER": r"\d+",
}


fas = {}


for i in regexes:
    fas[i] = FA_from_regex(regexes[i])

    for o in fas[i].outputs:
        fas[i].nodes[o].output = i


fa = FA(set(), set(), [])


offset = 1

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


fa = FA({0}, set(), [FANode("", set())])


for f in fas:
    fa.nodes[0].next.update(fas[f].inputs)
    fa.nodes += fas[f].nodes
    fa.outputs.update(fas[f].outputs)


def collapse(rid):
    global fa

    chars = {}

    for i in fa.nodes[rid].next:
        for c in fa.nodes[i].value:
            if c not in chars:
                chars[c] = set()

            chars[c].update([i])

    found = False

    for c in chars:
        if len(chars[c]) < 2:
            continue

        found = True

        fa.nodes.append(FANode(set(c), set()))

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

    result = False

    for i in fa.nodes[rid].next:
        result |= collapse(i)

    return result


while collapse(0):
    pass


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


config = [[-1] * 257 for _ in fa.nodes]


for node in fa.nodes:
    if node.output != '':
        for i in range(256):  # why vanilla python can't arr.fill(val)?
            config[node.id][i] = 0

        config[node.id][256] = node.output

    for child in node.next:
        for c in fa.nodes[child].value:
            config[node.id][ord(c)] = child


print(f"static int rules[{len(fa.nodes)}][257] = {'{'}")
for i in config:
    print(f"{'{'}{",".join(map(str, i))}{'}'},")
print("};")
