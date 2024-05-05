# from synt_gen import srules
from lexer import Token, lexer

from pprint import pprint


string = """
{
    array = [
        -1 + 2 * 3,
        "qwerty",
        '/home/user/file with space'
    ];

    a.b = {
        f = func (1 + 2) * 3;
    };
}
"""
# string = """
# a = if true then 1 else 2
# """


tokens = lexer(string)


pprint(tokens)
exit()


class Node:
    token: Token
    childs: list
    parent: object
    id: int

    def __init__(self, token: Token, childs: list, parent):
        self.token = token
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


srules = {
    "Start": [["E"]],

    "Set": [
        ["LCBR", "RCBR"],
        ["LCBR", "Sargs", "RCBR"],
    ],

    "Sargs": [
        ["Sarg"],
        ["Sargs", "Sarg"],
    ],

    "Sarg": [
        ["Name", "ASSIGN", "E1", "SEMICOLON"],
    ],

    "Array": [
        ["LSBR", "Aargs", "RSBR"],
        ["LSBR", "E1", "RSBR"],
    ],

    "Aargs": [
        ["E1", "COMMA", "E1"],
        ["Aargs", "COMMA", "E1"],
    ],

    "E": [
        ["Name", "ASSIGN", "E1"],
        ["E1"],
    ],

    "E1": [
        ["E2"],
        ["IF", "E1", "THEN", "E1", "ELSE", "E1"],
    ],

    "E2": [
        ["E2", "AMPERSAND", "E3"],
        ["E2", "CARET", "E3"],
        ["E2", "PIPE", "E3"],
        ["E3"],
    ],

    "E3": [
        ["E3", "PLUS", "E4"],
        ["E3", "MINUS", "E4"],
        ["E4"],
    ],

    "E4": [
        ["E4", "STAR", "E5"],
        ["E4", "SLASH", "E5"],
        ["E5"],
    ],

    "E5": [
        ["DEC_NUMBER"],
        ["FLOAT_NUMBER"],
        ["STRING"],
        ["PATH"],
        ["TRUE"],
        ["FALSE"],
        ["Set"],
        ["Array"],
        ["LBR", "E1", "RBR"],
        ["Name"],
        ["Func_decl"],
        ["Call"],
        ["EXCLAMATION", "E2"],
    ],

    "Name": [
        ["UNDEFINED"],
        ["Name", "DOT", "UNDEFINED"],
    ],

    "Func_decl": [
        ["Name", "COLON", "E1"],
        ["Set", "COLON", "E1"],
    ],

    "Call": [
        ["Name", "E1"],
    ],
}


stack = []

stack.append(Node(tokens.pop(0), [], None))

while len(tokens) > 0:
    start_with = []
    suitable = []

    # input()
    # print(stack)

    for type in srules:
        for rule in srules[type]:
            if rule == [i.token.type for i in stack[-len(rule):]]:
                suitable.append((type, len(rule)))
                continue

            ok = False
            max_len = 0

            for i in range(len(rule)):
                if rule[:i+1] == [j.token.type for j in stack[-i-1:]]:
                    ok = True
                    max_len = i+1
                    break

            if ok:
                start_with.append((type, max_len, rule))

    # print(start_with)
    # print(suitable)

    if len(suitable) == 0:
        stack.append(Node(tokens.pop(0), [], None))
        continue

    sw_new = []

    min_len = min([i[1] for i in suitable])

    for i in start_with:
        # is terminal and suitable
        if all([j.isupper() for j in i[2][i[1]]]) \
           and i[2][i[1]] != tokens[0].type:
            continue

        if len(i[2]) > min_len:
            sw_new.append(i)

    # print("sw_new:", sw_new)

    if len(sw_new) != 0:
        stack.append(Node(tokens.pop(0), [], None))
        continue

    s = sorted(suitable, key=lambda x: x[1])[-1]

    node = Node(Token("", s[0]), [], None)
    for i in range(s[1]):
        node.childs.append(stack.pop())
    node.childs.reverse()
    stack.append(node)

stack.pop(-1)  # remove EOI

# for i in stack:
#     i.print()

for i in stack:
    i.index()

did = 1
for i in stack:
    print(f"digraph {did} " + "{")
    i.to_dot()
    print("}")
    did += 1
