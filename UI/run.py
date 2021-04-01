import lexer

def main():

    lexer_list = lexer.lexen("language.txt")
    for item in lexer_list:
        print(item)

main()
