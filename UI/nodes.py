import tokens

class Node(object):
    def __init__(self, original_value : str, node_type : tokens.ParserTokens = tokens.ParserTokens.BASENODE ):
        self.original_value = original_value
        self.node_type = node_type

    def __str__(self):
        return 'Node({value}, {node_type})'.format(
            value=self.original_value,
            node_type=self.ParserTokens
        )

    def __repr__(self):
        return self.__str__()

class TimeNode(Node):
    def __init__(self, original_value : str, duration_number : int, duration_units : tokens.LexerTokens, node_type : tokens.ParserTokens tokens.ParserTokens.TIMENODE):
        super().__init__(original_value, node_type )
        self.duration_number = duration_number
        self.duration_units = duration_units

    def __str__(self):
        return 'TimeNode({duration_number}, {duration_units})'.format(
            duration_number=self.duration_number,
            duration_units=self.duration_units
        )

    def __repr__(self):
        return self.__str__()

class StartSectionNode(Node):
    def __init__(self, original_value : str, node_type : tokens.ParserTokens = tokens.ParserTokens.STARTSECTIONNODE):
        super().__init__(original_value, node_type )
    
    def __str__(self):
        return 'StartSection({node_type})'.format(
            node_type=self.ParserTokens
        )

    def __repr__(self):
        return self.__str__()

class ActivateNode(Node):
    def __init__(self, original_value : str, item : tokens.LexerTokens, setting : tokens.LexerTokens ,node_type : tokens.ParserTokens = tokens.ParserTokens.ACTIVATENODENODE):
        super().__init__(original_value, node_type )
    
    def __str__(self):
        return 'ActivateNode({node_type})'.format(
            node_type=self.ParserTokens
        )

    def __repr__(self):
        return self.__str__()

class PeriodNode(Node):
    def __init__(self, original_value : str, item : tokens.LexerTokens, duration : TimeDurationNode ,node_type : tokens.ParserTokens = tokens.ParserTokens.PERIODNODE):
        super().__init__(original_value, node_type )
        self.item = item
        self.duration = duration
        
    def __str__(self):
        return 'PeriodNode({item},{duration})'.format(
            item=self.item,
            duration=self.duration
        )

    def __repr__(self):
        return self.__str__()

class DurationNode(Node):
    def __init__(self, original_value : str, item : tokens.LexerTokens, duration : TimeDurationNode ,node_type : tokens.ParserTokens = tokens.ParserTokens.DURATIONNODE):
        super().__init__(original_value, node_type )
        self.item = item
        self.duration = duration
        
    def __str__(self):
        return 'DurationNode({item},{duration})'.format(
            item=self.item,
            duration=self.duration
        )

    def __repr__(self):
        return self.__str__()

class SetState(Node):
    def __init__(self, original_value : str, new_state : tokens.LexerTokens node_type : tokens.ParserTokens = tokens.ParserTokens.BASENODE ):
        self.original_value = original_value
        self.node_type = node_type

    def __str__(self):
        return 'SetState({new_state})'.format(
            new_state=self.new_state
        )

    def __repr__(self):
        return self.__str__()

class FunctionCallNode(Node):
    def