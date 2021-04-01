import tokens
from typing import List


def lexen( code_file_name : str) -> List[tokens.Token]:
    code = open(code_file_name, "r")
    code_text = code.read()
    tokens = lexCreateTokens(code_text)
    return tokens

def lexCreateTokens( seperate_words : List[str]) -> List[tokens.Token]:
    if len(seperate_words) == 0:
        return []
    head, *tail = seperate_words
    temp_list = []
    if head == tokens.LexerTokens.SETUP.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.SETUP)]
    elif head == tokens.LexerTokens.LOOP.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.LOOP)]
    elif head == tokens.LexerTokens.WEATHERSTATION.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.WEATHERSTATION)]
    elif head == tokens.LexerTokens.WALKREMINDER.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.WALKREMINDER)]
    elif head == tokens.LexerTokens.WEATHERREMINDER.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.WEATHERREMINDER)]
    elif head == tokens.LexerTokens.BREAKREMINDER.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.BREAKREMINDER)]
    elif head == tokens.LexerTokens.SCARNSURROUNDINGS.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.SCARNSURROUNDINGS)]
    elif head == tokens.LexerTokens.FOLLOWOBJECT.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.FOLLOWOBJECT)]
    elif head == tokens.LexerTokens.ACTIVE.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.ACTIVE)]
    elif head == tokens.LexerTokens.OFF.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.OFF)]
    elif head == tokens.LexerTokens.DURATION.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.DURATION)]
    elif head == tokens.LexerTokens.IS.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.IS)]
    elif head == tokens.LexerTokens.MILLISECOND.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.MILLISECOND)]
    elif head == tokens.LexerTokens.SECOND.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.SECOND)]
    elif head == tokens.LexerTokens.MINUTE.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.MINUTE)]
    elif head == tokens.LexerTokens.HOUR.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.HOUR)]
    elif head == tokens.LexerTokens.NEWLINE.value :
        temp_list += [tokens.Token(head, tokens.LexerTokens.NEWLINE)]

    return temp_list + lexCreateTokens(tail)
