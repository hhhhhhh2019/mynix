from dataclasses import dataclass, field
from copy import deepcopy
from string import ascii_letters


@dataclass
class Node:
    min: int
    max: int
    data: set | list | str

    def __hash__(self):
        return hash((self.min, self.max, tuple(self.data)))


@dataclass
class FANode:
    value: str
    next: set[int]
    output: str = ""  # using in lexer_gen.py
    using: bool = False  # using in lexer_gen.py
    can_undefined: bool = False  # using in lexer_gen.py
    id: int = 0  # using in lexer_gen.py


@dataclass
class FA:
    inputs: set[int]
    outputs: set[int]
    nodes: list[FANode]
    can_zero: bool = False
    next: set[int] = field(default_factory=lambda: set())


expression: list = []


def parse_number():
    global expression

    s = ""

    while len(expression) > 0 and expression[0] in "-0123456789":
        s += expression.pop(0)

    return int(s)


def parse_modifier():
    global expression

    match expression[0]:
        case '+':
            expression.pop(0)
            return (1, -1)
        case '*':
            expression.pop(0)
            return (0, -1)
        case '?':
            expression.pop(0)
            return (0, 1)
        case '{':
            expression.pop(0)
            n1 = parse_number()
            c = expression.pop(0)
            if c == '}':
                return (n1, n1)
            elif c != ',':
                print("Error")
            n2 = parse_number()
            expression.pop(0)
            return (n1, n2)
    return (0, 0)


def parse_set() -> Node:
    global expression

    result = Node(1, 1, set())

    while True:
        if len(expression) == 0:
            print("Error")
            break

        c = expression.pop(0)

        if c == ']':
            break

        if c == '(':
            result.data.add(parse_group())
        elif c == '\\':
            c = expression.pop(0)

            if c == 'd':
                result.data.update(set("0123456789"))
            elif c == 's':
                result.data.update(set(" \n\r\t"))
            elif c == 'w':
                result.data.update(set(ascii_letters))
        else:
            result.data.add(c)

    return result


def parse_group(must_br=True) -> Node:
    global expression

    result = Node(1, 1, [])

    while True:
        if len(expression) == 0:
            if must_br:
                print("Error")
            break

        c = expression.pop(0)

        if c == ')':
            if not must_br:
                print("Error")
            break

        if c == '[':
            result.data.append(parse_set())

        elif c == '(':
            result.data.append(parse_group())

        elif c == '\\':
            c = expression.pop(0)

            if c == 'd':
                result.data.append(Node(1, 1, set("0123456789")))
            elif c == 's':
                result.data.append(Node(1, 1, set(" \t\n\r")))
            elif c == 'w':
                result.data.append(Node(1, 1, set(ascii_letters)))

        else:
            result.data.append(Node(1, 1, c))

        if len(expression) == 0:
            continue

        if expression[0] in "+*?{":
            result.data[-1].min, result.data[-1].max = parse_modifier()

    return result


def FA_from_node(node: Node) -> FA:
    if type(node.data) is list:
        return FA_from_group(node)
    return FA_from_set(node)


def FA_from_set(node: Node) -> FA:
    if type(node.data) is str:
        return FA({0}, {0}, [FANode(node.data, set())])

    result = FA(set(), set(), [])

    for i in node.data:
        if type(i) is str:
            result.nodes.append(FANode(i, set()))
            result.inputs.update([len(result.nodes)-1])
            result.outputs.update([len(result.nodes)-1])
        else:
            fa = FA_from_node(i)
            fa.inputs = set(j + len(result.nodes) for j in fa.inputs)
            fa.outputs = set(j + len(result.nodes) for j in fa.outputs)
            for node in fa.nodes:
                node.next = set(i + len(result.nodes) for i in node.next)

            result.nodes += fa.nodes
            result.inputs.update(fa.inputs)
            result.outputs.update(fa.outputs)

    return result


def FA_from_group(node: Node) -> FA:
    result = FA(set(), set(), [])

    fas = []
    fid = 0

    for n in node.data:
        cnodeo = FA_from_node(n)
        cnodeo.id = fid
        fid += 1

        if n.max == -1:
            count = n.min if n.min != 0 else 1

            for i in range(count):
                fas.append(deepcopy(cnodeo))

            if n.min == 0:
                fas[-1].can_zero = True

            for i in fas[-1].outputs:
                fas[-1].nodes[i].next.update(fas[-1].inputs)
        else:
            for i in range(n.max):
                cnode = deepcopy(cnodeo)
                if i >= n.min:
                    cnode.can_zero = True
                fas.append(cnode)

    offset = 0

    for fa in fas:
        fa.inputs = set(i + offset for i in fa.inputs)
        fa.outputs = set(i + offset for i in fa.outputs)

        for node in fa.nodes:
            node.next = set(i + offset for i in node.next)

        offset += len(fa.nodes)

    for id, fa in enumerate(fas):
        for i in range(id + 1, len(fas)):
            if fas[i].id != fa.id:
                fa.next.update([i])
            elif i - id == 1:
                fa.next.update([i])

            if fas[i].can_zero is False:
                break

    for fa in fas:
        result.inputs.update(fa.inputs)

        if fa.can_zero is False:
            break

    outputs = set()
    queue = {len(fas)-1}

    while len(queue) > 0:
        fa = queue.pop()

        outputs.update([fa])

        if not fas[fa].can_zero:
            continue

        for id, i in enumerate(fas):
            if fa not in i.next:
                continue
            if id not in outputs:
                queue.update([id])

    for i in outputs:
        result.outputs.update(fas[i].outputs)

    for i in fas:
        result.nodes += i.nodes

        for j in i.outputs:
            for m in i.next:
                result.nodes[j].next.update(fas[m].inputs)

    return result


def FA_from_regex(ex: str) -> list:
    global expression

    expression = list(ex)

    node = parse_group(False)

    fanode = FA_from_group(node)

    return fanode


# FA_from_regex(r"(123)?(abc){1,4}456")
# pprint(FA_from_regex(r"[a(123)]+hq"))
# FA_from_regex(r"[+-]?[([123456789]\d*[eE][123456789]\d*)([([123456789]\d*\.)(\.\d+)])](\d*)?([eE][\-\+]?[123456789]\d*)?")
# FA_from_regex(r"[+-]?([123456789]\d*[eE][123456789]\d*|(([123456789]\d*\.)|(\.\d+))(\d*)?([eE][\-\+]?[123456789]\d*)?)")
# [+-]?(
#       [123456789]\d*[eE][123456789]\d* |
#       (
#           ([123456789]\d*\.) |
#           (\.\d+)
#       )
#       (\d*)?
#       ([eE][\-\+]?[123456789]\d*)?
# )
