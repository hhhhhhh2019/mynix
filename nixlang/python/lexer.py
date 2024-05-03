from lexer_gen import rules


class Token:
    value: str
    type: str

    def __init__(self, value: str, type: str) -> None:
        self.value = value
        self.type = type

    def __str__(self):
        a = self.value.strip().replace("\"", "\\\"")
        if a == "":
            return str(self.type)
        return str(self.type) + "\n" + a

    def __repr__(self) -> str:
        return str(self)


def lexer(string):
    offset = 0

    token = ""
    tokens = []

    state = 1

    while offset < len(string):
        c = string[offset]

        nstate = -1
        otherid = -1

        for rule in rules[state][0]:
            if "_other" in rules[state][0][rule]:
                otherid = rule

            if c in rules[state][0][rule]:
                nstate = rule

        nstate = nstate if nstate != -1 else otherid

        if nstate == -1:
            print("rules error")
            break
        elif nstate == 0:
            tokens.append(Token(token, rules[state][1]))
            token = ""
            state = 1
        else:
            token += c
            offset += 1
            state = nstate

    return [i for i in tokens if not i.type == "SPACE"] + \
           [Token("EOI", "$")]
