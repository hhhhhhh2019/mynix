from dataclasses import dataclass
from pprint import pprint


@dataclass
class Node:
    min: int
    max: int
    data: set | list | str

    def __hash__(self):
        return hash((self.min, self.max, tuple(self.data)))


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

        else:
            result.data.append(Node(1, 1, c))

        if len(expression) == 0:
            continue

        if expression[0] in "+*?{":
            result.data[-1].min, result.data[-1].max = parse_modifier()

    return result


def FA_from_regex(ex: str) -> list:
    global expression

    expression = list(ex)

    node = parse_group(False)

    pprint(node)


FA_from_regex(r"[+-]?[([123456789]\d*[eE][123456789]\d*)([([123456789]\d*\.)(\.\d+)])](\d*)?([eE][\-\+]?[123456789]\d*)?")
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
