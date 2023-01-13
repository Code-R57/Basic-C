from nltk.tree import *


text = open("parse_tree.txt", 'r').readline()
# assign your output (generalied list of the syntax tree) to varaible text
# text = "[ <program> [ <declaration> [ int ][ <vars> [ <vars> [ <var> [ <var> [ <letter> [ a ]]][ <letter> [ b ]]]][ , ][ <var> [ <letter> [ c ]]]]][ ; ][ <statements> [ <statement> [ <assignment> [ <var> [ <var> [ <letter> [ a ]]][ <letter> [ b ]]][ = ][ <expression> [ <term> [ <factor> [ <factor> [ <operand> [ <integer_const> [ <integer> [ 1 ]]]]][ * ][ <operand> [ <integer_const> [ <integer> [ 4 ]]]]]]]]][ ; ]]] "


text = text.replace("(", "ob")    #in the syntax tree, 'ob' will display in place of '('
text = text.replace(")", "cb")    #in the syntax tree, 'cb' will display in place of ')'
text = text.replace("[", "(")
text = text.replace("]", ")")


tree = Tree.fromstring(text)
tree.pretty_print(unicodelines=True, nodedist=10)   

