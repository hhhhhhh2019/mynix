from lexer import lexer, Token


string = """
1 + 2
"""

tokens = lexer(string)


class Node:
    token: Token
    next_token_type: "string"
    childs: list
    parent: object
    id: int

    def __init__(self, token: Token, next: "string" = None,
                 childs: list = [], parent=None):
        self.token = token
        self.next_token_type = next
        self.childs = childs
        self.parent = parent
        self.id = 0

    def __str__(self):
        return str(self.token)

    def __repr__(self) -> str:
        return str(self)

    def print(self, offset=0):
        print("  " * offset + str(self.token))
        for i in self.childs:
            i.print(offset+1)

    def index(self, id: int = 0) -> int:
        self.id = id

        for i in self.childs:
            id = i.index(id + 1)

        return id

    def to_dot(self):
        print(f"{self.id} [label=\"{str(self.token)}\"]")
        print(f"{self.id} -> " + "{", end='')
        for i in self.childs:
            print(i.id, end=' ')
        print("}")

        for i in self.childs:
            i.to_dot()


rules = (
    ((
            ("DEC_NUMBER", "_all")),
        "E1"),

    ((
            ("E1", "STAR"),
            ("STAR", "_all"),
            ("E1", "_all")),
        "E1"),

    ((
            ("E1", "_other")),
        "E"),

    ((
            ("E", "PLUS"),
            ("PLUS", "_all"),
            ("E", "_all")),
        "E"),

    ((
            ("LSBR", "_all"),
            ("Aarg", "RSBR"),
            ("RSBR", "_all")),
        "Array"),

    ((
            ("E", "RSBR")),
        "Aarg"),

    ((
            ("E", "COMMA"),
            ("COMMA", "_all"),
            ("Aarg", "_all")),
        "Aarg"),
)


def pop_token():
    if len(tokens) == 1:
        return Node(tokens.pop(0))
    return Node(tokens.pop(0), tokens[0])


stack = [pop_token()]


while len(tokens) > 0:
    starts_with = 0
    suitable = []

    print(stack)

    for i in rules:
        max_len = 0

        print("================")
        for j in range(min(len(stack), len(i[0]))):
            from_stack = stack[-j-1:]
            from_node = i[0][:j]

            for k in range(j):
                pass

            # from_stack = [(k.token.type, k.next_token_type)
            #               for k in stack[-j-1:]]
            #
            # print(from_stack, i[0][:j])
            #
            # if from_stack == i[0][:j]:
            #     max_len = j

        if max_len == 0:
            continue

        if max_len == len(i[0]):
            suitable.append((i[1], len(i[0])))
        else:
            starts_with += 1

    print(starts_with)
    print(suitable)

    if len(suitable) == 0 and starts_with == 0:
        print("Error")
        break

    if len(suitable) > 1:
        print("Error")
        break

    if len(suitable) == 1:
        node = Node(suitable[0][0])
        for i in range(suitable[0][1]):
            node.childs.append(pop_token())
        node.childs.reverse()
        node.next_token_type = tokens[0].type
        continue

    stack.append(pop_token())
