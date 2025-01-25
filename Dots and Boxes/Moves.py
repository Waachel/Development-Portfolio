from Algorithm import *
from GameBoard import *
from Node import *


# class for types of moves for player and AI
class Moves:
    def __init__(self, size_of_board, num_of_plys):
        current_state = GameBoard([], size_of_board)
        current_state.initialize_game_board()
        self.state = Node(current_state)
        self.num_of_plys = num_of_plys
        self.score = 0

    # player move
    def player(self):
        self.state.draw_board()

        x, y = input("Enter X Y coordinate: ").split()
        x_of_player = int(x)
        y_of_player = int(y)

        # if the spot hasn't been played already
        if (x_of_player, y_of_player) not in self.state.children:
            # take spot
            self.state.create_a_node(x_of_player, y_of_player, False)
            self.state = self.state.children[(x_of_player, y_of_player)]
        else:
            self.state = self.state.children[(x_of_player, y_of_player)]

        print("\n Current Score")
        print("---------------")
        print("Player Score: " + str(self.state.current_player_score))
        print("AI Score: " + str(self.state.current_ai_score))
        print("\n")

        # player move over, AI's turn
        self.ai()

    # AI move
    def ai(self):
        self.state.draw_board()

        # use minimax to determine the best possible move
        move = Algorithm.minimax_algorithm(self.state, self.num_of_plys)

        self.state = self.state.children[(move[0], move[1])]

        print("AI played: \n" + "(", str(move[0]), ", " + str(move[1]), ")", end="\n\n")

        print("\n Current Score")
        print("---------------")
        print("Player Score: " + str(self.state.current_player_score))
        print("AI Score: " + str(self.state.current_ai_score))
        print("\n")

        # if everything is full, see who won
        if len(self.state.children) == 0:
            self.state.Draw()
            self.who_won()
            return

        # AI move over, player's turn
        self.player()

    # check who won
    def who_won(self):
        print("GAME OVER\n")
        if self.state.current_player_score > self.state.current_ai_score:
            print("Player Won!")
            exit()
        elif self.state.current_ai_score > self.state.current_player_score:
            print("AI Won!")
            exit()
        else:
            print("Tie!")
            exit()

    def start(self):
        self.player()
