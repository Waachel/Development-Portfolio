
class Node:
    def __init__(self, current_state):
        self.current = current_state
        self.current_player_score = 0
        self.current_ai_score = 0
        self.children = {}

    # create a new node
    def create_a_node(self, i, j, player):
        self.children[(i, j)] = Node(self.current.get_current_state())

        if player:
            # draw the line from AI's move
            s, score = self.children[(i, j)].current.draw_line(i, j, self.current_ai_score)
            # update score - score won't change if box wasn't completed
            self.children[(i, j)].current_ai_score = score

        else:
            # draw the line from player's move
            s, score = self.children[(i, j)].current.draw_line(i, j, self.current_player_score)
            # update score - score won't change if box wasn't completed
            self.children[(i, j)].current_player_score = score

    # add node
    def add_a_node(self, i, j, child):
        self.children[(i, j)] = child

    # draw board
    def draw_board(self):
        self.current.draw_matrix()
