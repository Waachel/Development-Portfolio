from Moves import *


def main():
    while True:

        print("DOTS and BOXES")

        size_of_board = int(input("\nEnter size of board (i.e 2x2 would be 2, 3x3 would be 3): ")) * 2 + 1

        num_of_plys = int(input("\nEnter the number of plies for AI: "))

        # prep for game
        game = Moves(size_of_board, num_of_plys)

        # start game
        game.start()

        exit()


if __name__ == "__main__":
    main()
