from dataclasses import dataclass
from pprint import pprint


@dataclass
class Node:
    min: int
    max: int
    paral: set
    seq: list

    def __hash__(self):
        return hash((self.min, self.max, tuple(self.paral), tuple(self.seq)))


@dataclass
class FAGroup:
    inputs: list
    outputs: list
    nodes: list


@dataclass
class FANode:
    value: set
    next: set
    can_zero: bool = False


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

    node = Node(1, 1, set(), [])

    while True:
        if len(expression) == 0:
            print("Error")
            break

        c = expression.pop(0)

        if c == ']':
            break

        # if c == '(':
        #     node.paral.add(parse_group())

        if c == "\\":
            c = expression.pop(0)

            if c == 'd':
                node.paral.update(set("0123456789"))
            elif c == 's':
                node.paral.update(set(" \n\t\r"))

        else:
            node.paral.add(c)

    return node


def parse_group(must_br=True) -> Node:
    global expression

    node = Node(1, 1, set(), [])

    while True:
        if len(expression) == 0:
            if must_br:
                print("Error")
            break

        c = expression.pop(0)
        cnode = None

        if c == ')':
            if not must_br:
                print("Error")
            break

        if c == '(':
            cnode = parse_group()

        elif c == '[':
            cnode = parse_set()

        elif c not in "+*?{":
            if c == '\\':
                c = expression.pop(0)

                if c == 'd':
                    cnode = Node(1, 1, set("0123456789"), [])
                elif c == 's':
                    cnode = Node(1, 1, set(" \n\t\r"), [])
            else:
                cnode = Node(1, 1, set(c), [])

        if len(expression) > 0 and expression[0] in "+*?{":
            if cnode is None:
                print("Error")
                break

            cnode.min, cnode.max = parse_modifier()

        if cnode is None:
            print("Error")
            break

        node.seq.append(cnode)

    return node


def FANode_from_char(node: Node) -> FANode:
    return FANode(node.paral, set())


def FANode_from_node(node: Node) -> FANode | FAGroup:
    if len(node.seq) == 0:
        return FANode_from_char(node)
    return FA_from_group(node)


def FA_from_group(node: Node) -> FAGroup:
    result = FAGroup([], [], [])

    for i in node.seq:
        count = max(1, i.min if i.max == -1 else i.max)
        size = 1

        for j in range(count):
            fanode = FANode_from_node(i)

            if isinstance(fanode, FAGroup):
                result.nodes[-1].next.update(
                    set([len(result.nodes) + k for k in fanode.inputs]))
                for m in fanode.nodes:
                    m.next = set([len(result.nodes) + k for k in m.next])

                if i.min <= j:
                    for m in fanode.nodes:
                        m.can_zero = True

                result.nodes += fanode.nodes

                size = len(fanode.nodes)

            else:
                fanode.next.add(len(result.nodes) + 1)

                if i.min <= j:
                    fanode.can_zero = True

                result.nodes.append(fanode)

        if i.max == -1:
            result.nodes[-1].next.add(len(result.nodes) - size)

    for i in range(len(result.nodes)-2, -1, -1):
        j = 0
        next_list = list(result.nodes[i].next)

        while j < len(result.nodes[i].next):
            id = next_list[j]

            if id == len(result.nodes):
                j += 1
                continue

            if not result.nodes[id].can_zero:
                j += 1
                continue

            if result.nodes[id].next.issubset(result.nodes[i].next):
                j += 1
                continue

            result.nodes[i].next.update(result.nodes[id].next)
            next_list = list(result.nodes[i].next)

            j = 0

    for id, i in enumerate(result.nodes):
        result.inputs.append(id)

        if not i.can_zero:
            break

    for id, i in enumerate(result.nodes):
        if len(result.nodes) in i.next:
            result.outputs.append(id)

    return result


def FA_from_regex(ex: str) -> list:
    global expression

    expression = list(ex)

    node = parse_group(False)

    pprint(node)

    group = FA_from_group(node)
    pprint(group)


# FA_from_regex("(abc(123\\d){1,3})+")
FA_from_regex(r"abc(123){2,5}")
