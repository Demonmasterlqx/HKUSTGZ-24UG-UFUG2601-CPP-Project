import heapq


#==============================================================================
#   NODE CLASS
#==============================================================================
class Node:
    """
    A class to represent a node in the search tree. Each node contains:
    - state: The current configuration of the 8-puzzle.
    - parent: The node that generated this node.
    - g: The cost from the start node to this node (number of moves).
    - h: The estimated cost from this node to the goal (heuristic).
    """
    def __init__(self, state, parent=None, g=0, h=0):
        self.state = state
        self.parent = parent
        self.g = g
        self.h = h

    def f(self):
        """Evaluation function f(n) = g(n) + h(n)"""
        return self.g + self.h

    def __lt__(self, other):
        """
        Comparison method for the priority queue.
        Nodes are compared based on their f-score.
        """
        return self.f() < other.f()
        
    def __eq__(self, other):
        """Equality comparison based on the state."""
        return self.state == other.state

    def __hash__(self):
        """Hash method to add nodes to a set (closed list)."""
        # The state is a list of lists, which is not hashable.
        # Convert it to a tuple of tuples to make it hashable.
        return hash(tuple(map(tuple, self.state)))

#==============================================================================
#   HELPER FUNCTIONS (PROVIDED)
#==============================================================================

def get_goal_positions(goal_state):
    """
    Creates a dictionary mapping each tile value to its (row, col)
    position in the goal state. This is pre-calculated for efficiency.
    """
    positions = {}
    for r, row_list in enumerate(goal_state):
        for c, tile in enumerate(row_list):
            positions[tile] = (r, c)
    return positions

def calculate_manhattan_distance(state, goal_positions):
    """
    Calculates the Manhattan distance heuristic for a given state.
    h(n) = sum of the Manhattan distances of each tile from its goal position.
    """
    pass

def find_blank(state):
    """Finds the (row, col) of the blank tile (0)."""
    for r, row_list in enumerate(state):
        for c, tile in enumerate(row_list):
            if tile == 0:
                return r, c
    return None

def generate_successors(state):
    """Generates all valid successor states from the current state."""
    pass

def reconstruct_path(node):
    """Traces back from the goal node to the start to get the solution path."""
    path = []
    current = node
    while current is not None:
        path.append(current.state)
        current = current.parent
    return path[::-1] # Reverse the path to get start -> goal order

#==============================================================================
#   A* SOLVER (TO BE COMPLETED)
#==============================================================================

def solve(initial_state, goal_state):
    """
    Solves the 8-puzzle problem using the A* search algorithm.
    
    Args:
        initial_state: The starting configuration of the puzzle.
        goal_state: The target configuration of the puzzle.
        
    Returns:
        A list of states representing the path from the initial to the goal state,
        or None if no solution is found.
    """
    goal_positions = get_goal_positions(goal_state)
    
    # Create the start node
    h_start = calculate_manhattan_distance(initial_state, goal_positions)
    start_node = Node(state=initial_state, parent=None, g=0, h=h_start)
    
    # Initialize the open list (priority queue) and the closed list (set)
    open_list = []
    heapq.heappush(open_list, start_node)
    closed_set = set()

    # --- YOUR A* SEARCH LOGIC GOES HERE ---
    #
    # While the open list is not empty:
    # 1. Pop the node with the lowest f-score from the open list.
    # 2. If it's the goal, reconstruct and return the path.
    # 3. Add the current node's state to the closed set to avoid revisiting.
    # 4. Generate successors for the current node.
    # 5. For each successor:
    #    a. If its state is in the closed set, ignore it.
    #    b. Calculate its g and h scores.
    #    c. Check if it's already in the open list with a lower g-score.
    #       (A simple implementation can just add it, the priority queue handles it)
    #    d. If not in the open list, add it.
    pass
#==============================================================================
#   TESTING AND EVALUATION
#==============================================================================
if __name__ == "__main__":
    # Define the goal state (0 represents the blank space)
    GOAL_STATE = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 0]
    ]

    # Define test cases: {name, initial_state, optimal_length}
    test_cases = [
        {
            "name": "Simple Case (1 move)",
            "initial": [[1, 2, 3], [4, 5, 6], [7, 0, 8]],
            "optimal": 1
        },
        {
            "name": "Moderate Case (4 moves)",
            "initial": [[1, 2, 3], [0, 4, 6], [7, 5, 8]],
            "optimal": 3
        },
        {
            "name": "Complex Case (8 moves)",
            "initial": [[2, 8, 3], [1, 6, 4], [7, 0, 5]],
            "optimal": 8
        },
        {
            "name": "Deeper Case (14 moves)",
            "initial": [[5, 1, 7], [2, 6, 3], [4, 0, 8]],
            "optimal": 21
        },
        {
            "name": "Unsolvable Case",
            "initial": [[8, 1, 2], [0, 4, 3], [7, 6, 5]],
            "optimal": None
        }
    ]

    print("--- Running 8-Puzzle A* Solver Evaluation ---")
    all_passed = True
    for test in test_cases:
        print(f"\n--- Testing: {test['name']} ---")
        solution_path = solve(test["initial"], GOAL_STATE)
        
        if solution_path is None:
            if test["optimal"] is None:
                print("��� PASSED: Correctly identified as unsolvable.")
            else:
                print(f"��� FAILED: Returned no solution, but expected path of length {test['optimal']}.")
                all_passed = False
        else:
            path_length = len(solution_path) - 1 # Number of moves
            if test["optimal"] is None:
                print(f"��� FAILED: Found a solution of length {path_length}, but expected unsolvable.")
                all_passed = False
            elif path_length == test["optimal"]:
                print(f"��� PASSED: Found optimal path of length {path_length}.")
            else:
                print(f"��� FAILED: Found path of length {path_length}, but optimal is {test['optimal']}.")
                all_passed = False
    
    print("\n--- Evaluation Summary ---")
    if all_passed:
        print("������������ Congratulations! All test cases passed! ������������")
    else:
        print("���� Some test cases failed. Keep debugging!")
