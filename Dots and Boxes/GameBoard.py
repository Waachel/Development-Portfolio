from random import *


# class in charge of game board and it's states
class GameBoard:
    def __init__(self, matrix, size_of_board):
        self.matrix = matrix
        self.size_of_board = size_of_board

    # sets up the game board
    def initialize_game_board(self):
        p = 0
        points = [1, 2, 5, 2, 3, 2, 1, 3, 5, 4, 3, 1, 3, 4, 4, 5]

        for i in range(0, self.size_of_board):
            r = []
            for j in range(0, self.size_of_board):
                if i % 2 == 1 and j % 2 == 1:
                    # set point value here
                    r.append(points[p])
                    p = p + 1
                elif i % 2 == 0 and j % 2 == 0:
                    # print dots
                    r.append('*')
                else:
                    # space for line to be drawn
                    r.append(' ')
            self.matrix.append(r)

    # get matrix
    def get_matrix(self):
        ans = []
        for i in range(0, self.size_of_board):
            r = []
            for j in range(0, self.size_of_board):
                r.append(self.matrix[i][j])
            ans.append(r)
        return ans

    # get current state
    def get_current_state(self):
        return GameBoard(self.get_matrix(), self.size_of_board)

    # draw board
    def draw_matrix(self):

        if self.size_of_board > 9:
            print(" ", end='')
        print("   X-->")
        print("     ", end='')
        for i in range(0, self.size_of_board):
            print(str(i), end='  ')
        print()

        if self.size_of_board > 9:
            print(" ", end='')
        print("Y   ", end='')
        for i in range(0, self.size_of_board):
            print("___", end='')
        print()

        for j in range(self.size_of_board):
            if self.size_of_board > 9 and j < 10:
                print(" ", end='')
            if j == 0:
                print("| " + str(j) + "| ", end='')

            elif j == 1:
                print("v " + str(j) + "| ", end='')
            else:
                print("  " + str(j) + "| ", end='')

            for z in range(self.size_of_board):
                print(str(self.matrix[j][z]), end='  ')
            print()
        print("    ", end='')
        for i in range(0, self.size_of_board):
            print("___", end='')
        print()

    # draws line from player or AI move
    def draw_line(self, i, j, current_score):
        s = 0

        if j % 2 == 0 and i % 2 == 1:
            self.matrix[j][i] = '-'
            if j < self.size_of_board - 1:
                # check if box is complete
                if self.matrix[j+2][i] == '-' and self.matrix[j+1][i+1] == '|' \
                        and self.matrix[j+1][i-1] == '|':
                    s += self.matrix[j+1][i]
                    # if complete add to score
                    current_score = 1 + current_score

            if j > 0:
                # check if box is complete
                if self.matrix[j-2][i] == '-' and self.matrix[j-1][i+1] == '|' and self.matrix[j-1][i-1] == '|':
                    s += self.matrix[j-1][i]
                    # if complete add to score
                    current_score = 1 + current_score


        else:
            self.matrix[j][i] = '|'
            if i < self.size_of_board - 1:
                # check if box is complete
                if self.matrix[j][i+2] == '|' and self.matrix[j+1][i+1] == '-' and self.matrix[j-1][i+1] == '-':
                    s += self.matrix[j][i+1]
                    # if complete add to score
                    current_score = 1 + current_score
            if i > 0:
                # check if box is complete
                if self.matrix[j][i-2] == '|' and self.matrix[j+1][i-1] == '-' and self.matrix[j-1][i-1] == '-':
                    s += self.matrix[j][i-1]
                    # if complete add to score
                    current_score = 1 + current_score
        return s, current_score
