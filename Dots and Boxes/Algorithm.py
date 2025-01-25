# MiniMax Algorithm and Alpha - Beta Pruning class
class Algorithm:

    # Minimax Algorithm
    @staticmethod
    def minimax_algorithm(state, num_of_plys):
        # not_found = True
        # p = 0
        # num_of_points_worth = 0

        # search for available moves
        for i in range(state.current.size_of_board):
            for j in range(state.current.size_of_board):

                # check if spot is empty and not in a children state
                if state.current.matrix[i][j] == ' ' and (j, i) not in state.children:
                    # while not_found:
                        # if state.current.matrix[i+1][j] == p:
                           # num_of_points_worth = num_of_points_worth + p
                           # not_found = True
                        # elif state.current.matrix[i][j + 1] == p:
                           # num_of_points_worth = num_of_points_worth + p
                           # not_found = True
                       # p = p + 1

                    # create node for the available move
                    state.create_a_node(j, i, True)
                    if num_of_plys < 2:
                        return i, j

        tmp_score = 1000000
        i = 0
        j = 0
        for k, z in state.children.items():

            # use alpha to find maximum
            result = Algorithm.maximum(z, num_of_plys - 1, tmp_score)
            if tmp_score > result:
                tmp_score = result
                i = k[0]
                j = k[1]

        return i, j

    # maximum using alpha
    @staticmethod
    def maximum(state, num_of_plys, alpha):
        if num_of_plys == 0:
            return state.current_ai_score
        # not_found = True
        # p = 0
        # num_of_points_worth = 0

        # search for available moves from children of previous possible moves
        for i in range(state.current.size_of_board):
            for j in range(state.current.size_of_board):

                # check if spot is empty and not in a children node
                if state.current.matrix[i][j] == ' ' and (j, i) not in state.children:
                    # while not_found:
                    # if state.current.matrix[i+1][j] == p:
                    # num_of_points_worth = num_of_points_worth + p
                    # not_found = True
                    # elif state.current.matrix[i][j + 1] == p:
                    # num_of_points_worth = num_of_points_worth + p
                    # not_found = True
                    # p = p + 1

                    # create node for the available move
                    state.create_a_node(j, i, False)

        tmp_score = -1000000

        for k, z in state.children.items():
            # use beta to find minimum
            result = Algorithm.minimum(z, num_of_plys - 1, tmp_score)
            if tmp_score < result:
                tmp_score = result
            if result > alpha:
                return result

        return tmp_score

    # minimum using beta
    @staticmethod
    def minimum(state, num_of_plys, beta):
        if num_of_plys == 0:
            return state.current_ai_score
        # not_found = True
        # p = 0
        # num_of_points_worth = 0

        # search for available moves from children of previous possible moves
        for i in range(state.current.size_of_board):
            for j in range(state.current.size_of_board):
                # check if spot is empty and not in a children node
                if state.current.matrix[i][j] == ' ' and (j, i) not in state.children:
                    # while not_found:
                        # if state.current.matrix[i+1][j] == p:
                        # num_of_points_worth = num_of_points_worth + p
                        # not_found = True
                    # elif state.current.matrix[i][j + 1] == p:
                        # num_of_points_worth = num_of_points_worth + p
                        # not_found = True
                    # p = p + 1

                    # create node for the available move
                    state.create_a_node(j, i, True)

        tmp_score = 1000000

        for k, z in state.children.items():
            # use alpha to find maximum
            result = Algorithm.maximum(z, num_of_plys - 1, tmp_score)
            if tmp_score > result:
                tmp_score = result
            if result < beta:
                return result

        return tmp_score
