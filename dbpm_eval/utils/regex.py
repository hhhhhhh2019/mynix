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


expression: list = []


def parse_number():
    global expression

    s = ""

    while len(expression) > 0 and expression[0] in "0123456789":
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


def parse_set():
    global expression

    node = Node(1, 1, set(), [])

    while True:
        if len(expression) == 0:
            print("Error")
            break

        c = expression.pop(0)

        if c == ']':
            break

        if c == '(':
            node.paral.add(parse_group())

        elif c == "\\":
            c = expression.pop(0)

            if c == 'd':
                node.paral += list("0123456789")
            elif c == 's':
                node.paral += list(" \n\t\r")

        else:
            node.paral.add(c)

    return node


def parse_group(must_br=True):
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
    return FANode(node.paral)


def FA_from_group(node: Node) -> FANode:
    pass


def FA_from_regex(ex: str) -> list:
    global expression

    expression = list(ex)

    node = parse_group(False)

    pprint(node)


# FA_from_regex("(abc(123\\d){1,3})+")
FA_from_regex("[(abc)123]")
